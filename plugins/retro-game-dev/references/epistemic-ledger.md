# Epistemic ledger — retro-game-dev

## [VALID] — build on these
- NES, SNES, Genesis, and GB sprite/palette/scanline limits as stated.
- ca65/cc65, WLA-DX, RGBDS, and SGDK as the mainstream toolchains.
- Mesen 2's system coverage.
- Sprite flicker on NES is a consequence of the 8-per-scanline limit, not a bug.
- Bank switching via mapper (NES) or MBC (GB) is mandatory above the base address space.

## [PROTOTYPE] — plausible, must be measured / tested on hardware
- A specific mapper's behaviour under a specific edge case.
- Emulator cycle-accuracy claims beyond the emulator's documented level.
- Open-bus behaviour on a given console revision.
- Undocumented opcode behaviour (varies by chip revision).
- "This routine fits in the vblank budget" without a cycle count.

## [FICTION] — do not present as engineering
- "Modern game design ports directly to 8-bit hardware" (the constraints are the design).
- "The emulator passed, so it works on hardware."
- "You can just add more sprites" on NES/GB.
- "C is always fast enough" on 6502 (it often is not, without hand-tuning).
- "The toolchain handles the mapper for you" without a linker config.

## Rule
Classify first, answer second. One sentence of correction, then the budget.
