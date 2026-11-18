# LCD shutter drive

## Zero-DC rule (hard rule #2)
All shutter drive is symmetric AC. A normally-white TN cell is transparent at 0 V,
opaque under symmetric AC. DC bias causes electrochemical degradation of the alignment
layer. Never propose a DC drive.

## LEDC config (20 kHz)
- Frequency: 20 kHz (above audible, flicker-free for LEDs).
- Duty: 0–255, mapped to WARM/COLD/UV channels.
- Drive: symmetric AC pair per shutter (phase-shifted 180°), net DC ≈ 0 over a full cycle.

## Failure modes
- "LCD burned" → DC bias present; check AC balance on both electrodes.
- Won't darken → verify polarization axis and that AC amplitude exceeds threshold.
- Won't clear → verify 0 V state is actually applied during clear phase.

## Verification
- Scope both electrodes; confirm waveform is symmetric about 0 V over T/2.
- Confirm contrast ratio (Q2 in ENGRAM.md) before claiming performance.
