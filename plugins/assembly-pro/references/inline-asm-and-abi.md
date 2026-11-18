# Inline asm and ABI

## Inline asm is a last resort

Hard rule #4: inline asm must carry
1. a C fallback,
2. a complete clobber list,
3. a comment explaining why the compiler could not do it.

## GCC/Clang extended asm template (x86-64)

```c
static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi) : : "memory");
    return ((uint64_t)hi << 32) | lo;
}
```

- `"=a"(lo)`, `"=d"(hi)`: output constraints (eax, edx).
- `:` empty input list.
- `:` `"memory"` clobber: tells the compiler memory may have changed.
- `volatile`: prevents the asm from being deleted or reordered across it.

## Common mistakes

- **Missing clobbers:** if the asm modifies a register the compiler allocated, silent
  miscompilation.
- **No `"memory"` when memory is touched:** the compiler may cache a load across the asm.
- **Assuming `rsp` red zone in non-leaf functions:** undefined behaviour.
- **No C fallback:** the asm path must be conditionally compiled or wrapped so a
  non-asm build still works.

## RISC-V inline asm

```c
static inline uint64_t rdcycle(void) {
    uint64_t cyc;
    __asm__ volatile("rdcycle %0" : "=r"(cyc) : : );
    return cyc;
}
```

The constraint letters differ per ISA (`r` = general register on both). Check the vendor
manual.
