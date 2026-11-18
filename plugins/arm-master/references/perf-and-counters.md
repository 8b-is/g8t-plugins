# Perf counters and A76

## Counter set for A76-class cores

| Counter | What it tells you |
|---|---|
| cycles | elapsed core cycles |
| instructions | retired instructions |
| IPC | instructions per cycle (derived) |
| L1d misses | data cache pressure |
| branch misses | predictor misses |
| stalled cycles | backend/frontend stalls |

## `scripts/perf_record.sh`

Wraps `perf stat -e cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,branch-misses`
over the user's benchmark. No optimisation without a counter (hard rule #2).

## Interpreting IPC on A76

- IPC ≈ 1.0–1.5: scalar-bound, memory not the bottleneck.
- IPC < 0.5: likely memory-bound or front-end stalled; check L1d misses.
- IPC > 2: good vector utilisation; NEON may already be helping.

## Measure the whole loop (hard rule #5)

A faster kernel that changes cache behaviour or adds a copy can lose end-to-end. Benchmark
at the level the user cares about, not just the inner function.

## Regression check

Run neighbouring benchmarks before and after; any slowdown >2% in adjacent code is a
regression and invalidates the optimisation.
