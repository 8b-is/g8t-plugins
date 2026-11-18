# ENGRAM — retro-game-dev

Format: `KEY :: claim :: status :: source/date`

## Hardware limits (the medium)
- NES_SPRITES :: 64 total, 8 per scanline, 3 colors + transparency each :: VALID :: PPU
- NES_PALETTE :: 4 background palettes of 4 colors, 4 sprite palettes of 4 :: VALID :: PPU
- NES_SPRITE0 :: sprite-0 hit is the scanline IRQ primitive for splits/parallax :: VALID :: PPU
- SNES_SPRITES :: 128 total, 32 per line, two sprite sizes at a time :: VALID :: PPU
- GENESIS_SPRITES :: 80 total, 20 per line, 320×224 or 256×224 :: VALID :: VDP
- GB_SPRITES :: 40 total, 10 per line, 4 shades per palette :: VALID :: PPU

## Toolchains
- NES_TOOL :: ca65/cc65 is the mainstream assembler; asm6 and NESASM are alternatives :: VALID :: cc65 docs
- SNES_TOOL :: WLA-DX (`wla-65816`) is the multi-platform standard; asar and cc65 also target 65816 :: VALID :: WLA-DX, snes.nesdev.org
- GB_TOOL :: RGBDS is the standard GB/GBC assembler :: VALID :: RGBDS docs
- GENESIS_TOOL :: SGDK is a C kit; raw m68k asm is supported via `.s` files :: VALID :: SGDK docs

## Debugging
- MESEN :: Mesen 2 covers NES, SNES, GB, GBA, PCE, SMS/GG, WS; MCP server forks exist :: VALID :: Mesen2 repos, 2026
- GDB_STUB :: Mesen-GDB exposes a GDB stub for source-level debugging :: VALID :: XRetroCode / Mesen docs

## Open questions
- Q1 :: Target platform and mapper/MBC? :: UNRESOLVED
- Q2 :: Real hardware or emulator-only? Changes the accuracy budget. :: UNRESOLVED
- Q3 :: C or assembly? Platform-dependent. :: UNRESOLVED
