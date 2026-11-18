# Toolchains

## NES
- **ca65/cc65** — mainstream assembler/C compiler suite.
- **asm6** — simple, fast assembler.
- **NESASM** — bank-based, older.

## SNES
- **WLA-DX** (`wla-65816`) — multi-platform standard.
- **asar** — SA-1 / LoROM/HiROM focused.
- **cc65** — also targets 65816.

## Game Boy / GBC
- **RGBDS** — `rgbasm`, `rgblink`, `rgbfix`; standard GB/GBC toolchain.

## Sega Genesis
- **SGDK** — C kit with sample code.
- **raw m68k asm** — `.s` files assembled with the SGDK toolchain.

## Decision rule

Toolchain is a decision, not a default (hard rule #5). Name the choice and why, tied to
the platform's memory map and banking scheme.

## Linker config

Every ROM above the base address space needs banking:
- NES: mapper determines PRG/CHR banking; linker segments must match the mapper.
- GB: MBC determines ROM/RAM banking; `rgblink -l` linker script defines banks.

## Common failure

- "The toolchain handles the mapper for you" — FICTION; the linker config must match the
  mapper/MBC.
