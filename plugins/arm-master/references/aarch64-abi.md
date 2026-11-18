# AArch64 ABI (AAPCS64)

## Register roles

| Register | Role | Callee-saved? | Notes |
|---|---|---|---|
| x0–x7 | argument / result registers | No | x0 = return value |
| x8 | indirect result location | No | |
| x9–x15 | temporary registers | No | x15 = scratch |
| x16–x17 | IP0 / IP1 (intra-procedure-call) | No | |
| x18 | platform register | Reserved | Do not use in user code |
| x19–x28 | callee-saved | Yes | Must be preserved |
| x29 | frame pointer (FP) | Yes | |
| x30 | link register (LR) | Yes | |
| SP | stack pointer | — | 16-byte aligned at public interfaces |

## Floating-point / NEON registers

| Register | Role | Callee-saved? |
|---|---|---|
| v0–v7 | argument / result | No | lower 64 bits preserved? no — v0–v7 not preserved |
| v8–v15 | callee-saved | Yes | low 64 bits of each must be preserved |
| v16–v31 | temporary | No | |

## Rules

- **No red zone.** AAPCS64 does not define a red zone. Do not write below SP.
- **SP 16-byte aligned** at all public interfaces (function entry/exit).
- **x18 reserved/platform.** Never use x18 for general purposes in user code.
- **Inline asm:** declare clobbers and `"memory"` where relevant; always ship a C fallback.
- **Variadic:** AAPCS64 has fixed rules for variadic args; x0–x7 still used, then stack.

## Inline asm template (GCC/Clang)

```c
static inline uint64_t rdcycle(void) {
    uint64_t cyc;
    __asm__ volatile("mrs %0, pmccntr_el0" : "=r"(cyc) : : "memory");
    return cyc;
}
```

## Stack frame

16-byte aligned. If you need 32-byte alignment for NEON loads, adjust SP manually and
realign before calls.
