# Deepen Layer 0 (`Core`) Architecture

Status: completed

## Problem Statement

Layer 0 (`Core`) in `OrcEngine` is supposed to be the foundational module with zero upward dependencies, while `Engine` acts as the top-level runtime coordinator over all domain subsystems (`Events`, `Input`, `Graphics`, `Audio`, `Scripting`). Currently, this layering invariant is broken in three ways:

1. **Upward Dependency Cycles & Third-Party Header Leakage in Diagnostics (Candidate 1)**:
   - The `Logger` and `Debug` (`Fail-Fast Crash Contract`) modules live inside the `Engine` module rather than `Core`. Because every subsystem (`Events`, `Graphics`, `Audio`, `Scripting`) and the precompiled header require logging and `ORC_FATAL_CHECK` / `ORC_FATAL`, all lower-level subsystems are forced to depend upward on `Engine`, creating a bidirectional dependency cycle between `Engine` and the rest of the codebase.
   - `Logger` exposes `<spdlog/logger.h>` and `<spdlog/fmt/fmt.h>` in its public header and includes `Core` math types solely to reference `FilePath`, leaking third-party `spdlog` and GLM declarations into every translation unit and public header that includes `Event` or `Debug`.
   - `Debug` duplicates the `ORC_FATAL` and `ORC_FATAL_CHECK` macro definitions across `#ifdef ORC_DEBUG_ASSERTS` and `#else` branches, even though `ORC_DEBUGBREAK()` already compiles to a no-op when debug asserts are disabled.

2. **Shallow `Utility` Grab-Bag & Misplaced Layer 0 Primitives in `Engine` (Candidate 2)**:
   - `Engine` contains a shallow two-item `Utility` module (`StringViewHash` and a one-line `getErrnoMessage` pass-through with only a single caller in shader file loading), violating the `GLOSSARY.md` prohibition on generic `UtilityModule` grab-bags.
   - Pure Layer 0 primitives with zero `Engine` dependencies—`Clock` (steady-clock time measurement) and `Random` (Mersenne Twister pseudo-random generation)—also reside in `Engine`, forcing `Graphics` (`Animator`) to depend upward on `Engine` just to measure animation frame time and hash string views.
   - `Random` initializes its stored `m_seed` member to `0` while seeding its internal `std::mt19937` generator from `std::random_device{}()`, causing `Random::getSeed()` to return a stale `0` rather than the actual seed in use prior to an explicit `setSeed()` call.

3. **Single-Caller `FontUtility` Seam & Leaky `Rect<T>` Field Invariant (Candidate 4)**:
   - `ORC_FT_CALL` is isolated in a 12-line internal header included by only a single translation unit (`Font`), requiring a special-case exclusion in the repository's convention linter without providing reuse across multiple callers.
   - `Rect<T>` (`IntRect`, `FloatRect`) names its third and fourth public struct fields `right` and `bottom`, yet its constructor `Rect(x, y, width, height)`, `getSize()`, and `intersects()` store and interpret those fields as `width` and `height`. Every caller across `Texturable`, `Sprite`, `Text`, `Camera`, and resource manifest loading must work around this contradiction between the field names (`right`, `bottom`) and their actual stored meaning (`width`, `height`).

## Solution

Purify Layer 0 (`Core`) so that all foundational diagnostics, time, random, string-hashing, and 2D rectangle primitives live in `Core` with zero upward dependencies on `Engine`, seal `spdlog` completely behind the `Logger` implementation seam, dissolve the shallow `Utility` and single-caller `FontUtility` modules, and align `Rect<T>`'s public fields with its `width` and `height` semantics:

