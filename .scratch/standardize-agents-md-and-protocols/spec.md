# Standardize AGENTS.md, Merge Architectural Guidance, and Enforce Universal Agent Protocols

Status: resolved
Role: spec

## Problem Statement

OrcEngine currently has two disjoint versions of its agent guidance (`AGENTS.md`):
1. A recent version focused primarily on toolchain prerequisites, git commands, and issue tracker scripts, but lacking deep architectural diagrams, behavioral protocols, and coding conventions.
2. An older, comprehensive version containing invaluable architectural diagrams, lifecycle descriptions, consultation rules, and naming/style tables, but containing obsolete rendering descriptions (separate per-shape vertex arrays and inverted depth z-index hacks) that contradict recent architectural decisions (ADR 0001, ADR 0002).

Furthermore, when autonomous sessions or skills (such as `/implement-spec`) run, agents often overlook `AGENTS.md`, attempt manual CMake commands instead of utilizing the robust `scripts/` toolchain (`build.ps1`, `lint-conventions.ps1`), and fail to update architectural documentation alongside code changes, causing documentation drift.

## Solution

Synthesize a single, authoritative `AGENTS.md` at the repository root that merges the architectural depth, consultation protocols, and style rules from the older version with the script tooling, issue tracking, and dead-code pruning rules of the current version. Update the rendering subsystem section to match the collapsed dynamic quad batcher with unified `Vertex` and submission order. Establish a universal **Mandatory Agent Invariant** at the top of `AGENTS.md` enforcing that every autonomous session must review `AGENTS.md` and `GLOSSARY.md`, use `.\scripts\build.ps1` and `.\scripts\lint-conventions.ps1` as the primary build/check commands, and synchronize architectural documentation and glossary entries as a strict condition of ticket completion.

## User Stories

