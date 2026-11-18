# ISA cheatsheet

## x86-64 (SysV AMD64) register map
- rax : return value / arg
- rdi rsi rdx rcx r8 r9 : args 1–6 (not preserved)
- rbx rbp r12–r15 : callee-saved (must preserve)
- rsp : 16-byte aligned at `call`; red zone below rsp = leaf-only
- r10 r11 : caller-saved scratch

## RISC-V (psABI) register map
- x0 zero : hardwired 0
- x1 ra : return address
- x2 sp : stack pointer, 16-byte aligned
- x5–x7 t0–t2 : temporaries (not preserved)
- x8 a0 / x9 a1 : args / return value
- x10–x17 a2–a7 : args (not preserved)
- x18 s0 / x19 s1 / x20–x27 s2–s11 : callee-saved
- x28–x31 t3–t6 : temporaries

## Inline asm (GCC/Clang)
```c
__asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi) : : "memory");
```
Always declare clobbers and `"memory"`; always ship a C fallback.

## Rule
No speed claim without a measurement. `perf stat` first; asm last.
