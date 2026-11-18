# Epistemic ledger — ARM/SBC edition

## [VALID] — build on these
- Pi 5 = BCM2712 + RP1 southbridge; VideoCore VII = V3D 7.1.
- Mesa V3D/V3DV: conformant GLES 3.1 and Vulkan on Pi 4/5.
- Pi boots VideoCore first; mailbox is an RPC to firmware; no ARM-side HDMI PHY.
- VideoCore IV (Pi 0–3) ISA is publicly documented (2014).
- V3D 3.x/4.x/7.x ISA is NDA-only.
- VC4 kernel driver does not expose compute shaders or OpenCL.
- Mali-G610: vendor blob Vulkan 1.1 / OpenCL 2.2; PanVK Vulkan 1.2 conformant (Jun 2025).
- A76: NEON, FP16, SDOT; AAPCS64 rules as stated.

## [PROTOTYPE] — plausible, must be measured
- Hand-written asm beating compiler output on A76.
- PanVK being production-grade on a given RK3588 board with mainline kernel.
- Bare-metal framebuffer on Pi 5 (mailbox path exists, but RP1 changes the map).
- Any specific NEON speedup number for the user's kernel.

## [FICTION] — do not present as engineering
- "Direct GPU programming" on Pi 4/5 or Mali without NDA.
- OpenCL on Raspberry Pi's V3D.
- "Bare metal gives me direct HDMI control" on Broadcom SoCs.
- "Assembly is inherently faster."
- Any GPU-ISA claim sourced from a blog post rather than a spec or NDA doc.

## Rule
Classify first, answer second. One sentence of correction, then forward motion.
