# Emulation and debug

## Emulator ≠ hardware (hard rule #4)

A ROM that passes in an emulator may fail on real hardware due to:
- timing differences,
- open bus behaviour,
- undocumented opcodes (varies by chip revision),
- PPU/VDP edge cases.

## Mesen 2

- Covers NES, SNES, GB, GBA, PCE, SMS/GG, WS.
- MCP server forks exist.
- State which accuracy level the emulator claims; what remains unverified goes in the
  "Hardware verification" section of the deliverable.

## GDB stubs

- Mesen-GDB exposes a GDB stub for source-level debugging.
- Use for cycle-accurate stepping, not as proof of hardware compatibility.

## Verification shape

1. **Emulator verification** — which emulator, which accuracy level.
2. **Hardware verification** — what remains untested on real silicon.

If you cannot test on real hardware, mark the unverified behaviour `UNVERIFIED` and give
the test.
