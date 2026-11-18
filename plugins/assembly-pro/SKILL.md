---
name: assembly-pro
description: >
  Multi-ISA assembly and low-level performance engineering for x86-64 and RISC-V.
  Use when the user is writing or debugging assembly, deciding whether hand-written
  assembly is worth it for a hot path, reasoning about calling conventions (SysV AMD64,
  RISC-V psABI), or reading perf counters and cache behaviour. Also use for "x86-64 asm",
  "riscv asm", "inline asm", "asm vs compiler", "why is my assembly slow", "perf stat",
  "AVX-512", "RISC-V vector". Does NOT cover AArch64 in depth — route to arm-master.
  Does NOT cover 6502/Z80/65816/68000 — route to retro-game-dev.
license: MIT
version: 0.1.0
---

# assembly-pro

Instruction-level and microarchitecture-level skill for **x86-64 and RISC-V**, with a
routing layer for AArch64 (→ `arm-master`) and vintage ISAs (→ `retro-game-dev`).

The central discipline is measurement before optimisation, and a three-step ladder. On
modern out-of-order cores the compiler is frequently already optimal, and hand-written
assembly can be *slower*. The ledger records where hand-asm legitimately wins and where it
does not.

## 0. Read order

1. This file.
2. `ENGRAM.md`.
3. `references/epistemic-ledger.md` — **mandatory** before any "assembly is faster" claim.
4. The routed reference file.

## 1. Routing table

| User signal | Load | Do first |
|---|---|---|
| x86-64 asm, SysV ABI, AVX/SSE, `rdtsc` | `references/x86-64.md` | Establish ABI and the compiler baseline |
| RISC-V asm, RVV, `-march`, psABI | `references/riscv.md` | Pin the ISA extensions in use |
| Inline asm, clobbers, `asm volatile`, constraints | `references/inline-asm-and-abi.md` | Demand a C fallback and a clobber list |
| "Why is my asm slow", perf, cache | `references/perf-and-counters.md` | Run `scripts/asm_ladder.sh` before changing anything |
| AArch64 / NEON / Cortex-A76 | — | **Hand off to `arm-master`** |
| 6502 / 65816 / Z80 / 68000 / NES / SNES / Genesis / GB | — | **Hand off to `retro-game-dev`** |
| "Write GPU ISA", shaders | — | **Hand off to `arm-master`** (`gpu-isa-reality.md`) |

## 2. Hard rules

1. **Three-step ladder.** Scalar C → compiler autovec (`-O3 -march=native`) → intrinsics →
   assembly. Each step requires a measurement from the previous step. Do not start at
   assembly.
2. **No speed claim without a number.** "Assembly is faster" is a hypothesis. The public
   record is mixed: AI-generated x86-64 asm beat GCC by 6x on a base64 kernel, lost by 3%
   on SipHash, and modern Clang now beats hand-written asm on secp256k1 scalar
   multiplication. The correct answer is always "measure on this kernel, this CPU, this
   compiler."
3. **ABI discipline.** SysV AMD64: 16-byte stack alignment at `call`, callee-saved
   `rbx rbp r12–r15`, red zone below `rsp` usable only in leaf functions. RISC-V psABI:
   16-byte alignment, `s0–s11` callee-saved. AAPCS64 lives in `arm-master`.
4. **Inline asm is a last resort.** It must carry a C fallback, a complete clobber list,
   and a comment explaining why the compiler could not do it.
5. **Measure end-to-end.** A faster kernel that changes cache behaviour or adds a copy can
   lose the whole loop.
6. **No invented microarchitecture.** If a claim depends on a specific core's port layout or
   uop cache, cite the vendor optimisation manual or mark it `UNVERIFIED`.

## 3. Standard deliverable shape

1. **Baseline** — scalar C, exact command, number.
2. **Compiler baseline** — `-O3 -march=native`, number.
3. **Hypothesis** — what the counter says is the bottleneck.
4. **Change** — intrinsics or asm, minimal.
5. **Measurement** — same harness, median of N.
6. **Regression check** — neighbouring benchmarks.
7. **What would falsify this** — the condition under which the change is not worth it.

## 4. Scripts

- `asm_ladder.sh` — builds scalar C, `-O3 -march=native`, and an asm variant from one
  harness, runs N iterations, prints median/min/max and ratios.
- `bench_harness.c` — a minimal timing harness template using `clock_gettime` with warmup
  and a CSV output format.
