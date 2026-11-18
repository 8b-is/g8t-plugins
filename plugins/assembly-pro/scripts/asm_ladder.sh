#!/usr/bin/env bash
# asm_ladder.sh — build scalar C, -O3 -march=native, and an asm variant from one harness,
# run N iterations, print median/min/max and ratios.
# No Python. POSIX shell only.
set -u

ITERS="${1:-20}"
HARNESS="${2:-bench.c}"
ASM_SRC="${3:-bench.S}"
OUT="${4:-ladder}"

echo "[asm_ladder] harness=$HARNESS asm=$ASM_SRC iterations=$ITERS"

# Build: scalar C
cc -O0 -o "${OUT}_O0" "$HARNESS" || { echo "scalar build failed"; exit 1; }
# Build: compiler autovec
cc -O3 -march=native -o "${OUT}_O3" "$HARNESS" || { echo "O3 build failed"; exit 1; }
# Build: asm variant (asm provides the kernel; harness calls it)
cc -O2 -march=native -o "${OUT}_asm" "$HARNESS" "$ASM_SRC" || { echo "ASM build failed"; exit 1; }

run_times() {
    local bin="$1"
    local i
    for ((i=0; i<ITERS; i++)); do
        start=$(date +%s%N 2>/dev/null || echo 0)
        "$bin" >/dev/null 2>&1
        end=$(date +%s%N 2>/dev/null || echo 0)
        if [ "$start" != "0" ] && [ "$end" != "0" ]; then
            echo $((end - start))
        fi
    done | sort -n
}

echo "[asm_ladder] O0 times (ns):"
t_O0=$(run_times "${OUT}_O0")
echo "$t_O0" | head -1
echo "[asm_ladder] O3 times (ns):"
t_O3=$(run_times "${OUT}_O3")
echo "$t_O3" | head -1
echo "[asm_ladder] ASM times (ns):"
t_asm=$(run_times "${OUT}_asm")
echo "$t_asm" | head -1

o0_min=$(echo "$t_O0" | head -1)
o3_min=$(echo "$t_O3" | head -1)
asm_min=$(echo "$t_asm" | head -1)

echo "[asm_ladder] ratios (ASM relative):"
if [ -n "$asm_min" ] && [ "$o3_min" != "0" ]; then
    awk "BEGIN { printf \"  ASM/O3: %.3f  (lower is faster)\n\", $asm_min / $o3_min }"
fi
if [ -n "$asm_min" ] && [ "$o0_min" != "0" ]; then
    awk "BEGIN { printf \"  ASM/O0: %.3f\n\", $asm_min / $o0_min }"
fi

if [ -n "$asm_min" ] && [ -n "$o3_min" ] && [ "$o3_min" != "0" ]; then
    if awk "BEGIN { exit !($asm_min < $o3_min) }"; then
        echo "[asm_ladder] ASM beats -O3 -march=native"
    else
        echo "[asm_ladder] ASM does NOT beat -O3 -march=native — keep C"
    fi
fi
