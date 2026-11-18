#!/usr/bin/env bash
# install.sh — pure-bash installer for the esp32-pqro skill environment. No Python.
#
#   curl -fsSL https://raw.githubusercontent.com/peterlodri-sec/esp32-pqro/main/scripts/install.sh | bash
#
# Compiles the C scripts (sync_probe, serial_bridge, spectral_logger) with cc,
# optionally clones the 8b-is constellation, and verifies the binaries build.
set -euo pipefail

SKILL_DIR="${HOME}/.agents/skills/esp32-pqro"
CLONE_REPO="${CLONE_REPO:-}"   # set to github.com/8b-is/8b-is-engine to clone
CC="${CC:-cc}"

echo "[install] esp32-pqro skill installer (C, no Python)"

# 1. Compiler check
if ! command -v "${CC}" >/dev/null 2>&1; then
    echo "[install] ${CC} not found; install a C compiler first"
    exit 1
fi
echo "[install] cc: $(command -v ${CC})"

# 2. Compile C scripts
cd "${SKILL_DIR}/scripts"
for s in sync_probe.c serial_bridge.c spectral_logger.c; do
    bin="${s%.c}"
    echo "[install] compiling ${s} -> ${bin}"
    "${CC}" -O2 -Wall -Wextra -o "${bin}" "${s}" || {
        echo "[install] FAILED to compile ${s}"
        exit 1
    }
done

# 3. Optional: clone 8b-is constellation
if [ -n "$CLONE_REPO" ]; then
    DEST="${HOME}/workspace/${CLONE_REPO##*/}"
    if [ ! -d "$DEST" ]; then
        echo "[install] cloning https://github.com/${CLONE_REPO} -> ${DEST}"
        git clone "https://github.com/${CLONE_REPO}" "$DEST" || echo "[install] clone failed (network?), continuing"
    else
        echo "[install] ${DEST} already exists, skipping clone"
    fi
fi

echo "[install] done. Binaries in ${SKILL_DIR}/scripts/: sync_probe serial_bridge spectral_logger"
echo "[install] Load the skill: ESP32-PQRO (references in ${SKILL_DIR}/references)"
