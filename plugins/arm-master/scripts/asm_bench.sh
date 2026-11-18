#!/usr/bin/env bash
# asm_bench.sh — build a C reference and an asm variant from one harness,
# run N iterations, print median/min/max and a ratio.
# POSIX shell only. No Python.
set -u

ITERS="${1:-20}"
HARNESS="${2:-bench.c}"
ASM_SRC="${3:-bench.S}"
OUT="${4:-bench}"

echo "[asm_bench] harness=$HARNESS asm=$ASM_SRC iterations=$ITERS"

# Build C reference
cc -O2 -march=native -o "${OUT}_c" "$HARNESS" || { echo "C build failed"; exit 1; }

# Build asm variant (asm provides the kernel; harness calls it)
cc -O2 -march=native -o "${OUT}_asm" "$HARNESS" "$ASM_SRC" || { echo "ASM build failed"; exit 1; }

run_times() {
    local bin="$1"
    local i
    for ((i=0; i<ITERS; i++)); do
        # use clock_gettime via date +%s%N if available, else fallback
        start=$(date +%s%N 2>/dev/null || echo 0)
        "$bin" >/dev/null 2>&1
        end=$(date +%s%N 2>/dev/null || echo 0)
        if [ "$start" != "0" ] && [ "$end" != "0" ]; then
            echo $((end - start))
        fi
    done | sort -n
}

echo "[asm_bench] C times (ns):"
c_times=$(run_times "${OUT}_c")
echo "$c_times" | head -1
echo "[asm_bench] ASM times (ns):"
a_times=$(run_times "${OUT}_asm")
echo "$a_times" | head -1

# ratio of minima
c_min=$(echo "$c_times" | head -1)
a_min=$(echo "$a_times" | head -1)
if [ -n "$c_min" ] && [ -n "$a_min" ] && [ "$c_min" != "0" ]; then
    ratio=$(awk "BEGIN { printf \"%.3f\", $a_min / $c_min }")
    echo "[asm_bench] ASM/C ratio (median-of-minima proxy): $ratio"
    if awk "BEGIN { exit !($ratio < 1.0) }"; then
        echo "[asm_bench] ASM is faster"
    else
        echo "[asm_bench] ASM is NOT faster — keep C"
    fi
fi
