/* spectral_logger.c — parse AS7341 `SPEC|...` lines into CSV. No Python.
 *
 * Expected ESP32 line format (11 channels, pipe-delimited):
 *     SPEC|F1|F2|F3|F4|F5|F6|NIR1|NIR2|CLEAR1|CLEAR2|FIFO
 *
 * Build:
 *     cc -O2 -Wall -Wextra -o spectral_logger spectral_logger.c
 *
 * Usage:
 *     ./spectral_logger /dev/cu.usbmodem101 115200 [--csv out.csv] [--count N]
 */
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

static volatile sig_atomic_t stop = 0;
static void on_int(int s) { (void)s; stop = 1; }

static const char *CHANNELS[11] = {
    "F1", "F2", "F3", "F4", "F5", "F6",
    "NIR1", "NIR2", "CLEAR1", "CLEAR2", "FIFO"
};

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

static speed_t baud_from_long(long b) {
    switch (b) {
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 57600:  return B57600;
#ifdef B460800
        case 460800: return B460800;
#endif
#ifdef B921600
        case 921600: return B921600;
#endif
        default:     return (speed_t)0;
    }
}

/* parse "SPEC|v1|...|v11" into vals[11]. Returns 0 on success. */
static int parse_spec(const char *line, long vals[11]) {
    if (strncmp(line, "SPEC|", 5) != 0) return -1;
    const char *p = line + 5;
    for (int k = 0; k < 11; k++) {
        char *end;
        long v = strtol(p, &end, 10);
        if (end == p) return -1;
        vals[k] = v;
        p = end;
        if (k < 10) {
            if (*p != '|') return -1;
            p++;
        }
    }
    /* allow trailing newline/CR already stripped; ignore anything after 11th value */
    return 0;
}

static uint64_t now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

int main(int argc, char **argv) {
    const char *port = NULL;
    long baud_l = 115200;
    const char *csv_path = "-";
    long count = 0;

    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--csv") && i + 1 < argc) { csv_path = argv[++i]; continue; }
        if (!strcmp(argv[i], "--count") && i + 1 < argc) { count = strtol(argv[++i], NULL, 10); continue; }
        if (!port) { port = argv[i]; continue; }
        baud_l = strtol(argv[i], NULL, 10);
    }
    if (!port) {
        fprintf(stderr, "usage: %s PORT [BAUD] [--csv FILE] [--count N]\n", argv[0]);
        return 2;
    }

    speed_t br = baud_from_long(baud_l);
    if (br == 0) {
        fprintf(stderr, "unsupported baud %ld\n", baud_l);
        return 2;
    }

    int fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) { perror("open"); return 1; }
    if (set_baud(fd, br) != 0) { close(fd); return 1; }

    FILE *out = (strcmp(csv_path, "-") == 0) ? stdout : fopen(csv_path, "w");
    if (!out) { perror("fopen csv"); close(fd); return 1; }
    fprintf(out, "timestamp_us");
    for (int k = 0; k < 11; k++) fprintf(out, ",%s", CHANNELS[k]);
    fprintf(out, "\n");
    fflush(out);

    signal(SIGINT, on_int);
    signal(SIGTERM, on_int);

    fprintf(stderr, "[spectral_logger] listening on %s @ %ld\n", port, baud_l);

    char buf[4096];
    size_t buf_len = 0;
    long readings = 0;
    while (!stop && !(count && readings >= count)) {
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
            long vals[11];
            if (parse_spec(line, vals) == 0) {
                readings++;
                uint64_t ts = now_us();
                fprintf(out, "%llu", (unsigned long long)ts);
                for (int k = 0; k < 11; k++) fprintf(out, ",%ld", vals[k]);
                fprintf(out, "\n");
                if (readings % 50 == 0) fflush(out);
                fprintf(stderr, "[spectral_logger] readings=%ld\n", readings);
            }
            line = nl + 1;
        }
        size_t tail = buf + buf_len - line;
        memmove(buf, line, tail);
        buf_len = tail;
    }

    fflush(out);
    if (out != stdout) fclose(out);
    close(fd);
    return 0;
}
