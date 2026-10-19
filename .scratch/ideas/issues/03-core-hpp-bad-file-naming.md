# Core.hpp Bad File Naming

Status: needs-triage
Type: grilling

## Raw Proposal

Core.hpp bad file naming

## Context

- **Source:** Standalone (`OrcEngine/include/Orc/Core/Core.hpp`)
- **Captured:** 2026-10-19

## Frontier Seeds

- ❓ Should `Core.hpp` be renamed to a single more descriptive header, or split into focused headers by concern (e.g., smart pointers, assertions, macros)?
- ❓ What should happen to the `Orc/Core/` directory and its exports in `Orc.hpp`?
