# Spec: Core Header Modularization

## Problem Statement

As an engine contributor and game developer building on OrcEngine, the primary foundational header containing fundamental types, smart pointer definitions, math aliases, platform macros, and version constants is currently buried inside the engine coordinator module (`Orc/Engine/Core.hpp`). This naming and folder organization violates the engine's modular layering: core types and memory utilities are not engine lifecycle coordinators, but universal primitives upon which all subsystems (Graphics, Audio, Input, GUI, Resources, and Engine) depend. Having all unrelated primitives bundled into an improperly located, catch-all header causes confusion, creates circular conceptual dependencies, and prevents consumers from including only the granular primitives they need (such as pure math vector types or smart pointers).

## Solution

Decompose and relocate the foundational definitions into a dedicated, properly named `Orc/Core/` module. The monolithic header will be split into focused, single-responsibility headers for platform macros, memory ownership utilities, type/math aliases, and semantic versioning. To maintain ergonomic developer workflows and clear umbrella entry points, an aggregate `Orc/Core/Core.hpp` header will be provided, and all engine subsystems and client applications will be migrated cleanly to the new header hierarchy.

## User Stories

1. As a game developer, I want fundamental type aliases and math vectors to live in a dedicated `Orc/Core/` domain, so that foundational primitives are logically separated from high-level engine lifecycle modules.
2. As an engine developer, I want smart pointer aliases and factory functions (`Ref`, `UniquePtr`, `createRef`, `createUniquePtr`) defined in a focused memory header, so that memory management utilities have a clear single responsibility.
3. As an engine contributor, I want platform detection and debugbreak macros placed in a dedicated platform header, so that platform-specific intrinsics and signals do not pollute unrelated data structures.
4. As an engine contributor, I want GLM configuration and vector/matrix aliases consolidated in a focused types header, so that math primitives can be included without pulling in unrelated platform or version logic.
5. As an engine developer, I want version constants placed in a dedicated version header, so that version tracking can be updated and queried independently.
6. As a game developer, I want an umbrella `Orc/Core/Core.hpp` header that aggregates all core sub-headers, so that I can conveniently import all foundational primitives in one include if desired.
7. As a game developer, I want the top-level `Orc/Orc.hpp` umbrella header to export the new `Orc/Core/Core.hpp`, so that standard all-in-one includes continue to work seamlessly.
8. As a codebase maintainer, I want all obsolete `#include "Engine/Core.hpp"` directives across all engine headers, engine source files, and demo applications updated cleanly to the new core headers, so that no stale paths or broken includes remain in the tree.
9. As a library maintainer, I want the legacy `Orc/Engine/Core.hpp` file completely removed, so that there is no ambiguity about the correct location of core definitions.
10. As an agent following project invariants, I want the domain documentation (`GLOSSARY.md` and `AGENTS.md`) updated alongside the header relocation, so that ubiquitous language and architectural invariants stay strictly in sync.

## Implementation Decisions

- **Domain Separation**: A new top-level `Orc/Core/` directory will be established under public engine headers, representing the foundational Layer 0 module upon which all other engine subsystems depend.
- **Granular Decomposition**:
  - A platform module header will contain platform detection logic, debug break macros (`ORC_DEBUGBREAK`), assert toggle flags (`ORC_DEBUG_ASSERTS`), and platform intrinsic headers.
  - A memory module header will contain smart pointer type aliases (`Ref<T>`, `UniquePtr<T>`) and factory functions (`createRef`, `createUniquePtr`).
  - A types module header will configure GLM (`GLM_FORCE_CTOR_INIT`, `GLM_ENABLE_EXPERIMENTAL`), export vector and matrix aliases (`Vector2`, `Vector2f`, `Vector3f`, `Vector4f`, `Matrix`, `Matrix4`), file path aliases (`FilePath`), and driver ID definitions (`RendererID`).
  - A version module header will define semantic engine version constants (`MAJOR_VERSION`, `MINOR_VERSION`, `PATCH_VERSION`) inside the engine's version namespace.
  - An umbrella core header (`Orc/Core/Core.hpp`) will include all four granular headers, preserving convenience for modules that require complete core facilities.
- **Header Elimination & Include Migration**:
  - The legacy `Orc/Engine/Core.hpp` will be removed.
  - All existing include directives throughout the engine library and demo applications will be updated to reference `Core/Core.hpp` or the specific granular header as appropriate.
  - Top-level umbrella header `Orc/Orc.hpp` will include `Core/Core.hpp`.
- **Namespace Invariant**:
  - All symbols will continue to reside in the top-level engine namespace (`orc`) and nested namespaces (`orc::version`), preserving complete API compatibility for existing code.

## Testing Decisions

- **Definition of Good Test**: The verification must validate observable compilation and linking across the entire static library and executable demo suite. It must verify that all type aliases, macros, memory functions, and vector mathematics compile without warnings or errors, that no circular inclusions exist, and that all repo style and file conventions pass linting.
- **Modules Tested**: `OrcEngine` static library, `TestProject` demo application, CMake build definitions, and convention linter scripts.
- **Prior Art**: Existing CMake build pipeline and verification script (`.\scripts\verify.ps1` / `./scripts/verify.sh` and `.\scripts\lint-conventions.ps1`), which enforce convention compliance, MSVC/Ninja compilation, and execution of the smoke test.

## Out of Scope

- Introducing new smart pointer wrappers or custom memory allocators.
- Modifying GLM math types beyond the existing vector/matrix aliases.
- Redesigning logging or assert macros (which reside in `Debug.hpp` and `Logger.hpp`).

## Further Notes

- This refactor directly fulfills issue [03] from `.scratch/ideas/issues/03-core-hpp-bad-file-naming.md`.
- All changes must strictly pass `.\scripts\verify.ps1` before completing the feature.
