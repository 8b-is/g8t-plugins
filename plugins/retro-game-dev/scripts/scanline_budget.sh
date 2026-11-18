#!/usr/bin/env bash
# scanline_budget.sh — compute per-frame cycle budget and per-scanline sprite budget
# for the target platform.
# Usage: scanline_budget.sh <platform> [fps]
# POSIX shell only. No Python.
set -u

PLATFORM="${1:?usage: scanline_budget.sh <platform> [fps]}"
FPS="${2:-60}"

case "$PLATFORM" in
    nes)
        CPU_HZ=1789773
        CYCLES_PER_FRAME=$(( CPU_HZ / FPS ))
        VBANK_CYCLES=2273
        SPRITES_TOTAL=64
        SPRITES_PER_LINE=8
        ;;
    snes)
        CPU_HZ=3579545
        CYCLES_PER_FRAME=$(( CPU_HZ / FPS ))
        VBANK_CYCLES=0
        SPRITES_TOTAL=128
        SPRITES_PER_LINE=32
        ;;
    genesis)
        CPU_HZ=7670454
        CYCLES_PER_FRAME=$(( CPU_HZ / FPS ))
        VBANK_CYCLES=0
        SPRITES_TOTAL=80
        SPRITES_PER_LINE=20
        ;;
    gb)
        CPU_HZ=4194304
        CYCLES_PER_FRAME=$(( CPU_HZ / FPS ))
        VBANK_CYCLES=4560
        SPRITES_TOTAL=40
        SPRITES_PER_LINE=10
        ;;
    *)
        echo "unknown platform: $PLATFORM (nes|snes|genesis|gb)" >&2
        exit 1
        ;;
esac

echo "== $PLATFORM budget @ ${FPS}fps =="
echo "CPU Hz: $CPU_HZ"
echo "Cycles per frame: $CYCLES_PER_FRAME"
echo "Vblank cycles: $VBANK_CYCLES"
echo "Sprites total: $SPRITES_TOTAL"
echo "Sprites per scanline: $SPRITES_PER_LINE"

if [ "$VBANK_CYCLES" -gt 0 ]; then
    echo "Vblank budget fraction: $(( VBANK_CYCLES * 100 / CYCLES_PER_FRAME ))% of frame"
fi
echo "Sprites-per-line fraction of total: $(( SPRITES_PER_LINE * 100 / SPRITES_TOTAL ))%"
echo ""
echo "Note: these are hardware limits (the medium). Budget in cycles and scanlines,"
echo "not milliseconds. Exceeding sprites-per-scanline causes hardware drop/flicker."
