# SoC support matrix

## Version-anchored entries

Each row is dated. A claim without a kernel + firmware + Mesa version is incomplete.

| SoC | Board examples | Kernel | GPU driver | Mesa | GPU stack | Status | Date |
|---|---|---|---|---|---|---|---|
| BCM2712 | Pi 5, CM5 | Pi OS 6.6 / mainline | vc4 / V3D | 24.x+ | V3D + V3DV (GLES 3.1, Vulkan 1.2/1.3) | VALID | 2025 |
| BCM2711 | Pi 4, CM4 | Pi OS 6.6 / mainline | vc4 / V3D | 23.x+ | V3D + V3DV (GLES 3.1, Vulkan 1.2) | VALID | 2025 |
| BCM2835 | Pi 0/1 | Pi OS legacy | vc4 | legacy | GLES 2.0 only | VALID | 2024 |
| BCM2837 | Pi 2/3 | Pi OS | vc4 | legacy | GLES 2.0 only | VALID | 2024 |
| RK3588 | Banana Pi M5, Orange Pi 5, Radxa Rock 5 | vendor BSP / Collabora mainline | Panfrost / vendor libmali | 24.x+ | Panfrost + PanVK (Vulkan 1.2) | PROTOTYPE | Jun 2025 |

## Mainline vs BSP

- **Mainline**: upstream Linux, Panfrost/PanVK, DTS from device tree. Peripheral addresses stable across kernel versions.
- **BSP**: vendor kernel, vendor Mali blob, vendor DTS. Peripheral addresses may differ from mainline documentation.

Never mix instructions across the two (hard rule #2).

## Known gaps (UNVERIFIED)

- RK3588 DDR training on mainline — board-specific, verify per vendor.
- Pi 5 RP1 PCIe peripheral map under mainline 6.12+ — RPi patches pending.
