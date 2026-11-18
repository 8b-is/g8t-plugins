# NEON / ASIMD, SDOT, FP16

## A76 capabilities

- ASIMD/NEON (128-bit vector registers v0–v31)
- FP16 arithmetic (half-precision)
- SDOT / UDOT (dot product, integer)
- RDM (rounding double multiply-add)

## The three-step ladder (hard rule #1)

1. **Portable C** — get a scalar baseline, measure it.
2. **Compiler baseline** — `-O3 -march=native` (or `-mcpu=cortex-a76`), let autovectorisation run, measure it.
3. **NEON intrinsics** — only if C and compiler baseline both fail to meet the target. Measure again.

## When NEON helps (and when it doesn't)

- Helps: dense pixel/audio kernels, dot products, small fixed-size matrix ops, branchless min/max.
- Does not help: pointer-chasing, small trip counts, code already autovectorised by clang `-O3`.

## Realistic expectations

- NEON speedups on A76 are typically ~1.2x–1.8x for suitable kernels. Larger numbers (~2.75x)
  have been reported for full image-quality pipelines but are workload-specific.
- Mark any speedup claim `[PROTOTYPE]` until measured on the user's kernel.

## FP16 caveat

FP16 on A76 is supported but often lower throughput than FP32 for scalar code; vector FP16
wins only when memory-bandwidth bound. Measure before using FP16 for speed.

## SDOT

SDOT does 4× 8-bit dot product per lane pair. Use for INT8 quantised inference kernels,
not general scalar code.
