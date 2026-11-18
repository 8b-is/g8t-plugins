# Epistemic ledger

Every claim in the source corpus is classified. Agents MUST tag answers.

## [VALID] — engineering facts, safe to build on
- Zero-DC symmetric AC drive of TN LCD cells.
- esp_timer one-shot scheduling at µs resolution; IRAM_ATTR ISRs.
- LEDC PWM at ≥20 kHz for flicker-free LED drive.
- AS7341 spectral readout over I²C at 400 kHz.
- Hue Entertainment DTLS 1.2 stream on UDP :2100.
- Ternary quantization, LoRA, straight-through estimator as ML techniques.
- Chromostereopsis exists in human vision.
- embedded Rust: no_std + esp-hal targets ESP32-S3 (RISC-V) and ESP32 (Xtensa).
- C99 is the baseline for ESP-IDF components; -ffreestanding for bare metal.
- RISC-V rv32imc on ESP32-S3; Xtensa LX6/LX7 on ESP32.

## [PROTOTYPE] — plausible, must be measured before claiming
- TLV3501 + BPW34 sync front end at <1 µs jitter on a hobby build.
- Differential TIA (MCP6004) reading behind-lens flux.
- Stereo disparity as a spatial mask on a real rig.
- Stride-locked update loop with IMU commit windows.
- 5-fan azimuth panning via sin/cos duty weighting.
- g8t.is "Wild performance tricks" (linker-level, mutable slicing, atomic/non-atomic Vec reuse, buffer reuse) — real techniques from davidlattimore.github.io 2025-09-02 post; measure per-target.
- foken minting on g8t.is leaderboard — real mechanism on that site; unverified as a general-purpose token system.

## [FICTION] — do not present as engineering
- The glasses perform optical matrix multiplication.
- Depth disparity reprograms weights.
- A forest canopy functions as an optical computer for ternary weights.
- Dichoptic color lenses "enhance depth" for sensors (they enhance depth for the human observer; sensors see filtered flux, not depth).
- "Optical LoRA adapter" as a real adapter on a base model.

## Rule
If the user's framing is [FICTION], the answer states the fiction once,
plainly, then redirects to what the hardware can actually be built to do.
No lectures. One sentence, then forward motion.

## UNVERIFIED part numbers / URLs
- TPS65835 — could not confirm it is a 3D shutter controller; may be a TI power-management IC. Verify against a datasheet before using in a BOM.
- g8t.is foken minting API endpoints — unverified; do not cite as stable.
- github.com/8b-is + codeberg mirror — repos exist; mirror sync status unknown.