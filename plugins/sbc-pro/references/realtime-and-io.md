# Real-time and IO

## Pi 5 peripherals live behind RP1

- RP1 southbridge connects to BCM2712 over PCIe 2.0 x4.
- GPIO/I2C/SPI/PWM addresses differ from Pi 4. Do not copy Pi 4 device-tree or `memmap` values to Pi 5.
- `libgpiod` is the userspace API; `/dev/gpiochip*` numbering follows RP1 layout.

## Real-time scheduling

- `isolcpus` + `cpu-isolated` for deterministic cores.
- `chrt -f 99` for FIFO real-time priority (needs `CAP_SYS_NICE` or root).
- Watch for RT throttling: `/proc/rtl/` and `/proc/sys/kernel/sched_rt_runtime_us`.

## Common peripherals

| Interface | Pi 5 path | Pi 4 path | Notes |
|---|---|---|---|
| I2C | `/dev/i2c-*` via RP1 | `/dev/i2c-*` via bcm2835 | check `i2cdetect` |
| SPI | `/dev/spidev*` via RP1 | `/dev/spidev*` via bcm2835 | |
| PWM | RP1 PWM channels | bcm2835 PWM | |
| PCIe | RP1 is itself a PCIe device | external PCIe on CM4 | |

## Rule

Thermal and power precede performance: an SBC that is throttling makes every IO benchmark
meaningless. Read `references/power-thermal-safety.md` before tuning IO.
