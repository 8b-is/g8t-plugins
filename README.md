# g8t-plugins

Sovereign, agent-agnostic plugin registry for 8b-is skills. Installs via
`st plugin` (Smart Tree) or directly into `~/.agents/skills/`.

## Plugins

| Plugin | Purpose | Status |
|---|---|---|
| `esp32-pro` | ESP32 / ESP32-S3 embedded firmware, C99, embedded Rust, RISC-V/XTENSA assembly, zero-DC shutter drive | `0.1.0` |
| `sbc-pro` | Linux SBC engineering (Pi 5/4/Zero/CM, RK3588, VideoCore, Panfrost/PanVK, boot chain, thermal) | `0.1.0` |
| `arm-master` | AArch64/ARMv8-A/ARMv9 low-level perf: assembly, NEON/ASIMD, SDOT/FP16, AAPCS64, perf counters | `0.1.0` |

## Install

```bash
# Generic agent-agnostic skills folder
npx skills add https://g8t.is/registry/esp32-pro

# Via Smart Tree (recommended)
st plugin add esp32-pro

# Claude Code
claude plugin marketplace add 8b-is/g8t-plugins
claude plugin install esp32-pro

# Sovereign CLI (verifies signatures + ledger before install)
g8t plugin add esp32-pro --verify
```

## Cross-skill handoff

- `sbc-pro` → `arm-master`: instruction selection, NEON, ABI, cache behaviour, GPU-ISA reality.
- `arm-master` → `sbc-pro`: board-level facts (peripheral addresses, firmware mediation, driver availability).

## Trust chain

`st plugin add` runs three gates before installation:

1. **Registry verification** — BLAKE3 against the signed `registry/index.json`.
2. **Security scan** — plugin artifact passed through smart-tree's `foreign_patterns`
   matcher (no `npx`, no IPFS gateways, no mutable npm tags).
3. **Ledger hash check** — `references/epistemic-ledger.md` hash must match the signed manifest.

Only then does `AiInstaller` write `~/.agents/skills/<name>/` and register MCP/hooks.

## Mirrors

- GitHub: `https://github.com/8b-is/g8t-plugins` (canonical forge)
- Codeberg: `https://codeberg.org/8b-is/g8t-plugins` (metadata only, see `MIRROR.md`)
- Forgejo: `https://v13.next.forgejo.org/8b-is/g8t-plugins` (full sovereign mirror)

## License

MIT
