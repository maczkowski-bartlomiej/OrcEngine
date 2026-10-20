# 03: Sync Core Documentation and Invariants

Status: completed
Type: task
Blocked by: 02

**What to build:** Synchronize the engine domain documentation and architectural decision records with the new `Orc/Core/` module structure, fulfilling project invariants and ensuring the verification gate passes 100%.

**Blocked by:** 02 (Migrate Includes and Remove Legacy Engine/Core.hpp)

- [x] `AGENTS.md` is updated with documentation for the `Orc/Core/` domain, describing the responsibilities of `Platform.hpp`, `Memory.hpp`, `Types.hpp`, `Version.hpp`, and `Core.hpp`
- [x] `GLOSSARY.md` is updated to define the Core domain and reflect ubiquitous language
- [x] An Architectural Decision Record is created under `docs/adr/` capturing the context, decision, and consequences of separating foundational core primitives from engine coordination
- [x] The full unified verification gate (`.\scripts\verify.ps1`) passes with zero warnings or errors (conventions linting, compilation, 5-second smoke test)
