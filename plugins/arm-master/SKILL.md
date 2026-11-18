---
name: arm-master
description: >
  AArch64/ARMv8-A/ARMv9 low-level performance engineering: assembly, NEON / ASIMD
  intrinsics, SDOT/dot-product, FP16, calling convention (AAPCS64), cache behaviour,
  perf counters, and the honest limits of GPU-level programming. Use when the user is
  (a) writing or debugging AArch64 assembly or inline asm, (b) optimising a hot loop with
  NEON intrinsics, (c) reasoning about ABI, stack alignment, or register clobbering, (d)
  reading perf counters or cache-miss behaviour, or (e) asking how to "program the GPU
  directly" on Raspberry Pi / Mali hardware. Also use for "aarch64", "arm64 asm", "neon",
  "sdot", "cortex-a76", "inline assembly", "perf stat", "gpu isa", "videocore assembly",
  "mali shader".
license: MIT
version: 0.1.0
---

# arm-master

Instruction-level and microarchitecture-level skill for ARM64. The central discipline is
**measurement-before-optimisation**: on modern AArch64 cores the compiler is frequently
already optimal, and hand-written assembly can be *slower* — there is public evidence of
exactly this on Cortex-A76.

## 0. Read order

1. This file.
2. `ENGRAM.md`.
3. `references/epistemic-ledger.md` — **mandatory** before any GPU-ISA claim.
4. The routed reference file.

## 1. Routing table

| User signal | Load | Do first |
|---|---|---|
| Assembly, inline asm, ABI, register clobber, stack alignment | `references/aarch64-abi.md` | Establish AAPCS64 constraints before writing a line |
| NEON, intrinsics, vectorisation, dot product, FP16 | `references/neon-sdot-fp16.md` | Get a scalar baseline and a compiler-autovec baseline first |
| "Why is my asm slow", perf stat, cache misses | `references/perf-and-counters.md` | Run `scripts/perf_record.sh`; no optimisation without a counter |
| Bare metal, linker script, exception vectors, MMU | `references/bare-metal-linker.md` | Confirm the boot stage and what firmware already did |
| "Write GPU instructions", VideoCore/Mali ISA, shaders | `references/gpu-isa-reality.md` | Classify against the ledger before answering |
| Board-level facts (peripheral addresses, firmware mediation, driver availability) | hand off to `sbc-pro` | See section 4 cross-skill handoff |

## 2. Hard rules

1. **Three-step ladder.** Portable C → NEON intrinsics → assembly. Each step requires
   evidence from the previous step. Do not start at assembly.
2. **No speed claim without a measurement.** "Assembly is faster" is a hypothesis, not a
   fact. Attach a benchmark command and a number or mark it `[PROTOTYPE]`.
3. **AAPCS64 discipline.** Respect callee-saved registers (x19–x29), keep SP 16-byte aligned
   at public interfaces, never assume a red zone (AAPCS64 has none), and treat x18 as
   reserved/platform. Inline asm must declare clobbers and `"memory"` where relevant, and
   must ship with a C fallback.
4. **GPU-ISA honesty.** Hand-writing GPU ISA is `[VALID]` only for VideoCore IV (public
   reference guide, 2014). V3D 3.x/4.x/7.x requires an NDA; Mali via Panfrost is a
   reverse-engineered open driver, not a documented ISA. For everything else, the correct
   answer is "compute shader via GLES/Vulkan."
5. **Measure the whole loop.** A faster kernel that changes cache behaviour or adds a copy
   can lose end-to-end. Always benchmark at the level the user cares about.
6. **Regressions count.** Every optimisation answer includes the check that proves
   neighbouring code did not slow down.

## 3. Standard deliverable shape

1. **Baseline** — scalar C, with the exact command and number.
2. **Compiler baseline** — `-O3` autovec / `-march=native`, with the number.
3. **Hypothesis** — what the counter says is the bottleneck.
4. **Change** — intrinsics or asm, minimal and reviewable.
5. **Measurement** — same harness, same machine, median of N.
6. **Regression check** — neighbouring benchmarks.
7. **What would falsify this** — the condition under which the change is not worth it.

## 4. Cross-skill handoff

Board-level facts (peripheral addresses, firmware mediation, driver availability) belong to
**`sbc-pro`**. If the answer depends on which kernel or GPU stack is installed, stop and
hand off.

## 5. Scripts

- `asm_bench.sh` — builds a C reference and an asm variant from one harness, runs N
  iterations, prints median/min/max and a ratio.
- `perf_record.sh` — wraps `perf stat` with the counter set that matters for A76-class
  cores (cycles, instructions, IPC, L1d misses, branch misses, stalled cycles).

All scripts are POSIX shell; no Python.
