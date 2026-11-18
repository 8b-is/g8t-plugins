# Perf counters and A76 / x86

## Counter set (Linux `perf stat`)

| Counter | What it tells you |
|---|---|
| cycles | elapsed core cycles |
| instructions | retired instructions |
| IPC | instructions per cycle (derived) |
| cache-references / cache-misses | last-level cache pressure |
| branch-misses | predictor misses |
| stalled-cycles-frontend / stalled-cycles-backend | front-end vs back-end bound |

## `scripts/asm_ladder.sh`

Builds scalar C, `-O3 -march=native`, and an asm variant from one harness, runs N
iterations, prints median/min/max and ratios. No optimisation without a counter (hard
rule #2).

## Interpreting IPC

- IPC ≈ 1.0–1.5: scalar-bound, memory not the bottleneck.
- IPC < 0.5: likely memory-bound or front-end stalled; check cache-misses.
- IPC > 2: good vector utilisation; vectorisation may already be helping.

## Measure end-to-end (hard rule #5)

A faster kernel that changes cache behaviour or adds a copy can lose the whole loop.
Benchmark at the level the user cares about, not just the inner function.

## Regression check

Run neighbouring benchmarks before and after; any slowdown >2% in adjacent code is a
regression and invalidates the optimisation.
