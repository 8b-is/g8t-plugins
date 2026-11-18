# Embedded Rust

## Targets
- ESP32-S3: RISC-V rv32imc, `esp-hal`, `no_std`.
- ESP32: Xtensa LX6/LX7, `esp-hal`, `no_std`.

## Key crates (verify versions against crates.io before pinning)
- `esp-hal` — HAL for ESP32/ESP32-S3.
- `esp-printlog`, `esp-backtrace` — logging/panic.
- `embedded-hal` — GPIO, I²C, SPI traits.

## Patterns
- `no_main` + `#[entry]` entry point.
- Peripherals taken once: `let p = Peripherals::take().unwrap();`
- ISRs: `#[ram]` / interrupt attributes; avoid `.flash()` access in ISR.
- `esp_timer` one-shots via `esp_hal::timer::Timer`.

## Anti-patterns
- Allocating in the ISR/tick loop (no_std + no alloc by default).
- Blocking I²C/SPI in an async context without `spawn`.
