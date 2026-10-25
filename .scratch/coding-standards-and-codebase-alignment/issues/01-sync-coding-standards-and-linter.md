# 01: Synchronize Coding Standards, Agent Protocols, Formatter, and Linter

Status: resolved
Type: task
Blocked by: none

**What to build:** Synchronize `CODING_STANDARDS.md`, `AGENTS.md`, `.clang-format`, and `scripts/lint-conventions.ps1` to accurately describe and enforce the project's C++20 conventions:
1. Organize C++ rules under an explicit C++20 section in `CODING_STANDARDS.md` and remove premature claims of C# coverage.
2. In the naming convention table, specify that `m_camelCase` applies to class member variables, whereas public fields of plain data structs (`Config`, `LayerContext`, `Vertex`, `Color`, `Rect`, `AudioSettings`, `VideoSettings`, `SpecialKeys`, and `Event` types) use unprefixed `camelCase`.
3. In `CODING_STANDARDS.md` and `.clang-format`, specify Allman braces for classes, structs, enums, functions, and all control statements (`if`, `else`, `for`, `while`, `do`, including single-statement bodies), while keeping `namespace` and lambda opening braces on the same line and omitting block braces on `switch` `case` labels.
4. Update the precompiled header rules in `CODING_STANDARDS.md` and `AGENTS.md` to document that `OrcPch.hpp` is automatically force-included by CMake via `target_precompile_headers` and must not be manually `#include`d in `.cpp` files. Update `.clang-format` include categories accordingly.
5. In `CODING_STANDARDS.md`, mandate that observer getters returning shared resources (`Ref<T>`) return `const Ref<T>&`, and clarify that OpenGL bind/uniform methods are `const` because they do not mutate C++ asset state.
6. In `CODING_STANDARDS.md`, require public engine headers to remain free of third-party SDK headers (`fmod`, `freetype`, OS, and .NET hosting headers).
7. Fix broken ADR and directory links in `AGENTS.md` (ADR 0003, ADR 0004 links, `OrcPch.hpp` path, and `Orc/Events/`, `Orc/Input/`, `Orc/Graphics/Window.hpp` paths).
8. Update `scripts/lint-conventions.ps1` to detect and flag trailing whitespace, bringing it into 1:1 parity with `scripts/lint-conventions.sh`.

**Blocked by:** None (can start immediately)

- [x] `CODING_STANDARDS.md` organizes C++ rules under an explicit C++20 section without claiming C# coverage
- [x] `CODING_STANDARDS.md` documents `m_camelCase` for class members vs. `camelCase` for public struct fields
- [x] `CODING_STANDARDS.md` documents Allman braces for all control statements and same-line for namespace/lambda
- [x] `CODING_STANDARDS.md` and `AGENTS.md` document automatic CMake force-inclusion of `OrcPch.hpp` without manual includes
- [x] `CODING_STANDARDS.md` mandates `const Ref<T>&` for observer getters and explains OpenGL bind/uniform logical constness
- [x] `CODING_STANDARDS.md` mandates zero third-party SDK headers in public engine headers
- [x] `AGENTS.md` fixes ADR 0003/0004 links, `OrcPch.hpp` path, and `Orc/Events/` / `Orc/Input/` / `Orc/Graphics/Window.hpp` paths
- [x] `.clang-format` include categories and brace settings align with updated standards
- [x] `scripts/lint-conventions.ps1` checks and flags trailing whitespace in parity with `scripts/lint-conventions.sh`
- [x] Conventions linter passes across the repository

## Answer

Synchronized CODING_STANDARDS.md, AGENTS.md, .clang-format, and scripts/lint-conventions.ps1 with authoritative C++20 conventions, ADR links, struct vs class naming, Allman braces, and trailing whitespace linting.
