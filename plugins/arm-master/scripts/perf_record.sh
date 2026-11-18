#!/usr/bin/env bash
# perf_record.sh — wrap perf stat with the counter set that matters for A76-class cores.
# cycles, instructions, IPC, L1d misses, branch misses, stalled cycles.
# No optimisation without a counter.
set -u

BIN="${1:?usage: perf_record.sh <binary> [args...]}"
shift

COUNTERS="cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,branch-misses"

echo "[perf_record] counters: $COUNTERS"
echo "[perf_record] target: $BIN $*"
echo

perf stat -e "$COUNTERS" "$BIN" "$@"

echo
echo "[perf_record] IPC = instructions / cycles (compute from the two lines above)"
echo "[perf_record] Interpret: IPC<0.5 => memory/stall bound; IPC 1.0-1.5 => scalar; IPC>2 => vectorised"
