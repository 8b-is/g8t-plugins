# ISA routing

## Where each ISA lives

| ISA | Owner | Reference |
|---|---|---|
| x86-64 (SysV AMD64) | assembly-pro | `references/x86-64.md` |
| RISC-V (psABI, RVV) | assembly-pro | `references/riscv.md` |
| AArch64 / NEON / Cortex-A76 | arm-master | `arm-master/references/aarch64-abi.md`, `neon-sdot-fp16.md` |
| 6502 / 65816 / Z80 / 68000 | retro-game-dev | platform-specific files |
| GPU ISA (VideoCore IV, V3D 3.x–7.x, Mali) | arm-master | `arm-master/references/gpu-isa-reality.md` |

## Routing rule

If the question asks about AArch64, NEON, FP16, SDOT, or AAPCS64 — stop and hand off to
`arm-master`. If it asks about 6502/Z80/65816/68000 — stop and hand off to
`retro-game-dev`. assembly-pro owns x86-64 and RISC-V only.

## Cross-skill handoff

- assembly-pro → arm-master: AArch64 specifics, GPU ISA reality.
- assembly-pro → retro-game-dev: vintage ISA specifics.
- retro-game-dev → assembly-pro: general asm discipline / the ladder for a vintage platform
  when the user asks "is hand-asm worth it vs C on this platform?"
