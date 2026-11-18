# ARM64 instruction cheatsheet

## AAPCS64 register map (quick)
- x0–x7 : args / results (not preserved)
- x8    : indirect result (not preserved)
- x9–x15: temporaries (not preserved)
- x16–x17: IP0/IP1 (not preserved)
- x18   : platform reserved (do not use)
- x19–x28: callee-saved (must preserve)
- x29   : frame pointer (callee-saved)
- x30   : link register (callee-saved)
- SP    : 16-byte aligned at public interfaces, no red zone

## NEON / ASIMD (v0–v31, 128-bit)
- v0–v7  : args / results (not preserved)
- v8–v15 : callee-saved (preserve low 64 bits)
- v16–v31: temporaries (not preserved)

## Useful A64 instructions
- `mrs x0, pmccntr_el0`   — cycle counter (user-accessible if enabled)
- `mrs x0, cntpct_el0`    — physical counter
- `udot v0.4s, v1.16b, v2.16b` — unsigned dot product (INT8)
- `sdot v0.4s, v1.16b, v2.16b` — signed dot product
- `fmls v0.4s, v1.4s, v2.4s`   — fused multiply-subtract (FP32)
- `umov w0, v0.b[0]`     — extract lane
- `fmla v0.8h, v1.8h, v2.8h`   — FP16 multiply-add

## AAPCS64 inline asm (GCC/Clang)
```c
__asm__ volatile("mrs %0, pmccntr_el0" : "=r"(cyc) : : "memory");
```
Always declare clobbers and `"memory"`; always ship a C fallback.

## Rule
No speed claim without a measurement. `perf stat` first; asm last.
