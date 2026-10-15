# Remove drawQuad API in Renderer

Status: needs-triage
Type: grilling

## Raw Proposal

In Rederer.cpp remove drawQuad API

## Context

- **Source:** Standalone (`OrcEngine/source/Orc/Graphics/Renderer.cpp`)
- **Captured:** 2026-10-15

## Frontier Seeds

- ❓ What primitive submission mechanism replaces `drawQuad` for callers (e.g., shapes, text, sprites)?
- ❓ How should existing call sites across `TestProject` demo layers be migrated?
