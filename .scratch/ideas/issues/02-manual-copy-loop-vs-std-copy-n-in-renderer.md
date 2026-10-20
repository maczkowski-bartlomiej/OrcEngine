# Manual Copy Loop vs std::copy_n in Renderer

Status: resolved
Type: grilling

## Raw Proposal

in renderer.cpp shouldnt we stick with the manual copy in loop instead calling std::copy_n for better code quat.quality

## Context

- **Source:** Standalone (`OrcEngine/source/Orc/Graphics/Renderer.cpp`)
- **Captured:** 2026-10-15
- **Resolved:** 2026-10-20

## Resolution

- Replaced the two-phase `std::copy_n` + secondary mutation loop with a single-pass loop in `submitQuad` and `draw(const Text&)`.
- Eliminated redundant double memory writes across cache lines, unified texture index stamping in one step, and simplified debugging.
- Refactored `submitQuad` to accept `const Ref<Texture>&` to eliminate stale texture slot indices on batch capacity flushes.
