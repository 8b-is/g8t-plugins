#!/usr/bin/env bash
# rom_probe.sh — read an iNES/NES/SFC/GB ROM header and print mapper, PRG/CHR sizes,
# MBC type, and header checksum where applicable.
# POSIX shell only. No Python.
set -u

FILE="${1:?usage: rom_probe.sh <rom>}"

if [ ! -f "$FILE" ]; then
    echo "not found: $FILE" >&2
    exit 1
fi

SIZE=$(stat -c '%s' "$FILE" 2>/dev/null || stat -f '%z' "$FILE")
echo "== $FILE =="
echo "size: $SIZE bytes"

# Detect by magic
MAGIC=$(head -c 4 "$FILE" | od -An -tx1 | tr -d ' \n')
echo "magic: $MAGIC"

case "$MAGIC" in
    4e45531a)
        echo "format: iNES (NES)"
        PRG_ROMS=$(( $(od -An -tu1 -j4 -N1 "$FILE" | tr -d ' ') ))
        CHR_ROMS=$(( $(od -An -tu1 -j5 -N1 "$FILE" | tr -d ' ') ))
        FLAGS6=$(od -An -tu1 -j6 -N1 "$FILE" | tr -d ' ')
        FLAGS7=$(od -An -tu1 -j7 -N1 "$FILE" | tr -d ' ')
        MAPPER=$(( ((FLAGS7 >> 4) & 0xF) | ((FLAGS6 >> 4) & 0xF) ))
        echo "PRG ROM units: $PRG_ROMS (x16 KiB = $((PRG_ROMS*16)) KiB)"
        echo "CHR ROM units: $CHR_ROMS (x8 KiB = $((CHR_ROMS*8)) KiB)"
        echo "mapper: $MAPPER"
        echo "mirroring: $(( FLAGS6 & 1 )) (0=horizontal, 1=vertical)"
        echo "battery-backed PRG: $(( (FLAGS6 >> 1) & 1 ))"
        echo "trainer: $(( (FLAGS6 >> 2) & 1 ))"
        ;;
    4742)
        echo "format: Game Boy"
        TITLE=$(head -c 16 -s "$FILE" | tail -c 15 | tr -d '\0')
        echo "title: $TITLE"
        MBC_CODE=$(od -An -tu1 -j0x147 -N1 "$FILE" | tr -d ' ')
        echo "MBC byte: $MBC_CODE"
        case "$MBC_CODE" in
            0) echo "MBC: none (32K ROM fixed)" ;;
            1) echo "MBC: MBC1" ;;
            2) echo "MBC: MBC2" ;;
            3) echo "MBC: MBC3" ;;
            5) echo "MBC: MBC5" ;;
            *) echo "MBC: unknown/code $MBC_CODE" ;;
        esac
        ;;
    *)
        echo "format: unknown or raw binary"
        ;;
esac
