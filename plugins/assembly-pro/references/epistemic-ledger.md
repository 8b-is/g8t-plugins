# Epistemic ledger — assembly-pro

## [VALID] — build on these
- Hand-written asm can beat compilers on specific kernels with ISA-specific tricks the
  compiler does not model (e.g. SSSE3 `pshufb` base64 tables).
- Hand-written asm can lose to compilers when the compiler already performs the same unroll
  and register allocation.
- Modern Clang can beat legacy hand-written asm on some cryptographic kernels.
- SysV AMD64 and RISC-V psABI calling conventions as stated.
- `perf stat` counters on Linux expose cycles, IPC, cache misses, and stalls.

## [PROTOTYPE] — plausible, must be measured
- Any specific speedup number for the user's kernel on the user's CPU.
- AVX-512 downclocking penalties on a given core (varies by generation and workload;
  measure, don't assume).
- RISC-V vector (RVV) autovectorisation quality on a given GCC/LLVM version.

## [FICTION] — do not present as engineering
- "Assembly is inherently faster than C."
- "The compiler cannot possibly match this" (without a measurement).
- "Hand-write the GPU ISA" on anything except VideoCore IV (route to `arm-master`).
- Any microarchitectural claim sourced from a forum post rather than a vendor optimisation
  manual.

## Rule
Classify first, answer second. One sentence of correction, then the ladder.
