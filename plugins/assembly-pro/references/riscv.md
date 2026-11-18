# RISC-V (psABI)

## Register roles

| Register | ABI name | Role | Callee-saved? |
|---|---|---|---|
| x0 | zero | hardwired 0 | N/A |
| x1 | ra | return address | No |
| x2 | sp | stack pointer | No | 16-byte aligned at call
| x3 | gp | global pointer | No |
| x4 | tp | thread pointer | No |
| x5–x7 | t0–t2 | temporaries | No |
| x8 | a0 | argument / return value | No |
| x9 | a1 | argument | No |
| x10–x17 | a2–a7 | arguments | No |
| x18 | s0 | saved register | Yes |
| x19 | s1 | saved register | Yes |
| x20–x27 | s2–s11 | saved registers | Yes |
| x28–x31 | t3–t6 | temporaries | No |

## ABI rules

- **Stack:** 16-byte aligned at every call boundary.
- **Return:** `a0` (and `a1` for 128-bit).
- **Varargs:** follow the same register stack; spilled args on the stack.
- **`t0–t6`, `a0–a7`, `ra`:** caller-saved.
- **`s0–s11`:** callee-saved; must be saved/restored if used.

## Prologue template

```asm
    addi    sp, sp, -N            ; frame, keep 16-byte aligned
    sw      ra, N-4(sp)           ; save return address
    sw      s0, N-8(sp)           ; save callee-saved if used
    mv      s0, sp                ; optional frame pointer
    ; ... body ...
    lw      ra, N-4(sp)
    lw      s0, N-8(sp)
    addi    sp, sp, N
    ret
```

## RISC-V Vector (RVV)

- RVV is a separate ISA extension (`-march=rv64imac_zvl512b` etc.).
- Compiler autovectorisation of RVV is immature on GCC/LLVM as of 2025–2026.
- Hand RVV only after the scalar baseline and `-O3 -march=native` baseline both exist
  and are measured.
