# Provenance attestation — g8t-plugins release 0.1.0

## Human-review attestation

| Field | Value |
|---|---|
| humanReviewed | true |
| llmAssisted | true |
| reviewer | 8b-is/reviewers |
| reviewCommit | UNVERIFIED |
| date | 2026-11-18 |

All `SKILL.md` prose in `plugins/*` is LLM-assisted. Every epistemic claim in each
plugin's `references/epistemic-ledger.md` is classified `[VALID]`, `[PROTOTYPE]`, or
`[FICTION]`. Scripts execute on the user's machine; they have been reviewed for
supply-chain safety (no `npx`, no IPFS gateways, no mutable npm tags).

## Codeberg posture

Metadata mirror only (Mode A). See `MIRROR.md`. No `plugins/*/SKILL.md` is ever pushed
to Codeberg per Codeberg ToU §2(1)7.

## Release verification

- Registry `index.json` is signed with minisign (offline key) and cosign (keyless OIDC).
- Each plugin tarball has a matching `.minisig` and `.sigstore.json`.
- `st plugin verify` recomputes BLAKE3, checks both signatures, and cross-references
  the ledger hash against `references/epistemic-ledger.md`.

## Key rotation

Public keys live at `https://g8t.is/keys/keys.json` with `keys.json.minisig`. Rotation
publishes the new key signed by the old key.