1. **Relocate `Logger` and `Debug` (`Fail-Fast Crash Contract`) to `Core` & Seal `spdlog` (Candidate 1)**:
   - Move `Logger` and `Debug` from `Engine` into `Core` so `Core`, `Events`, `Input`, `Graphics`, `Audio`, `Scripting`, and the precompiled header depend strictly downward on `Core`.
   - Hide `spdlog::logger` behind `Logger`'s implementation by routing `Logger::log` through a type-erased formatting seam (`fmt::string_view` and `fmt::format_args`), eliminating `<spdlog/logger.h>` from public headers and making `spdlog` a private implementation dependency of `OrcEngine`.
   - Narrow `Logger`'s header dependencies to `<filesystem>` (or `FilePath`) without pulling GLM math types into `Logger` or `Debug`.
   - Collapse the duplicated `#ifdef ORC_DEBUG_ASSERTS` macro blocks in `Debug` into a single unified definition of `ORC_FATAL` and `ORC_FATAL_CHECK` backed by `ORC_DEBUGBREAK()`.

2. **Dissolve `Utility` and Move `StringViewHash`, `Clock`, and `Random` to `Core` (Candidate 2)**:
   - Delete the `Utility` module (`Utility` header and implementation).
   - Move `StringViewHash` into `Core` and inline `std::system_category().message(errno)` at its sole call site in shader file reading.
   - Move `Clock` and `Random` from `Engine` into `Core` so `Graphics` (`Animator`) and `Engine` both consume time and hashing primitives directly from Layer 0 (`Core`).
   - Fix `Random` initialization so `m_seed` captures the initial `std::random_device{}()` value used to seed `m_engine`, ensuring `Random::getSeed()` always returns the active seed.

3. **Fold Single-Caller `FontUtility` & Repair the `Rect<T>` Interface Invariant (Candidate 4)**:
   - Inline `ORC_FT_CALL` directly into the `Font` implementation and delete the single-caller `FontUtility` header and its convention-linter exception.
   - Rename `Rect<T>`'s third and fourth fields from `right` and `bottom` to `width` and `height`, relocate `Rect<T>` (`IntRect`, `FloatRect`) into `Core` as a foundational geometric primitive alongside `Vector2<T>`, and update all `Graphics`, `Engine`, and `TestProject` callers to use `.width` and `.height`.

## User Stories

1. As an engine maintainer, I want `Logger` and `Debug` (`ORC_FATAL`, `ORC_FATAL_CHECK`) to reside in `Core`, so that `Events`, `Input`, `Graphics`, `Audio`, and `Scripting` never include headers from `Engine`.
2. As an engine maintainer, I want the precompiled header to include only standard library headers and `Core` headers, so that Layer 1 subsystems cannot accidentally rely on implicit upward `Engine` includes.
3. As an engine maintainer, I want `<spdlog/logger.h>` hidden inside `Logger`'s implementation rather than exposed in `Logger`'s public header, so that public engine headers remain free of third-party logging SDK types and compile faster.
4. As an engine maintainer, I want `spdlog` linked `PRIVATE` to `OrcEngine` (while keeping `fmt` formatting available to callers of `ORC_LOG_*` and `ORC_FATAL*`), so that consumer targets like `TestProject` are insulated from `spdlog` internals.
5. As an engine maintainer, I want `ORC_FATAL` and `ORC_FATAL_CHECK` defined once in `Debug` without `#ifdef ORC_DEBUG_ASSERTS` duplication, so that the `Fail-Fast Crash Contract` has a single authoritative macro definition across Debug and Release builds.
6. As an engine maintainer, I want `Utility` deleted and `StringViewHash` provided by `Core`, so that heterogeneous `std::string_view` map lookups work across `Animator`, `GameLayerManager`, and `Resources` without a grab-bag `Utility` module.
7. As an engine maintainer, I want shader file stream error reporting to query `std::system_category().message(errno)` directly, so that a separate translation unit is not maintained for a one-line standard library call.
8. As an engine maintainer, I want `Clock` located in `Core`, so that `Animator` and `Engine` can measure elapsed time without `Graphics` depending on `Engine`.
9. As a game developer, I want `Random` located in `Core` with `Random::getSeed()` returning the exact seed used to initialize the generator even before `Random::setSeed()` is called, so that procedural runs can be logged and reproduced deterministically.
10. As an engine maintainer, I want `ORC_FT_CALL` defined locally inside `Font`'s implementation rather than in a single-caller header, so that private FreeType error handling has high locality and requires no convention-linter workarounds.
11. As a game developer using `Rect<T>` (`IntRect`, `FloatRect`), I want its fields named `left`, `top`, `width`, and `height`, so that reading struct members matches the constructor parameters `(x, y, width, height)`, `getSize()`, and `intersects()` without off-by-origin bugs.
12. As an engine maintainer, I want `Rect<T>` located in `Core` alongside `Vector2<T>`, so that 2D axis-aligned bounding boxes are available as a Layer 0 primitive across all subsystems.
13. As an engine maintainer, I want `Texturable`, `Sprite`, `Text`, `Camera`, and animation manifest loading updated to use `Rect<T>::width` and `Rect<T>::height` cleanly, so that UV calculation, bounding-box calculation, and orthographic `Viewport` projection are self-documenting.
14. As an engine maintainer, I want `GLOSSARY.md`, `AGENTS.md`, `CODING_STANDARDS.md`, and `docs/adr/` updated to reflect the deepened `Core` module and cleaned-up seam locations, so that documentation stays 100% synchronized with the codebase.

