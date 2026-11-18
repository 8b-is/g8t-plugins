/* bench_harness.c — minimal timing harness template.
 * Compile: cc -O2 -march=native -o bench bench_harness.c <kernel.c|kernel.S>
 * Usage: ./bench [iterations]
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint64_t now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

/* The kernel under test. Defined either in a .c file or in an .S file. */
extern long kernel(long n);

/* Warm up the core and caches before measuring. */
static void warmup(void) {
    for (long i = 0; i < 1000000L; i++) kernel(i);
}

int main(int argc, char **argv) {
    long iters = (argc > 1) ? atol(argv[1]) : 1000000L;
    warmup();

    uint64_t start = now_ns();
    volatile long sink = 0;
    for (long i = 0; i < iters; i++) sink += kernel(i);
    uint64_t end = now_ns();

    double ns_per = (double)(end - start) / (double)iters;
    printf("iters,%ld,total_ns,%llu,ns_per_iter,%.4f,sink,%ld\n",
           iters, (unsigned long long)(end - start), ns_per, sink);
    return 0;
}
