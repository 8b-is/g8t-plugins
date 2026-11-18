# Assembly

## ISAs
- ESP32-S3: RISC-V rv32imc (32-bit, integer + multiply/divide + compressed).
- ESP32: Xtensa LX6/LX7 (32-bit, variable-length, Windowed registers).

## When to use
- Only when C/Rust cannot meet the timing budget (measured via `sync_probe.py`).
- Cycle-counted inner loops, SIMD-ish DSP, context-switch assembly.

## Rules
- Comment every instruction with cycle cost and purpose.
- Preserve callee-saved registers per ABI.
- Do not hand-write assembly for control flow the compiler emits identically.
- Verify with `objdump -d` after any change.

## Tricks (borrowed from Wild linker / g8t.is perf blog)
- Mutable slicing to split a buffer into non-overlapping mutable shards (Rust `split_off_mut` equivalent via pointer arithmetic in C).
- Atomic ↔ non-arbitrary in-place Vec reuse via transmute of identical-layout types (Rust-specific; in C use `memcpy`/union type-punning with `-fno-strict-aliasing` if needed).
- Buffer reuse: allocate once, `clear()` inside the loop.
