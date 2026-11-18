#!/usr/bin/env python3
"""g8t.py — minimal sovereign CLI surface for g8t-plugins.

In the smart-tree re-base this becomes `st plugin`; this file is the standalone
entry point used before `st plugin` lands, and as a reference implementation.
"""
import argparse
import hashlib
import json
import sys
from pathlib import Path

INDEX_URL = "https://g8t.is/registry/index.json"


def blake3_file(path: Path) -> str:
    import blake3
    h = blake3.blake3()
    h.update(path.read_bytes())
    return "b3:" + h.hexdigest()


def verify_plugin(name: str, index: dict) -> int:
    for p in index.get("plugins", []):
        if p["name"] == name:
            print(f"[verify] {name} {p['version']}: ledgerHash={p.get('ledgerHash')}")
            print("[verify] OK (placeholder; real verify checks BLAKE3 + minisig + cosign)")
            return 0
    print(f"[verify] {name}: not found in registry index")
    return 1


def main() -> int:
    ap = argparse.ArgumentParser(prog="g8t")
    sub = ap.add_subparsers(dest="cmd")

    p_add = sub.add_parser("add", help="add a plugin")
    p_add.add_argument("name")
    p_add.add_argument("--verify", action="store_true")

    p_list = sub.add_parser("list", help="list plugins in registry")
    p_verify = sub.add_parser("verify", help="verify a plugin")
    p_verify.add_argument("name")

    args = ap.parse_args()

    if args.cmd == "add":
        print(f"[add] {args.name} (placeholder; install via st plugin add)")
        return 0
    if args.cmd == "list":
        print("[list] esp32-pro, sbc-pro, arm-master")
        return 0
    if args.cmd == "verify":
        index = {"plugins": []}  # placeholder: fetch from INDEX_URL in real use
        return verify_plugin(args.name, index)

    ap.print_help()
    return 1


if __name__ == "__main__":
    sys.exit(main())
