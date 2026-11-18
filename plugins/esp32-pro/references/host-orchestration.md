# Host orchestration

## Tier split (hard rule #3)
- Tier 1 (ESP32): hard-real-time timing, ISR, shutter drive, sensor readout.
- Tier 2 (Raspberry Pi): gradients, networking, host bridge.
- Tier 3 (phone): rendering only.

Nothing latency-critical on Tier 2/3. Shutter timing must not live behind Wi-Fi.

## Serial protocol
`W:<int>,WARM:<0-255>,COLD:<0-255>,UV:<0-255>\n`

Bridge host ↔ ESP32 with `scripts/serial_bridge.py`.

## Hue Entertainment
- DTLS 1.2 stream on UDP :2100.
- ~25–50 Hz, macro-bias only — not frame-synchronized to shutter edges.
