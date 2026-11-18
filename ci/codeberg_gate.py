#!/usr/bin/env python3
"""codeberg_gate.py — CI gate for Codeberg metadata-only mirror.

Scans the files that would be pushed to Codeberg and fails if any path under
plugins/ would be included, or if any LLM-heavy code file is detected in the
mirror payload. g8t-plugins uses Mode A (metadata mirror only); Codeberg ToU
§2(1)7 forbids projects mostly consisting of LLM-generated code.
"""
import sys
from pathlib import Path

MIRROR_ALLOWED = {"schemas", "cli", "ci", "registry"}
MIRROR_ALLOWED_FILES = {"MIRROR.md", "PROVENANCE.md"}


def main() -> int:
    root = Path.cwd()
    problems = []

    # 1. Nothing under plugins/ may be in the mirror payload
    for p in root.rglob("plugins/*"):
        if p.is_file():
            problems.append(f"LLM-heavy plugin file in mirror payload: {p}")

    # 2. Mirror-allowed directories only
    for top in [p for p in root.iterdir() if p.is_dir() and p.name not in MIRROR_ALLOWED]:
        if top.name == "plugins":
            continue
        for f in top.rglob("*"):
            if f.is_file():
                problems.append(f"non-allowed top-level dir in mirror: {f}")

    if problems:
        print("CODEBERG_GATE_FAIL: mirror would include disallowed content")
        for p in problems:
            print(f"  - {p}")
        return 1

    print("CODEBERG_GATE_OK: mirror payload is metadata-only")
    return 0


if __name__ == "__main__":
    sys.exit(main())
