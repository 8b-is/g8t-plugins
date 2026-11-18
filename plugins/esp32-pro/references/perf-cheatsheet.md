# Performance cheatsheet

Distilled from popular web/blog posts (e.g. davidlattimore.github.io "Wild Performance Tricks", 2025-09-02; Wild linker; RustForge 2025 talk). Each trick: **measure before and after.** No trick is universal.

## Buffer reuse (Rust / C)
Allocate outside the hot loop; `clear()` / reset inside. Reuse the heap allocation across iterations rather than reallocating per frame.

## Mutable slicing / sharding
Split one backing buffer into non-overlapping mutable slices so multiple threads (or ISR + main) can write without locks. In Rust: `split_off_mut` + `par_bridge`. In C: pointer arithmetic into a single array with index ranges.

## Atomic ↔ non-atomic in-place conversion
A `Vec<T>` and `Vec<AtomicT>` with identical layout can be reinterpreted without a copy loop (the optimizer elides it). Use when a hot path needs shared random writes but the rest of the code needs plain values. Verify via `objdump` / assembly that the conversion is a no-op.

## Zero-allocation tick loop
Pre-allocate all buffers at init. The tick loop must not call `malloc`/`Vec::new`/`String::new`. Use bump arenas or ring buffers.

## Cache locality
Keep per-object data adjacent (contiguous ID ranges) so a thread's working set stays in cache.

## LUT over branches
Replace small branchy lookups with a 256-entry table indexed by a byte. Validated in embedded demoscene/trick-library contexts; verify cache pressure on your target.

## measure-then-apply
Every perf claim must show `sync_probe.py` numbers (or equivalent) before and after.
