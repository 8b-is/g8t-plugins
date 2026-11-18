---
name: retro-game-dev
description: >
  Retro console game development: NES (6502), SNES (65816), Sega Genesis (68000), Game Boy /
  GBC (Z80/LR35902), GBA (ARM7TDMI). Use when the user is writing assembly or C for a retro
  console, hitting hardware limits (sprites per scanline, palette size, tile budget, bank
  switching, cycle timing), choosing a toolchain (ca65/cc65, RGBDS, WLA-DX, SGDK, asm6), or
  debugging in an emulator (Mesen, GDB stubs, scanline viewers). Also use for "NES dev",
  "SNES dev", "Genesis homebrew", "Game Boy homebrew", "sprite flicker", "scanline timing",
  "mapper", "CHR RAM".
license: MIT
version: 0.1.0
---

# retro-game-dev

Console hardware is the design medium. The central discipline is **constraint-first design**:
the sprite-per-scanline limit, the palette budget, and the cycle budget are not obstacles to
abstract away — they are the vocabulary the game is written in. The second discipline is
**emulator ≠ hardware**: an emulator that runs the ROM is not proof the ROM runs on a
console.

## 0. Read order

1. This file.
2. `ENGRAM.md`.
3. `references/epistemic-ledger.md` — **mandatory** before any hardware-limit or cycle-accuracy
   claim.
4. The routed platform file.

## 1. Routing table

| Platform / signal | Load | Do first |
|---|---|---|
| NES, 6502, PPU, mapper, sprite 0 hit | `references/nes-6502.md` | Establish mapper and scanline budget |
| SNES, 65816, Mode 7, HDMA | `references/snes-65816.md` | Establish mode and DMA budget |
| Genesis, 68000, SGDK, Z80 sound | `references/genesis-68000.md` | Establish SGDK vs raw asm |
| Game Boy, RGBDS, LR35902, MBC | `references/gameboy-z80.md` | Establish MBC and VRAM bank budget |
| Toolchain choice, linker config, build | `references/toolchains.md` | Match toolchain to platform |
| Emulator debugging, GDB, Mesen, cycle counts | `references/emulation-and-debug.md` | Confirm the emulator's accuracy level |

## 2. Hard rules

1. **Name the mapper / MBC / banking scheme before writing code.** A NES ROM without a
   mapper is not a NES game; a GB ROM without an MBC cannot bank. Every platform answer
   starts with the memory map.
2. **Budget in cycles and scanlines, not milliseconds.** "This will be fast enough" is not
   a retro-dev statement. State the per-frame cycle budget and what fraction the routine
   consumes.
3. **Respect the hardware limits as facts.** NES: 64 sprites total, 8 per scanline, 3 colors
   + transparency per sprite, 4 background palettes. SNES: 128 sprites, 32 per line, 16
   colors per palette (with sub-palettes), two sprite sizes at a time. Genesis: 80 sprites,
   20 per line, 320×224 or 256×224. Game Boy: 40 sprites, 10 per line, 4 shades per palette.
   These are the medium.
4. **Emulator ≠ hardware.** A ROM that passes in Mesen may fail on a CRT or a real console
   due to timing, open bus, or undocumented opcodes. State which accuracy level the emulator
   claims and what remains unverified.
5. **Toolchain is a decision, not a default.** ca65 vs asm6 vs NESASM for NES; WLA-DX vs
   asar vs cc65 for SNES; RGBDS for GB; SGDK vs raw m68k for Genesis. Name the choice and
   why.
6. **No invented hardware quirks.** If a behaviour is undocumented or revision-specific, mark
   it `UNVERIFIED` and give the test.

## 3. Standard deliverable shape

1. **Platform + mapper/MBC + toolchain** — the three anchors.
2. **Memory map** — what lives where.
3. **Budget** — cycles/frame, sprites/line, tiles, palettes.
4. **Code** — assembly or C, with the hardware registers named.
5. **Emulator verification** — which emulator, which accuracy level.
6. **Hardware verification** — what remains untested on real silicon.
7. **What would falsify this** — the condition under which the design breaks.

## 4. Cross-skill handoff

Vintage ISA micro-optimisation at the instruction level routes to **`assembly-pro`** for
general asm discipline; platform quirks stay here. If the question is "is hand-asm worth it
vs C on this platform," the answer is often yes for 6502/Z80 and often no for 68000 with
SGDK — state the platform before invoking the ladder.

## 4. Scripts

- `rom_probe.sh` — reads an iNES/NES/SFC/GB ROM header and prints mapper, PRG/CHR sizes,
  MBC type, and header checksum where applicable.
- `scanline_budget.sh` — computes per-frame cycle budget and per-scanline sprite budget for
  the target platform from the ROM/frame params.

All scripts are POSIX shell; no Python.
