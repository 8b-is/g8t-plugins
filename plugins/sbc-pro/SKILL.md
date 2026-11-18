---
name: sbc-pro
description: >
  Expert-level Linux SBC engineering for Raspberry Pi (Pi 5 / Pi 4 / Zero 2 W / CM4 / CM5),
  Banana Pi, Orange Pi, Radxa Rock, and other BCM2712 / RK3588 / RK3588S boards. Use when
  the user is debugging or designing (a) boot chain and VideoCore/U-Boot firmware behaviour,
  (b) kernel driver or device-tree issues, (c) GPU stack selection (Mesa V3D/V3DV vs
  Panfrost/PanVK vs vendor blob), (d) mainline-vs-BSP divergence, (e) real-time scheduling,
  GPIO/I2C/SPI/PCIe peripherals, or (f) thermal/power throttling on an SBC. Also use for
  "raspberry pi", "banana pi", "rk3588", "videocore", "panfrost", "panvk", "mesa on pi",
  "sbc benchmark", "why is my pi throttling".
license: MIT
version: 0.1.0
---

# sbc-pro

Board-level and OS-level skill for ARM single-board computers. The central discipline is
**version-anchored advice**: SBC support changes monthly, so no recommendation is valid
without the exact SoC, kernel, firmware, and Mesa versions in hand.

## 0. Read order

1. This file.
2. `ENGRAM.md` — durable decisions and known-good configurations.
3. `references/epistemic-ledger.md` — **mandatory** before any GPU or "direct hardware access"
   claim.
4. Only the reference file the task routes to.

## 1. Routing table

| User signal | Load | Do first |
|---|---|---|
| "GPU not working", Vulkan/GLES failure, Panfrost/PanVK | `references/gpu-stacks.md` | Run `scripts/soc_probe.sh`; identify stack (Mesa vs blob) before diagnosing |
| Boot hangs, firmware, `config.txt`, U-Boot, "GPU boots first" | `references/boot-and-mailbox.md` | Establish which stage owns the failure |
| Mainline vs vendor kernel, device tree, missing peripheral | `references/soc-support-matrix.md` | Pin kernel + DTS + firmware versions |
| GPIO/I2C/SPI/PWM/PCIe, RP1 southbridge | `references/realtime-and-io.md` | Note Pi 5 peripherals live behind RP1 over PCIe |
| Throttling, undervoltage, thermal, power budgeting | `references/power-thermal-safety.md` | Read throttle flags before tuning anything |
| "Direct GPU access", "write my own driver", "bare metal HDMI" | `references/epistemic-ledger.md` | Classify the tier, then answer inside it |
| Instruction selection, NEON, ABI, cache behaviour, GPU-ISA reality | `references/...` (hand off to `arm-master`) | See section 4 cross-skill handoff |

## 2. Hard rules

1. **Version gate.** Before advising, obtain SoC model, kernel release, firmware/bootloader
   version, Mesa version, and active GPU driver. If the user has not supplied them, ask for
   `scripts/soc_probe.sh` output. No version-free recommendations.
2. **Mainline vs BSP is a fork in the road, not a detail.** Never mix instructions across
   the two. State which one the answer applies to.
3. **Name the GPU stack.** "The GPU supports X" is meaningless. Say whether X comes from Mesa
   V3D/V3DV, Panfrost/PanVK, or the vendor blob, and on which kernel.
4. **The ARM core is not the boot master on Broadcom parts.** On Pi, VideoCore firmware runs
   first; the mailbox property interface is an RPC to a closed binary, and there is no ARM-side
   HDMI/DSI PHY to program. Do not describe mailbox framebuffer setup as "writing a video
   driver."
5. **Thermal and power precede performance.** An SBC that is throttling makes every benchmark
   meaningless. Check throttle flags first.
6. **Date every support claim.** "RK3588 has Vulkan" is incomplete without "via PanVK, as of
   <month year>" or "via vendor blob."
7. **No invented part numbers or errata.** Mark `UNVERIFIED` and give the datasheet lookup
   step.

## 3. Standard deliverable shape

1. **Version block** — SoC, kernel, firmware, Mesa, driver.
2. **Stack classification** — mainline or BSP; Mesa or blob.
3. **Failure-domain isolation** — boot / kernel / userspace / thermal.
4. **Minimal reproduction** — one command or one script.
5. **Fix, scoped to the stack** — with the version range it applies to.
6. **Regression check** — what to re-run to confirm nothing else broke.
7. **What would falsify this** — the honest test.

## 4. Cross-skill handoff

If the question drops below the OS — instruction selection, NEON, ABI, cache behaviour, or
GPU-ISA reality — hand off to **`arm-master`**. If `arm-master` hits a board-level fact
(peripheral address, firmware mediation, driver availability), it hands back here.

## 5. Scripts

- `soc_probe.sh` — emits a single version block: `/proc/device-tree/model`, `uname -a`,
  `vcgencmd version` (if present), `vulkaninfo --summary`, `glxinfo`/`eglinfo`, Mesa version,
  `dmesg | grep -i -E 'v3d|panfrost|mali'`.
- `gpu_caps.sh` — parses `vulkaninfo`/`eglinfo` into a capability table and flags features
  that require a non-default stack.
- `thermal_watch.sh` — samples `vcgencmd get_throttled`, thermal zones, and clocks at 1 Hz;
  prints the first throttle transition.

All scripts are POSIX shell; no Python.
