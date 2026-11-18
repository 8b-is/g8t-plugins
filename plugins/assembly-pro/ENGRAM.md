# ENGRAM — assembly-pro

Format: `KEY :: claim :: status :: source/date`

## Performance reality
- ASM_NOT_INHERENTLY_FAST :: hand-written asm can be slower than compiler output on modern OoO cores :: VALID :: public benchmarks, 2025–2026
- BASE64_WIN :: SSSE3 `pshufb` table-free base64 decode beat GCC by 4.8–6.3x in a published kernel :: VALID :: benchmark, 2026
- SIPHASH_LOSS :: same benchmark, GCC won by ~3% on Redis SipHash :: VALID :: benchmark, 2026
- CLANG_WINS :: modern Clang scalar multiplication on secp256k1 beat the hand-asm path by ~20–25% lower latency :: VALID :: secp256k1 PR, 2026
- LADDER :: C → `-O3 -march=native` → intrinsics → asm, counter-gated :: DECIDED :: v0.1

## ABI
- SYSV_AMD64 :: 16-byte alignment at `call`; red zone usable in leaf functions only; `rbx rbp r12–r15` callee-saved :: VALID
- RISCV_PSABI :: 16-byte alignment; `s0–s11` callee-saved; `a0–a7` args/returns :: VALID
- AARCH64 :: routed to arm-master :: DECIDED

## Open questions
- Q1 :: Target CPU and compiler version? Changes every answer. :: UNRESOLVED
- Q2 :: Is the hot path actually hot, or is this premature? :: UNRESOLVED
- Q3 :: Does the user need portability, or is a single-microarchitecture tune acceptable? :: UNRESOLVED
