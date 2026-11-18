# GPU stacks

## The three tiers of GPU access (classify before answering)

| Tier | What you actually do | Where it is real |
|---|---|---|
| ISA-level — hand-write GPU instructions | Emit V3D/Mali machine code yourself | Only VideoCore IV (Pi 0–3), public ISA guide (Feb 2014). V3D 3.x/4.x/7.x has **no public ISA spec** — NDA only |
| Command-stream-level — build your own command lists | Write your own driver against kernel DRM ioctls | Possible but you are reimplementing Mesa; ISA still undocumented |
| API-level — compute via GLES/Vulkan | Compute shaders on V3D 7.1 (Pi 5), Vulkan on Mali-G610 | Fully real and the correct default |

## Stack selection by board

| Board | GPU | Open stack | Blob stack | Notes |
|---|---|---|---|---|
| Pi 5 | VideoCore VII (V3D 7.1) | Mesa V3D + V3DV (Vulkan) | — | GLES 3.1 + Vulkan 1.2/1.3 |
| Pi 4 | VideoCore VI (V3D 4.2) | Mesa V3D + V3DV | — | GLES 3.1 + Vulkan |
| Pi 0–3 | VideoCore IV (V3D 2.x) | Mesa VC4 | — | GLES 2.0 only; no compute shaders or OpenCL |
| RK3588 | Mali-G610 MP4 | Panfrost + PanVK | vendor libmali | PanVK Vulkan 1.2 conformant Jun 2025; CSF firmware required |

## Rule

"Direct GPU programming" is `[FICTION]` on Pi 4/5 and on Mali without an NDA, and `[VALID]`
only on VideoCore IV. Everything else is API-level compute with an open driver underneath.

## Verification

Run `scripts/soc_probe.sh` and identify the active stack before diagnosing any GPU issue.
