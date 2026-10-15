# Manual Copy Loop vs std::copy_n in Renderer

Status: needs-triage
Type: grilling

## Raw Proposal

in renderer.cpp shouldnt we stick with the manual copy in loop instead calling std::copy_n for better code quat.quality

## Context

- **Source:** Standalone (`OrcEngine/source/Orc/Graphics/Renderer.cpp`)
- **Captured:** 2026-10-15

## Frontier Seeds

- ❓ What code quality, readability, or debugging trade-offs exist between an explicit vertex-by-vertex assignment loop versus `std::copy_n` / `std::memcpy`?
- ❓ Does `std::copy_n` affect optimization, vectorization, or trivial copy guarantees for vertex types compared to a manual loop?
