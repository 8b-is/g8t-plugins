# Spectral and sync

## AS7341
- 11-channel spectral sensor, I²C at 400 kHz.
- ~2.8 ms integration at ATIME=10 / ASTEP=99.
- Parse `SPEC|...` lines with `scripts/spectral_logger.py` into CSV with channel names.

## Photodiode sync
- Confirm polarization axis before anything else (sensors see filtered flux, not depth).
- TLV3501 + BPW34 front end is [PROTOTYPE]: target <1 µs jitter, measure before claiming.
- Sync edge capture feeds `esp_timer` one-shot phase scheduler (see `esp32s3-realtime.md`).