1. As an AI agent, I want clear, unmissable startup instructions at the top of `AGENTS.md`, so that I never execute tasks without understanding project invariants.
2. As a project owner, I want all agents to use `.\scripts\build.ps1` and `./scripts/build.sh` as the primary build command, so that MSVC/Ninja toolchain detection and environment variables are handled automatically without manual CMake errors.
3. As a project owner, I want all agents to validate conventions with `.\scripts\lint-conventions.ps1`, so that formatting, tab indentation, and `#pragma once` rules are enforced before completion.
4. As a project owner, I want agents to synchronize `GLOSSARY.md`, ADRs, and `AGENTS.md` subsystem sections whenever implementing code changes, so that architectural documentation never drifts out of date.
5. As an engine developer, I want `AGENTS.md` to document the high-level project direction (rendering refactoring -> C# scripting -> release game in separate repo), so that all technical decisions align with long-term goals.
6. As a developer, I want strict consultation principles in `AGENTS.md` (zero unilateral assumptions, propose options first, incremental execution, preserve scope), so that agents never make unconfirmed architectural leaps.
7. As an AI agent, I want complete subsystem architecture documentation with an ASCII lifecycle diagram, so that I understand frame sequencing across `Engine::run()`, `GameLayer`, `Renderer`, `Gui`, and `Audio`.
8. As an AI agent, I want clear documentation of `GameLayer` hooks (`onAttach`, `onDetach`, `onUpdate`, `onRender`, `onGuiRender`, `onEvent`), so that I add layer features in the correct lifecycle phase.
9. As an AI agent, I want clear documentation on the `Event` `std::variant` pattern matching and dispatching (`visit`, `Overload`, `is<T>`, `getIf<T>`), so that input and window handling code follows established patterns.
10. As an AI agent, I want the rendering subsystem documentation in `AGENTS.md` to accurately reflect the unified dynamic quad batcher, unified `Vertex`, and natural submission order, so that I am not misled by obsolete separate batch arrays or depth-buffer hacks.
11. As an AI agent, I want clear documentation on FMOD audio buses (`Master`, `Music`, `SFX`) and event instance playback lifecycles, so that audio integration follows engine standards.
12. As an AI agent, I want documentation on the XML resource management cache and string view hashing, so that asset lookups remain allocation-free.
13. As an engine developer, I want a comprehensive table of naming conventions (PascalCase, camelCase, `m_camelCase`, UPPER_SNAKE_CASE) in `AGENTS.md`, so that new code matches existing style seamlessly.
14. As an engine developer, I want explicit parameter and argument passing conventions (`std::string_view`, `const FilePath&`, `std::span`, pass-by-value sinks), so that memory and performance contracts are preserved.
15. As an engine developer, I want RAII, Rule of 5, and smart pointer conventions (`Ref<T>`, `createRef`, `UniquePtr<T>`, never calling `release()` in destructors) documented, so that resource leaks are prevented.
16. As a maintainer, I want the "No legacy cruft" rule permanently documented, so that agents aggressively delete dead code, unused methods, and legacy structs without asking for permission.
17. As an AI agent, I want the local markdown issue tracking system (`.scratch/`, `ticket.ps1`, triage labels, wayfinding frontier) clearly documented in `AGENTS.md`, so that I navigate and close tickets according to repo conventions.

## Implementation Decisions

- **Top-Level Mandatory Agent Protocol**: The document opens with a binding invariant block requiring every agent session and subagent to inspect `AGENTS.md` and `GLOSSARY.md` on startup, execute builds through `scripts/build.ps1` / `scripts/build.sh`, run convention linting, and synchronize architecture/docs on task completion.
- **Seven-Section Merged Structure**:
  1. *Project Overview & Mission*: Core technologies, static library target, repository paths.
  2. *Project Direction*: High-level roadmap from architecture cleanup to C# scripting and game development.
  3. *Mandatory Agent Behavior & Consultation Protocol*: Behavioral constraints, zero unilateral assumptions, consult-first workflows, incremental delivery, and documentation synchronization.
  4. *Subsystem Architecture & Lifecycle*: Comprehensive system flows including top-level main loop diagram, layer manager, input/event variant dispatching, updated single dynamic quad batcher (with unified `Vertex` and submission order), FMOD audio bus routing, and XML resource caches.
  5. *Code Patterns, Naming & Style*: Formal naming table, argument passing rules, RAII and smart pointer aliases, logging/assert macros, and tab-based indentation.
  6. *Script-First Build, Run & Validation Discipline*: Automated script runner documentation (`build.ps1`, `lint-conventions.ps1`, `run-demo.ps1`), build presets, validation checklist, and the aggressive "No legacy cruft" rule.
  7. *Issue Tracking & Domain Docs*: Integration with `.scratch/` tickets, triage labels, ADR directory, and domain glossary.
- **Synchronizing Documentation with Code**: A formal completion requirement mandates that any code changes affecting terminology, architecture, or subsystems must update `GLOSSARY.md`, record ADRs where trade-offs occur, and update relevant sections of `AGENTS.md`.
- **Script-First Tooling Primacy**: `scripts/build.ps1` (and `scripts/build.sh`) is established as the primary build command because it handles MSVC environment activation and toolchain resolution automatically, preventing shell configuration failures.
- **Universal Rule Application Without Modifying Individual Skills**: Rely on the system's unconditional loading of root `AGENTS.md` into all agent sessions, ensuring these invariants govern every skill without modifying individual skill files.

## Testing Decisions

- **Documentation Consistency and Syntax**: Validate that all markdown links, code blocks, and diagrams are structurally valid and contain zero markdown lint errors.
- **Script Verification**: Verify that `.\scripts\lint-conventions.ps1` runs cleanly and reports zero violations against repository standards.
- **Build Workflow Verification**: Ensure `.\scripts\build.ps1` executes successfully across presets and adheres to documented instructions.

## Out of Scope

- Modifying C++ engine source or demo code.
- Editing individual skill files across `.agent/skills/`.
- Changing build system CMake logic or dependencies.

## Further Notes

- Once this spec is implemented, any new agent session will immediately receive the merged, updated `AGENTS.md` as an always-on rule, solving the issue of agents bypassing project rules or using manual CMake commands.
