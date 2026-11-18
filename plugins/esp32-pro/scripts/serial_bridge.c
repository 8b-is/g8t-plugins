/* serial_bridge.c — bidirectional bridge between a host process and the ESP32
 * serial protocol. No Python.
 *
 * ESP32 protocol (one command per line, newline terminated):
 *     W:<int>,WARM:<0-255>,COLD:<0-255>,UV:<0-255>
 *
 * Build:
 *     cc -O2 -Wall -Wextra -o serial_bridge serial_bridge.c
 *
 * Usage:
 *     ./serial_bridge /dev/cu.usbmodem101 115200 [--host-cmd 'my_host']
 *
 * The host command's stdin/stdout are bridged to the serial port:
 *   serial -> host stdin,  host stdout -> serial
 */
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>

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

static speed_t baud_from_long(long b) {
    switch (b) {
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 57600:  return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
#ifdef B460800
        case 460800: return B460800;
#endif
#ifdef B921600
        case 921600: return B921600;
#endif
        default:     return (speed_t)0;
    }
}

int main(int argc, char **argv) {
    const char *port = NULL;
    long baud_l = 115200;
    const char *host_cmd = NULL;

    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--host-cmd") && i + 1 < argc) { host_cmd = argv[++i]; continue; }
        if (!port) { port = argv[i]; continue; }
        baud_l = strtol(argv[i], NULL, 10);
    }
    if (!port) {
        fprintf(stderr, "usage: %s PORT [BAUD] [--host-cmd CMD]\n", argv[0]);
        return 2;
    }

    speed_t br = baud_from_long(baud_l);
    if (br == 0) {
        fprintf(stderr, "unsupported baud %ld\n", baud_l);
        return 2;
    }

    int sfd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (sfd < 0) { perror("open"); return 1; }
    if (set_baud(sfd, br) != 0) { close(sfd); return 1; }

    int child_stdin = -1, child_stdout = -1;
    pid_t pid = 0;

    if (host_cmd) {
        int p_stdin[2], p_stdout[2];
        if (pipe(p_stdin) != 0 || pipe(p_stdout) != 0) {
            perror("pipe"); close(sfd); return 1;
        }
        pid = fork();
        if (pid < 0) { perror("fork"); close(sfd); return 1; }
        if (pid == 0) {
            /* child: host process */
            close(p_stdin[1]);  /* close write end of stdin pipe */
            close(p_stdout[0]); /* close read end of stdout pipe */
            dup2(p_stdin[0], STDIN_FILENO);
            dup2(p_stdout[1], STDOUT_FILENO);
            close(p_stdin[0]);
            close(p_stdout[1]);
            /* close the serial fd in child */
            close(sfd);
            execlp("sh", "sh", "-c", host_cmd, (char *)NULL);
            perror("execlp");
            _exit(127);
        }
        /* parent */
        close(p_stdin[0]);  /* we write to child stdin */
        close(p_stdout[1]); /* we read from child stdout */
        child_stdin = p_stdin[1];
        child_stdout = p_stdout[0];
    }

    fprintf(stderr, "[serial_bridge] port=%s baud=%ld host_cmd=%s\n",
            port, baud_l, host_cmd ? host_cmd : "(none)");

    signal(SIGINT, on_int);
    signal(SIGTERM, on_int);

    if (!host_cmd) {
        /* serial -> stdout only */
        char buf[4096];
        while (!stop) {
            ssize_t r = read(sfd, buf, sizeof(buf));
            if (r < 0) {
                if (errno == EAGAIN || errno == EINTR) continue;
                perror("read"); break;
            }
            if (r == 0) break;
            ssize_t w = write(STDOUT_FILENO, buf, (size_t)r);
            if (w < 0) { if (errno == EINTR) continue; perror("write stdout"); break; }
        }
        close(sfd);
        return 0;
    }

    /* multiplex: serial <-> child */
    while (!stop) {
        fd_set rfds;
        FD_ZERO(&rfds);
        int maxfd = sfd;
        FD_SET(sfd, &rfds);
        if (child_stdout >= 0) { FD_SET(child_stdout, &rfds); if (child_stdout > maxfd) maxfd = child_stdout; }

        struct timeval tv = { .tv_sec = 0, .tv_usec = 100000 };
        int rv = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if (rv < 0) { if (errno == EINTR) continue; perror("select"); break; }

        if (FD_ISSET(sfd, &rfds)) {
            char buf[4096];
            ssize_t r = read(sfd, buf, sizeof(buf));
            if (r > 0) {
                if (child_stdin >= 0) {
                    ssize_t w = write(child_stdin, buf, (size_t)r);
                    if (w < 0 && errno != EAGAIN) { perror("write child"); break; }
                }
                fprintf(stderr, "[esp32] %.*s", (int)r, buf);
                if (r > 0 && buf[r - 1] != '\n') fprintf(stderr, "\n");
            } else if (r == 0) {
                break;
            } else if (errno != EAGAIN && errno != EINTR) {
                perror("read serial"); break;
            }
        }
        if (FD_ISSET(child_stdout, &rfds)) {
            char buf[4096];
            ssize_t r = read(child_stdout, buf, sizeof(buf));
            if (r > 0) {
                ssize_t w = write(sfd, buf, (size_t)r);
                if (w < 0) { if (errno == EINTR) continue; perror("write serial"); break; }
                fprintf(stderr, "[host] %.*s", (int)r, buf);
                if (r > 0 && buf[r - 1] != '\n') fprintf(stderr, "\n");
            } else if (r == 0) {
                break; /* host closed stdout */
            } else if (errno != EAGAIN && errno != EINTR) {
                perror("read host"); break;
            }
        }
    }

    if (child_stdin >= 0) close(child_stdin);
    if (child_stdout >= 0) close(child_stdout);
    close(sfd);
    if (pid > 0) {
        kill(pid, SIGTERM);
        waitpid(pid, NULL, 0);
    }
    return 0;
}
