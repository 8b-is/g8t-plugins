# Codeberg mirror policy — g8t-plugins

## Mode: A — metadata mirror only

This repo is mirrored to Codeberg (`codeberg.org/8b-is/g8t-plugins`) as a **metadata-only
mirror**. The following paths are pushed:

- `schemas/`
- `cli/`
- `ci/`
- `registry/`
- `MIRROR.md`
- `PROVENANCE.md`

The following paths are **not** pushed:

- `plugins/*/SKILL.md`
- `plugins/*/ENGRAM.md`
- `plugins/*/references/` (any)
- `plugins/*/scripts/` (any)
- `plugins/*/assets/` (any)

## Why

Codeberg's Terms of Use § 2(1)7 forbid projects "mostly consisting of code written by
generative AI tools" and name Claude explicitly. Every `SKILL.md` in this registry is
LLM-assisted. A full `git push --mirror` of `plugins/*` would be the exact shape Codeberg's
policy targets, and the mirror would be removed without warning.

The metadata mirror gives Codeberg users discovery and verification (schema, index, CI,
gate) without hosting the LLM-authored plugin payload. The sovereign copy — the full
repo including every `SKILL.md` — lives on Forgejo at `v13.next.forgejo.org/8b-is/g8t-plugins`.

## Precedent

Smart Tree is already mirrored at `codeberg.org/8b-is/smart-tree` and `v13.next.forgejo.org/8bit-wraith/smart-tree`.
That live precedent informs this policy: if smart-tree's Codeberg mirror survives, the policy
is applied leniently to established repos; if it disappears, Codeberg drops out of g8t-plugins
entirely and Forgejo becomes the only mirror.

## CI gate

`.github/workflows/mirror-codeberg.yml` runs `ci/codeberg_gate.py` before any push. The gate
scans the mirror payload for LLM-heavy content and fails the CI if `plugins/` would be
included. Mode A cannot fail this gate because `plugins/*` is never in the subtree push.

## Review

If you are considering switching to Mode B (full mirror), you must update `PROVENANCE.md`
with a human-review attestation and accept that the mirror is best-effort: it works until a
report lands, then it is gone.
