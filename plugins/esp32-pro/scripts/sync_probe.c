/* sync_probe.c — sync GPIO jitter probe. No Python.
 *
 * Reads ESP32 serial lines of the form:
 *     SYNC:<edge_us>
 * Computes edge-to-edge interval histogram: mean, p50, p99, max, jitter.
 * Run before any timing design (hard rule #4: measure before optimizing).
 *
 * Build:
 *     cc -O2 -Wall -Wextra -o sync_probe sync_probe.c
 *
 * Usage:
 *     ./sync_probe /dev/cu.usbmodem101 115200 [--csv out.csv] [--count N]
 */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static volatile sig_atomic_t stop = 0;
static void on_int(int s) { (void)s; stop = 1; }

static int set_baud(int fd, speed_t baud) {
    struct termios t;
    if (tcgetattr(fd, &t) != 0) { perror("tcgetattr"); return -1; }
    cfmakeraw(&t);
    t.c_cflag |= CLOCAL | CREAD;
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;
    if (cfsetispeed(&t, baud) != 0 || cfsetospeed(&t, baud) != 0) {
        perror("cfset*speed"); return -1;
    }
    if (tcsetattr(fd, TCSANOW, &t) != 0) { perror("tcsetattr"); return -1; }
    return 0;
}

static int cmp_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static uint64_t percentile(uint64_t *s, size_t n, double p) {
    if (n == 0) return 0;
    double idx = p * (n - 1);
    size_t lo = (size_t)idx;
    double frac = idx - lo;
    if (lo + 1 >= n) return s[n - 1];
    return (uint64_t)(s[lo] + frac * (s[lo + 1] - s[lo]));
}

int main(int argc, char **argv) {
    const char *port = NULL;
    speed_t baud = B115200;
    const char *csv_path = NULL;
    long count = 0; /* 0 = run forever */

    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--csv") && i + 1 < argc) { csv_path = argv[++i]; continue; }
        if (!strcmp(argv[i], "--count") && i + 1 < argc) { count = strtol(argv[++i], NULL, 10); continue; }
        if (!port) { port = argv[i]; continue; }
        baud = (speed_t)strtol(argv[i], NULL, 10);
    }
    if (!port) {
        fprintf(stderr, "usage: %s PORT [BAUD] [--csv FILE] [--count N]\n", argv[0]);
        return 2;
    }

    /* map numeric baud to termios constant */
    speed_t br;
    switch (baud) {
        case 9600:   br = B9600;   break;
        case 19200:  br = B19200;  break;
        case 38400:  br = B38400;  break;
        case 57600:  br = B57600;  break;
        case 115200: br = B115200; break;
        case 230400: br = B230400; break;
#ifdef B460800
        case 460800: br = B460800; break;
#endif
#ifdef B921600
        case 921600: br = B921600; break;
#endif
        default:
            fprintf(stderr, "unsupported baud %ld (use 9600..921600)\n", (long)baud);
            return 2;
    }

    int fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) { perror("open"); return 1; }
    if (set_baud(fd, br) != 0) { close(fd); return 1; }

    signal(SIGINT, on_int);
    signal(SIGTERM, on_int);

    long edges = 0;
    uint64_t *intervals = NULL;
    size_t ncap = 0, n = 0;
    uint64_t last = 0;
    int have_last = 0;
    uint64_t total = 0;

    fprintf(stderr, "[sync_probe] listening on %s @ %ld (Ctrl-C to stop)\n", port, (long)br);

    char buf[4096];
    size_t buf_len = 0;
    while (!stop && !(count && edges >= count)) {
        ssize_t r = read(fd, buf + buf_len, sizeof(buf) - buf_len - 1);
        if (r < 0) {
            if (errno == EAGAIN || errno == EINTR) { usleep(1000); continue; }
            perror("read"); break;
        }
        if (r == 0) { usleep(1000); continue; }
        buf_len += (size_t)r;
        buf[buf_len] = '\0';

        char *line = buf;
        char *nl;
        while ((nl = strchr(line, '\n')) != NULL) {
            *nl = '\0';
            /* parse SYNC:<us> */
            if (strncmp(line, "SYNC:", 5) == 0) {
                char *end;
                uint64_t ts = strtoull(line + 5, &end, 10);
                if (*end == '\0') {
                    edges++;
                    if (have_last) {
                        uint64_t iv = ts - last;
                        if (n == ncap) {
                            ncap = ncap ? ncap * 2 : 1024;
                            intervals = realloc(intervals, ncap * sizeof(uint64_t));
                            if (!intervals) { perror("realloc"); return 1; }
                        }
                        intervals[n++] = iv;
                        total += iv;
                    }
                    last = ts;
                    have_last = 1;
                    if (edges % 100 == 0) {
                        fprintf(stderr, "[sync_probe] edges=%ld last_interval=%lluus\n",
                                edges, (unsigned long long)(n ? intervals[n-1] : 0));
                    }
                }
            }
            line = nl + 1;
        }
        /* keep the partial tail */
        size_t tail = buf + buf_len - line;
        memmove(buf, line, tail);
        buf_len = tail;
    }

    if (n == 0) {
        fprintf(stderr, "[sync_probe] no intervals captured\n");
        free(intervals);
        close(fd);
        return 1;
    }

    qsort(intervals, n, sizeof(uint64_t), cmp_u64);
    uint64_t p50 = percentile(intervals, n, 0.5);
    uint64_t p99 = percentile(intervals, n, 0.99);
    uint64_t pmax = intervals[n - 1];
    double mean = (double)total / n;
    uint64_t jitter = pmax - p50;

    printf("[sync_probe] edges=%ld\n", edges);
    printf("[sync_probe] mean=%.2fus  p50=%lluus  p99=%lluus  max=%lluus  jitter(pmax-p50)=%lluus\n",
           mean, (unsigned long long)p50, (unsigned long long)p99,
           (unsigned long long)pmax, (unsigned long long)jitter);

    if (csv_path) {
        FILE *f = fopen(csv_path, "w");
        if (!f) { perror("fopen csv"); }
        else {
            fprintf(f, "interval_us\n");
            for (size_t k = 0; k < n; k++) fprintf(f, "%llu\n", (unsigned long long)intervals[k]);
            fclose(f);
            fprintf(stderr, "[sync_probe] wrote %s\n", csv_path);
        }
    }

    free(intervals);
    close(fd);
    return 0;
}
