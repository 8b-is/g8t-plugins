# Sega Genesis (68000 / VDP)

## CPU
- Motorola 68000 @ ~7.67 MHz.
- Z80 co-processor for sound.

## VDP / video limits (hard rule #3)

| Limit | Value |
|---|---|
| Sprites total | 80 |
| Sprites per scanline | 20 |
| Resolution | 320×224 or 256×224 |
| Colors | 64 master palette, 4 palettes of 16 |

## Toolchain

- SGDK is a C kit; raw m68k asm supported via `.s` files.
- Is hand-asm worth it? On 68000 with SGDK, often no — C is competitive; on 6502/Z80,
  often yes. State the platform before invoking the `assembly-pro` ladder.

## Bank switching

- Cartridge banking via mapper registers; name the mapper before coding.

## Common failure

- Exceeding 20 sprites/line → hardware drop.
- Z80 sound driver bugs are a classic Genesis failure mode.
