# esp32-pqro

ESP32 / ESP32-S3 embedded firmware, C99, embedded Rust, and RISC-V/XTENSA assembly
optimization for real-time optical, sensor, and actuator rigs. With an epistemic-hygiene
layer that separates what the hardware demonstrably does from what a project *wants* it to do.

## What this is

A Codewhale skill (`esp32-pqro`) you load when working on:

- Hard-real-time ISR/timer code on ESP32-S3 (`esp_timer` one-shots, `IRAM_ATTR` ISRs)
- Zero-DC AC drive of capacitive liquid-crystal (TN) shutters
- Optical sync capture and phase-locked timing
- Multi-sensor I²C/SPI pipelines into a host orchestrator (Raspberry Pi / phone / Hue bridge)
- Embedded Rust / C99 / assembly performance tuning
- "Optical computing / weight programming / brain-computer-adjacent" DIY hardware that mixes
  ML vocabulary with real hardware

## File tree

```
esp32-pqro/
├── SKILL.md              # routing + hard rules (load first)
├── ENGRAM.md             # persistent memory, load every session
├── README.md             # this file
├── AGENTS.md             # coding-agent installer prompt
├── install.sh            # one-line installer
├── assets/
│   └── topology.txt      # Tier 1/2/3 ASCII topology
├── scripts/
│   ├── sync_probe.c     # sync GPIO jitter histogram + p99
│   ├── serial_bridge.c  # host <-> ESP32 serial bridge
│   └── spectral_logger.c# SPEC|... -> CSV
└── references/
    ├── epistemic-ledger.md       # [VALID] / [PROTOTYPE] / [FICTION] claims
    ├── esp32s3-realtime.md       # timers, ISRs, timing budget
    ├── lcd-shutter-drive.md      # zero-DC AC drive, LEDC 20 kHz
    ├── spectral-and-sync.md      # AS7341, photodiode sync
    ├── host-orchestration.md     # Tier 1/2/3 split, serial protocol, Hue
    ├── safety.md                 # LiPo, UV, flex cable, contact lenses
    ├── embedded-rust.md          # no_std + esp-hal on S3/ESP32
    ├── c99.md                    # C99 baseline, freestanding
    ├── assembly.md               # RISC-V rv32imc / Xtensa
    ├── perf-cheatsheet.md        # distilled web/blog perf tricks
    ├── g8t-foken.md              # g8t.is, foken, 8b-is repos
    └── repo-mirrors.md           # github.com/8b-is + codeberg mirror
```

## Quick start

Load the skill in Codewhale:

```
/skills esp32-pqro
```

Or install via the one-liner:

```bash
curl -fsSL https://raw.githubusercontent.com/peterlodri-sec/esp32-pqro/main/scripts/install.sh | bash
```

## Hard rules (summary)

1. **Three-layer honesty** — tag every claim `[VALID]`, `[PROTOTYPE]`, or `[FICTION]`.
2. **Zero DC on liquid crystal, ever** — symmetric AC only.
3. **Tier discipline** — timing on Tier 1 (ESP32); nothing latency-critical behind Wi-Fi.
4. **Measure before optimizing** — `sync_probe` first.
5. **Safety precedes instruction** — read `references/safety.md` for LiPo/UV/flex-cable/contact-lens work.
6. **No invented citations** — mark unverified part numbers/URLs `UNVERIFIED`.

## Standard deliverable shape

For a build request, produce in this order:

1. Claim classification
2. Topology (Tier 1/2/3)
3. BOM (with `UNVERIFIED` flags)
4. Timing budget (frame period, ISR budget, I²C time, slack)
5. Firmware (`IRAM_ATTR` ISRs, `esp_timer` one-shots, LEDC config)
6. Host code (only if Tier 2/3 in scope)
7. Calibration sequence (dark baseline → polarization → sync lock → loop)
8. **What would falsify this design** (mandatory)

## Scripts

| Script | Purpose |
|---|---|
| `sync_probe.c` (→ `sync_probe`) | Edge-to-edge jitter histogram + p99 from sync GPIO |
| `serial_bridge.c` (→ `serial_bridge`) | Bridge host process ↔ ESP32 `W:<int>,WARM:<0-255>,COLD:<0-255>,UV:<0-255>` |
| `spectral_logger.c` (→ `spectral_logger`) | Parse `SPEC|...` AS7341 lines into CSV |

All scripts are pure C (compile with `cc -O2 -Wall -Wextra`) plus bash; no Python, no pyserial, no numpy.

## License

MIT