## Implementation Decisions

- **Relocating Diagnostics (`Logger` & `Debug`) to `Core`**:
  - `Logger` and `Debug` move from `Engine` to `Core` and are exported through the `Core` and top-level umbrella headers.
  - `Logger` replaces its template direct call to `m_logger->log(...)` in the header with a thin inline template front-end that packs variadic arguments via `fmt::make_format_args` and delegates to a non-template `Logger::logImpl(Level level, std::source_location sourceLocation, fmt::string_view format, fmt::format_args args)` method implemented in `Logger`'s source file.
  - `Logger`'s public header removes `<spdlog/logger.h>`, `<spdlog/fmt/fmt.h>`, and `m_logger` (`std::shared_ptr<spdlog::logger>`), keeping the `spdlog` logger instance inside an anonymous namespace (or file-static storage) in `Logger`'s implementation file.
  - In CMake, `spdlog::spdlog` is linked `PRIVATE` (and `fmt::fmt` `PUBLIC`) on `OrcEngine`, fulfilling the coding standard that third-party SDK headers do not leak through public headers.
  - `Debug` defines `ORC_FATAL` and `ORC_FATAL_CHECK` once: both macros invoke `ORC_LOG_FATAL`, `ORC_DEBUGBREAK()` (which expands to a no-op when `ORC_DEBUG_ASSERTS` is not defined), and `std::abort()`.
- **Dissolving `Utility` and Relocating `Clock`, `Random`, and `StringViewHash` to `Core`**:
  - The `Utility` header and source file in `Engine` are deleted.
  - `StringViewHash` is placed in `Core` so any module including `Core` can construct heterogeneous string-keyed unordered maps.
  - The single call to `getErrnoMessage(errno)` in shader file loading is replaced with `std::system_category().message(errno)` using `<system_error>`.
  - `Clock` and `Random` move from `Engine` to `Core`.
  - `Random` initializes `m_seed` from `std::random_device{}()` first and seeds `m_engine` with `m_seed`, guaranteeing that `Random::getSeed()` always reflects the active generator seed from startup.
- **Inlining `FontUtility` into `Font`**:
  - The single-caller `FontUtility` private header is deleted, and `ORC_FT_CALL` is defined directly at the top of `Font`'s implementation file.
  - The `*FontUtility.hpp` exemption in `scripts/lint-conventions.ps1` and `scripts/lint-conventions.sh` is removed.
