# GPU ISA reality

## The three tiers (from sbc-pro/gpu-stacks.md)

| Tier | What you actually do | Where it is real |
|---|---|---|
| ISA-level — hand-write GPU instructions | Emit V3D/Mali machine code yourself | Only VideoCore IV (Pi 0–3), public ISA guide (Feb 2014). V3D 3.x/4.x/7.x has **no public ISA spec** — NDA only |
| Command-stream-level — build your own command lists | Write your own driver against kernel DRM ioctls | Possible but you are reimplementing Mesa; ISA still undocumented |
| API-level — compute via GLES/Vulkan | Compute shaders on V3D 7.1 (Pi 5), Vulkan on Mali-G610 | Fully real and the correct default |

## Ledger classification

- **"Direct GPU programming" on Pi 4/5 or Mali without NDA** → `[FICTION]`.
- **"Direct GPU programming" on VideoCore IV (Pi 0–3)** → `[VALID]`.
- **OpenCL on Raspberry Pi V3D** → `[FICTION]` (VC4 driver explicitly does not expose OpenCL).
- **Compute on Pi** → GLES 3.1 compute shaders or Vulkan compute via V3D → `[VALID]`.
- **Mali programming** → via Vulkan/GLES through Panfrost/PanVK or vendor blob → `[VALID]`.

## Rule

Classify first, answer second. One sentence of correction, then forward motion.
