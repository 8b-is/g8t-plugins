# ENGRAM — sbc-pro

Format: `KEY :: claim :: status :: source/date`

## Platform facts
- PI5_SOC :: BCM2712, 4x Cortex-A76, VideoCore VII (V3D 7.1) :: VALID :: 2023+
- PI5_RP1 :: peripherals sit behind RP1 southbridge over PCIe 2.0 x4; peripheral map differs from Pi 4 :: VALID :: RPi docs
- PI_BOOT :: VideoCore firmware runs first; ARM released after; mailbox is an RPC to a closed blob; no ARM-side HDMI/DSI PHY :: VALID :: SoC architecture
- PI_MAILBOX :: property-tag channel 8; 16-byte-aligned buffer; cache clean/invalidate required; 0xC0000000 uncached alias for VideoCore :: VALID :: driver sources
- V3D_PI5 :: Mesa V3D/V3DV, conformant GLES 3.1 + Vulkan 1.2/1.3 depending on Mesa :: VALID :: 2025
- V3D_PI4 :: V3D 4.2, same Mesa stack :: VALID :: Mesa docs
- VC4_PI3 :: V3D 2.x, GLES 2.0; kernel driver explicitly does not expose compute shaders or OpenCL :: VALID :: kernel drm/vc4 docs
- VC4_SPEC :: VideoCore IV 3D Architecture Reference Guide publicly released Feb 2014 (111 pp.) :: VALID :: Broadcom
- V3D_NDA :: no public ISA spec for V3D 3.x/4.x/7.x; NDA with Broadcom/RPi required :: VALID :: Mesa docs
- MALI_G610 :: RK3588, Mali-G610 MP4; vendor blob exposes GLES 3.2 / OpenCL 2.2 / Vulkan 1.1 :: VALID :: Banana Pi specs
- PANVK :: Vulkan 1.2 conformant on Mali-G610 as of Jun 2025; CSF firmware required, sourced from Rockchip libmali :: VALID :: Khronos, 2025-06

## Decisions
- STACK_RULE :: always name Mesa vs blob vs NDA before any GPU claim :: DECIDED :: v0.1
- HANDOFF :: sbc-pro handles board/OS; arm-master handles instruction/ISA :: DECIDED :: v0.1
- PROBE_FIRST :: no advice without soc_probe.sh version block :: DECIDED :: v0.1

## Open questions
- Q1 :: Does the user's Pi 5 run Pi OS kernel or mainline? Changes every peripheral answer. :: UNRESOLVED
- Q2 :: Which RK3588 board, and is it on vendor BSP or Collabora mainline? :: UNRESOLVED
- Q3 :: Is GPU compute actually on the critical path, or is this premature optimisation? :: UNRESOLVED
