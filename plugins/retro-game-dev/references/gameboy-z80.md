# Game Boy / GBC (LR35902 / PPU)

## CPU
- Sharp LR35902 (Z80-like, no Z80 instructions like IX/IY, some 8080-like) @ ~4.19 MHz.
- Registers: A, F, B, C, D, E, H, L; AF, BC, DE, HL pairs.

## PPU / video limits (hard rule #3)

| Limit | Value |
|---|---|
| Sprites total | 40 |
| Sprites per scanline | 10 |
| Shades per palette | 4 (4-color grayscale) |
| Resolution | 160×144 |
| VRAM | 8 KiB, access restricted during mode 3 |

## MBC (mandatory first fact)

- No MBC = 32 KiB ROM + 8 KiB RAM fixed.
- MBC1/MBC2/MBC3/MBC5 for banking above 32 KiB ROM / 8 KiB RAM.
- Name the MBC before writing code (hard rule #1).

## Timing

- Frame: ~70,224 cycles (~59.7 Hz).
- Vblank: ~4,560 cycles for VRAM/OAM updates.
- VRAM access restricted during mode 3 (active scanline drawing).

## Toolchain

- RGBDS is the standard GB/GBC assembler.

## Common failure

- Writing VRAM during mode 3 → garbled tiles on real hardware.
- Exceeding 10 sprites/line → hardware drop.
