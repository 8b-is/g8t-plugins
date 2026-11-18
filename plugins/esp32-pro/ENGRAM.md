# ENGRAM — esp32-pqro persistent memory

Compact, load-every-session artifact. One line per durable fact or decision.
Format: `KEY :: claim :: status :: source`

## Hardware
- S3_TIMER :: esp_timer one-shot resolution is µs-scale; suitable for T/2 phase scheduling :: VALID :: esp-idf docs
- S3_ISR :: ISR handlers must be IRAM_ATTR and avoid flash access :: VALID :: esp-idf docs
- LEDC_20K :: 20 kHz LEDC PWM is above audible range and flicker-free for LEDs :: VALID :: practice
- TN_DRIVE :: normally-white TN cell is transparent at 0 V, opaque under symmetric AC :: VALID :: LCD physics
- TN_AC :: DC bias causes electrochemical degradation of the alignment layer :: VALID :: LCD physics
- AS7341 :: 11-channel spectral sensor, I²C, ~2.8 ms integration at ATIME=10/ASTEP=99 :: VALID :: datasheet

## Topology decisions
- TIER_SPLIT :: Tier1 ESP32 timing, Tier2 Pi gradients/networking, Tier3 phone rendering :: DECIDED :: v0.1 design
- SERIAL_PROTO :: `W:<int>,WARM:<int>,COLD:<int>,UV:<int>\n` :: DECIDED :: v0.1 design
- HUE_PATH :: Hue Entertainment DTLS UDP :2100, ~25–50 Hz, macro-bias only :: VALID :: Hue API docs

## Claim ledger (short form)
- OPTICAL_MAC :: single shutter pair does not perform matrix multiplication :: FICTION :: ledger
- OPTICAL_LORA :: phase offset changes shutter state, not model weights :: FICTION :: ledger
- FOREST_SLM :: canopy dappling is a real light modulator; "computes dot products against your weights" is not :: MIXED :: ledger
- CONTACTS_DEPTH :: dichoptic color can induce chromostereopsis in a human observer; it does not improve machine sensing :: MIXED :: ledger

## Embedded Rust / C99 / assembly
- EMBEDDED_RUST :: no_std + esp-hal on ESP32-S3; RISC-V core :: VALID :: esp-hal docs
- C99_IDF :: ESP-IDF components compiled as C99, -ffreestanding for bare metal :: VALID :: esp-idf
- XTENSA_ASM :: ESP32 (not S3) is Xtensa; S3 is RISC-V :: VALID :: espressif docs

## g8t / foken / 8b-is
- G8T_IS :: g8t.is — Wild linker perf tricks, foken minting, leaderboard :: UNVERIFIED :: blog post 2025-09-02
- FOKEN_MINTING :: foken tokens minted on g8t.is leaderboard :: UNVERIFIED :: g8t.is
- REPO_MIRRORS :: github.com/8b-is + codeberg mirror :: UNVERIFIED :: mirror sync status unknown

## Open questions (carry forward)
- Q1 :: Is the stock DLP-Link ASIC's sync output accessible without desoldering? :: UNRESOLVED
- Q2 :: Actual contrast ratio of the user's specific glasses model :: UNRESOLVED
- Q3 :: Whether Tier-1 should accept SPI SLM input or stay serial-only :: UNRESOLVED
- Q4 :: Is TPS65835 a 3D shutter controller or a TI PMIC? :: UNVERIFIED

## User context
- GOAL :: state-of-the-art, agent-agnostic skill + engram; v0.1 scaffold for skill-creator iteration
- STYLE :: accepts speculation but wants the real/fiction boundary explicit
- NEXT :: run skill-creator plugin, add eval set, test triggering against other frontier models