# Power, thermal, safety

## Throttle flags (Pi)

- `vcgencmd get_throttled` — bitmask of under-voltage, frequency cap, throttle, temp limit.
- Read these **before** any performance tuning or benchmarking.
- A throttled SBC makes every number meaningless (hard rule #5).

## Thermal zones

- `/sys/class/thermal/thermal_zone*/temp` — millidegrees Celsius.
- Sample at 1 Hz with `scripts/thermal_watch.sh` during any sustained load.

## Power budgeting

- Pi 5: 5A / 25W official supply recommended; undersupply causes under-voltage flag.
- USB current limited by supply; total platform power < supply rating.
- Undervoltage is sticky until cleared; check `vcgencmd get_throttled` after any reboot.

## Safety

- Do not hot-plug GPIO on a powered Pi (3.3V tolerance only; 5V destroys the SoC).
- ESD precautions when handling bare boards.
- Do not stack HATs without checking 3.3V/5V rail budgets.

## Rule

If `vcgencmd get_throttled` reports any throttle bit set, the honest first answer is "fix
power/thermal before optimising," not "here is a faster kernel parameter."
