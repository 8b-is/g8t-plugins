# ESP32-S3 real-time

## Timer / ISR
- `esp_timer` one-shot: µs resolution, suitable for T/2 phase scheduling on TN shutters.
- ISR handlers: `IRAM_ATTR`, must not access flash (PSRAM/flash reads can fault when cache disabled).
- Sync edge capture: GPIO ISR records `esp_timer_get_time()` into a lock-free ring; histogram + p99 computed by `scripts/sync_probe.py`.

## Measurement method (mandatory before any timing fix)
1. Run `sync_probe.py` against the sync GPIO.
2. Record edge-to-edge interval histogram; note p99 and max jitter.
3. Propose a fix only after the baseline number exists.
4. Re-run `sync_probe.py` after the fix; delta must be ≥ the jitter floor.

## Typical budget (frame = 16.67 ms @60 Hz)
| Item | Budget |
|---|---|
| Frame period | 16.67 ms |
| ISR entry + edge timestamp | <5 µs |
| I²C transaction (AS7341, 400 kHz) | ~2.8 ms integration + overhead |
| LEDC update | <100 µs |
| Slack for host comms | remaining |
