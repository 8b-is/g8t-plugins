# st plugin integration — g8t-plugins

This document wires `g8t-plugins` into smart-tree (`st`) as the plugin runtime:
`st` is the installer, `m8` is the plugin database, `AiInstaller` is the plugin
installer, and `foreign_patterns` is the plugin firewall.

## st plugin subcommand surface

```bash
st plugin add esp32-pro                 # fetch index → verify → scan → MEM8 → AiInstaller
st plugin add esp32-pro --target universal
st plugin list                          # reads MEM8 plugin registry
st plugin verify esp32-pro              # BLAKE3 + minisig + cosign + ledger hash
st plugin ledger esp32-pro              # renders [VALID]/[PROTOTYPE]/[FICTION]
st plugin publish                       # sign, hash, append to index, tag
st plugin remove esp32-pro --clean-hooks
st plugin mirror status                 # github / codeberg / forgejo divergence
```

Three gates run before `AiInstaller` installs anything:

1. **Registry verification** — BLAKE3 against the signed `registry/index.json`.
2. **Security scan** — plugin artifact through smart-tree's `foreign_patterns`
   matcher (no `npx`, no IPFS gateways, no mutable npm tags).
3. **Ledger hash check** — `references/epistemic-ledger.md` hash must match the
   signed manifest's `ledgerHash`.

Only after all three gates does `AiInstaller` write `~/.agents/skills/<name>/`
and register MCP/hooks.

## MEM8 section types for plugin capsules

| Section | Content | Smart-tree analogue |
|---|---|---|
| `PLUG` | plugin name, version, BLAKE3, registry URL | existing WAVE header |
| `EPIS` | epistemic ledger, tagged claim records | new — the 8b.is differentiator |
| `HOOK` | hook commands installed for this plugin | mirrors `hooks.json` |
| `MCPX` | MCP server entries contributed by the plugin | mirrors `.mcp.json` |
| `PROV` | human-review attestation, reviewer, review commit | new |
| `SESS` | last verify timestamp, last security-scan result | extends `.aye_consciousness.m8` |

`st plugin verify` becomes a single MEM8 validation pass (`m8 validate` checks magic,
section bounds, whole-file IEEE CRC32) plus signature verification. A tampered ledger
section fails at section-CRC before any crypto.

## Cross-skill handoff: sbc-pro ↔ arm-master

| Direction | Trigger | What moves |
|---|---|---|
| sbc-pro → arm-master | instruction selection, NEON, ABI, cache behaviour, GPU-ISA reality | the low-level perf question |
| arm-master → sbc-pro | peripheral address, firmware mediation, driver availability, kernel/GPU stack | the board-level fact |

Each SKILL.md section 4 names the other skill by name. When a question depends on both,
the answer must state the split explicitly: "board-level fact → sbc-pro; instruction
question → arm-master."

## Adapter generation (client targets)

| Client | Adapter path | Consumed format |
|---|---|---|
| Claude Code | `com.anthropic.claude-code/.claude-plugin/marketplace.json` | Claude plugin marketplace |
| Codex | `com.openai.codex/AGENTS.md` + `skills/` | `AGENTS.md` + skills CLI |
| Gemini CLI | `com.google.gemini/gemini-extension.json` | Gemini extension manifest |
| Cursor | `com.cursor/.cursor/rules/<name>.mdc` | Cursor rules |
| Generic | `~/.agents/skills/<name>/` | Agent Skills folder |

One `st plugin add` regenerates all five via `AiTarget::{Claude,Chatgpt,Gemini,Universal}`.

## Codeberg posture (Mode A)

Metadata mirror only: `schemas/`, `cli/`, `ci/`, `registry/`, `MIRROR.md`,
`PROVENANCE.md`. No `plugins/*/SKILL.md` ever pushed to Codeberg per ToU §2(1)7.
Full sovereign copy on Forgejo (`v13.next.forgejo.org/8b-is/g8t-plugins`).