- **Repairing `Rect<T>` Field Semantics and Relocating to `Core`**:
  - `Rect<T>` (`IntRect`, `FloatRect`) moves to `Core` and renames its fields from `left, top, right, bottom` to `left, top, width, height`.
  - `Rect<T>::getPosition()` returns `Vector2<T>(left, top)`, `Rect<T>::getSize()` returns `Vector2<T>(width, height)`, and `Rect<T>::intersects(const Rect& other)` checks `left < other.left + other.width && left + width > other.left && top < other.top + other.height && top + height > other.top`.
  - In `Texturable::updateVerticesTexCoords()`, normalized UV min/max coordinates are computed as local `float` or `Vector2f` bounds (`u0`, `v0`, `u1`, `v1`) rather than stuffing `(left + width)` and `(top + height)` into a `FloatRect`.
  - In `Camera`, `setViewportSize` and `recalculateViewMatrix` use `m_viewport.width` and `m_viewport.height` (with orthographic projection `glm::ortho(m_viewport.left, m_viewport.left + m_viewport.width, m_viewport.top + m_viewport.height, m_viewport.top, 1.0f, -1.0f)`), preserving identical behavior when `left == 0` and `top == 0` while becoming mathematically sound for non-zero origins.
  - All remaining references to `.right` and `.bottom` on `Rect<T>` in `Sprite`, `Text`, `Texturable`, and `Resources<Animation>` are updated to `.width` and `.height`.
- **Layering Invariant Enforcement**:
  - After these moves, zero files in `Core`, `Events`, `Input`, `Graphics`, `Audio`, or `Script` will `#include` any header from `Engine`.
  - All angle-bracket or relative internal `#include` directives in `Engine` and `Core` are normalized to quoted root-relative paths (`"Core/..."`, `"Engine/..."`, etc.).

## Testing Decisions

- **What Makes a Good Test**:
  - Tests and verification checks must validate external module behavior across public seams (`Core` primitives, `Logger` / `Debug` formatting and fail-fast contracts, `Clock` / `Random` determinism, `Rect<T>` geometry, and strict downward `#include` layering) rather than internal implementation details such as private `spdlog` sink objects.
- **Primary Testing Seams**:
  1. **Unified Verification & Smoke Seam (`scripts/verify.ps1` / `scripts/verify.sh`)**: Exercises convention linting (`lint-conventions`), full Debug compilation with zero compiler warnings across `OrcEngine` and `TestProject`, and a 5-second live runtime smoke test of `TestProject` (verifying `Logger` initialization and output, `Clock` frame timing, `Animator` and `Sprite` `FloatRect` rendering, `Font` glyph loading via `ORC_FT_CALL`, and `Camera` `Viewport` projection).
  2. **Architectural Layer Seam Verification**: Verifies that no header or source file in `Core`, `Events`, `Input`, `Graphics`, `Audio`, `Script`, or `OrcPch.hpp` contains an upward `#include "Engine/..."`, and no public header in `OrcEngine/include/Orc/` includes `<spdlog/...>`.
- **Modules to Be Tested & Verified**:
  - `Core` diagnostics (`Logger`, `Debug`): formatting with zero and multiple arguments, source-location reporting, and absence of `<spdlog/...>` in public headers.
  - `Core` utilities and time/random primitives (`Clock`, `Random`, `StringViewHash`): `Random::getSeed()` truthfulness before and after `setSeed()`, and heterogeneous `std::string_view` map lookups.
  - `Core` / `Graphics` `Rect<T>` geometry: `Sprite` UV mapping, `Text` local/global bounds, `Animation` frame slicing, and `Camera` projection in `TestProject`.
- **Prior Art in the Codebase**:
  - `scripts/verify.ps1` / `scripts/verify.sh` (`scripts/lint-conventions.ps1`, `scripts/build.ps1`, and `scripts/run-demo.ps1 -TimeoutSeconds 5 -FailOnErrorLog`).

## Out of Scope

- Collapsing the existing `Core` headers (`Core`, `Memory`, `Platform`, `Types`, `Version`) into a single file or relocating `RendererID` (Candidate 3 from the architecture review).
- Adding new logging sinks, runtime log-level configuration files, or asynchronous logging threads.
- Adding new random number distributions beyond the existing uniform integer, float, and double methods on `Random`.

## Further Notes

- Updates `GLOSSARY.md` (`Core` definition), `AGENTS.md` (file path pointers for `Debug`, `Logger`, and `ORC_FT_CALL`), `CODING_STANDARDS.md` (header hygiene and error-handling file paths), and records an ADR in `docs/adr/` documenting the Layer 0 (`Core`) purification and `spdlog` encapsulation seam.
