# Remove drawQuad API in Renderer

Status: resolved
Type: grilling

## Raw Proposal

In Rederer.cpp remove drawQuad API

## Context

- **Source:** Standalone (`OrcEngine/source/Orc/Graphics/Renderer.cpp`)
- **Captured:** 2026-10-15
- **Resolved:** 2026-10-20

## Resolution

- Confirmed zero callers existed across `OrcEngine` and `TestProject`.
- Removed all four `drawQuad` overloads from `Renderer.hpp` and `Renderer.cpp`.
- Removed dead `begin()` and `end()` lifecycle aliases from `Renderer.hpp` in favor of canonical `beginScene()` and `endScene()`.
- Callers submit 2D primitives via high-level shapes (`Sprite`, `Rectangle`, `Circle`, `Text`) and `drawLine()`.
