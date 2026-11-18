# ENGRAM — arm-master

## Microarchitecture
- A76 :: Cortex-A76 (Pi 5, RK3588 big cores) supports ASIMD/NEON, FP16, RDM, DotProd/SDOT :: VALID
- ASM_RISK :: public FFmpeg/A76 data shows hand-asm can be slower than clang when clang performs the same unroll :: VALID :: 2026
- NEON_GAIN :: realistic NEON speedups on A76 are ~1.2x–1.8x for suitable kernels; ~2.75x reported for a full image-quality pipeline :: PROTOTYPE :: measure per kernel
- AAPCS64 :: x19–x29 callee-saved; SP 16-byte aligned at public interfaces; no red zone; x18 platform-reserved :: VALID

## GPU reality
- GPU_ISA_PI3 :: VideoCore IV ISA is publicly documented; hand-written GPU code is legitimate :: VALID
- GPU_ISA_PI45 :: V3D 3.x/4.x/7.x ISA is NDA-only; "direct GPU programming" reduces to Mesa or NDA :: FICTION (as commonly meant)
- GPU_COMPUTE_PI :: compute on Pi means GLES 3.1 compute shaders or Vulkan compute via V3D; not OpenCL, not ISA :: VALID
- GPU_MALI :: program via Vulkan/GLES through Panfrost/PanVK or vendor blob; OpenCL not supported by Panfrost :: VALID

## Decisions
- LADDER :: C → intrinsics → asm, counter-gated at every step :: DECIDED :: v0.1
- BENCH :: median of N on the same machine, end-to-end level :: DECIDED :: v0.1
