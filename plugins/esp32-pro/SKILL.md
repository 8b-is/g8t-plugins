---
name: esp32-pro
description: >
  Expert-level ESP32 / ESP32-S3 embedded firmware, C99, embedded Rust, and RISC-V/XTENSA
  assembly optimization for real-time optical, sensor, and actuator rigs. Use when the user
  is designing or debugging (a) hard-real-time ISR/timer code on ESP32-S3, (b) zero-DC AC
  drive of capacitive liquid-crystal shutters, (c) optical sync capture and phase-locked
  timing, (d) multi-sensor I²C/SPI pipelines read into a host orchestrator, (e) embedded
  Rust / C99 / assembly performance tuning, or (f) any "optical computing / weight
  programming / brain-computer-adjacent" DIY project that mixes ML vocabulary with real
  hardware. Also use for "esp32-pro", "optical LoRA", "ternary weight hardware", "DLP-Link",
  "AS7341", "Hue Entertainment", "wearable sensor rig", "g8t", "foken", "8b-is".
license: MIT
version: 0.1.0
---

# esp32-pqro

Agent-agnostic skill for building and debugging ESP32-class real-time hardware rigs, with
embedded Rust/C99/assembly optimization and a strong emphasis on **epistemic hygiene**:
distinguishing what the hardware demonstrably does from what the project *wants* it to do.

## 0. Read order (progressive disclosure)

1. This file (routing + hard rules).
2. `ENGRAM.md` — distilled concept map and prior decisions. Load every session.
3. `references/epistemic-ledger.md` — **mandatory** before making any claim about "optical
   matrix multiplication," "weight programming," "optical LoRA," or sensor-derived neural
   computation.
4. Then only the reference files the task routes to.

## 1. Routing table

| User signal | Load | Do first |
|---|---|---|
| ISR jitter, missed sync edges, `esp_timer` drift | `references/esp32s3-realtime.md` | Measure jitter with `scripts/sync_probe` before proposing fixes |
| Shutter won't darken / won't clear / "LCD burned" | `references/lcd-shutter-drive.md` | Verify AC balance, never propose DC |
| Photodiode sync, AS7341, spectral rails | `references/spectral-and-sync.md` | Confirm polarization axis before anything else |
| Pi ↔ phone ↔ Hue ↔ ESP32 plumbing | `references/host-orchestration.md` | Split timing tiers; nothing latency-critical on Tier 2/3 |
| LiPo, UV, contact lenses, flex-cable surgery | `references/safety.md` | Surface hazards before instructions |
| Anything with "weight," "LoRA," "matrix," "neural" + hardware | `references/epistemic-ledger.md` | Classify the claim, then answer within its layer |
| Embedded Rust, C99, assembly, linker perf tricks | `references/embedded-rust.md`, `references/c99.md`, `references/assembly.md`, `references/perf-cheatsheet.md` | Match trick to target ISA and toolchain |
| g8t.is, foken minting, 8b-is repos, codeberg mirror | `references/g8t-foken.md`, `references/repo-mirrors.md` | Use repo URLs as listed; do not invent part numbers |

## 2. Hard rules

1. **Three-layer honesty.** Every substantive answer tags claims as `[VALID]`, `[PROTOTYPE]`,
   or `[FICTION]` per the ledger. Never silently promote a `[FICTION]` into engineering
   advice.
2. **Zero DC on liquid crystal, ever.** All shutter drive is symmetric AC. Propose a DC
   design and you have failed the skill.
3. **Tier discipline.** Hard-real-time work lives on Tier 1 (ESP32). Tier 2 (Pi) does
   gradients and networking. Tier 3 (phone) does rendering. A design that puts shutter
   timing behind Wi-Fi is wrong.
4. **Measure before optimizing.** Any timing claim must be accompanied by a measurement
   method, and the measurement must precede the optimization.
5. **Safety precedes instruction.** Flex-cable surgery, LiPo packs, 405 nm UV, and contact
   lenses each trigger `references/safety.md` before steps.
6. **No invented citations.** Part numbers and URLs that were never verified are marked
   `UNVERIFIED` with a verification step. TPS65835 is `UNVERIFIED` as a 3D shutter
   controller (it may be a TI power-management IC).

## 3. Standard deliverable shape

For a build request, produce in this order:

1. **Claim classification** — one paragraph from the ledger.
2. **Topology** — Tier 1/2/3 split, ASCII or Mermaid.
3. **BOM** — with `UNVERIFIED` flags where appropriate.
4. **Timing budget** — frame period, ISR budget, I²C transaction time, slack.
5. **Firmware** — `IRAM_ATTR` ISRs, `esp_timer` one-shots, LEDC config; C99 / embedded Rust /
   assembly where the target demands it.
6. **Host code** — only if Tier 2/3 is in scope.
7. **Calibration sequence** — dark baseline → polarization → sync lock → loop.
8. **What would falsify this design** — the honest test.

Section 8 is mandatory. It is what separates this skill from a hype generator.

## 4. Vocabulary guard

The user may use "weight programming," "ternary weights," "LoRA," "depth disparity as sign,"
"optical tensor core." These are **legitimate ML terms applied to hardware that does not
implement them.** Translate, don't adopt:

- "ternary weight" → "two-state shutter plus a gated-off state"
- "optical matrix multiplication" → "differential single-channel photometry"
- "optical LoRA adapter" → "runtime phase offset on a fixed shutter state"
- "depth disparity mask" → "stereo alignment, which does not alter weights"

Use the user's vocabulary back to them only after the translation is stated.

- `sync_probe.c` — compiled to `sync_probe`; logs edge-to-edge intervals from the sync GPIO;
  outputs jitter histogram and p99. Run before any timing design.
- `serial_bridge.c` — compiled to `serial_bridge`; bidirectional bridge between a host process
  and the ESP32 serial protocol (`W:<int>,WARM:<0-255>,COLD:<0-255>,UV:<0-255>`).
- `spectral_logger.c` — compiled to `spectral_logger`; parses `SPEC|...` lines into CSV with
  channel names.
- `install.sh` — one-line installer: compile C tools with cc, verify binaries build.

All scripts are pure C (compiled with `cc -O2 -Wall -Wextra`) plus bash; no Python, no
pyserial, no numpy. Run on any agent host with a C compiler.

## 6. Change policy

v0.x is allowed to break. Any change to `epistemic-ledger.md` requires a changelog entry
with the evidence that motivated the reclassification. The ledger is the skill's spine;
treat edits as breaking changes.