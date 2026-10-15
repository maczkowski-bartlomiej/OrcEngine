# Issue 01: Collapse the 2D Render Submission Pipeline

Status: needs-triage

## Description

The 2D rendering pipeline currently fragments batching across 4 separate shape structures (`m_sprites`, `m_circles`, `m_rectangles`, `m_glyphs`) inside `Renderer`, each duplicating vertex and texture slot tracking. Furthermore, frame lifecycle (`begin`/`end`/`clear`) leaks between `Engine::run()` and every demo layer in `TestProject`.

## Objective

Consolidate 2D primitive rendering behind a unified `Renderer2D` deep module that:
1. Replaces separate shape batchers with a unified 2D quad batcher.
2. Preserves submission order naturally without relying on artificial z-index inversion hacks.
3. Cleans the frame rendering lifecycle so layers submit primitives without duplicate `begin`/`end` invocations.

See `.scratch/architecture-review/ideas.md#candidate-1-collapse-the-2d-render-submission-pipeline` for detailed analysis and grilling questions.

## Comments
