# Issue 01: Collapse the 2D Render Submission Pipeline

Status: resolved

## Description

The 2D rendering pipeline currently fragments batching across 4 separate shape structures (`m_sprites`, `m_circles`, `m_rectangles`, `m_glyphs`) inside `Renderer`, each duplicating vertex and texture slot tracking. Furthermore, frame lifecycle (`begin`/`end`/`clear`) leaks between `Engine::run()` and every demo layer in `TestProject`.

## Objective

Consolidate 2D primitive rendering behind a unified `Renderer2D` deep module that:
1. Replaces separate shape batchers with a unified 2D quad batcher.
2. Preserves submission order naturally without relying on artificial z-index inversion hacks.
3. Cleans the frame rendering lifecycle so layers submit primitives without duplicate `begin`/`end` invocations.

See `.scratch/collapse-2d-render-submission-pipeline/spec.md` for the full synthesized specification.

## Comments

- 2026-10-15: Grilling interview completed and synthesized into spec.
- 2026-10-15: GLOSSARY.md updated with domain terms.
- 2026-10-15: ADR 0001 created in docs/adr/0001-collapse-2d-render-submission-pipeline.md.
- 2026-10-15: Fully implemented via Tickets 01-04 on integration branch. Verified on windows-debug and windows-release.
