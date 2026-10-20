# 01: Create Orc/Core Modular Headers

Status: completed
Type: task
Blocked by: none

**What to build:** Establish the new modular `Orc/Core/` domain headers alongside the legacy header, following an expand-contract refactoring pattern so that all foundational types, macros, and memory helpers are available from focused headers without breaking existing code.

**Blocked by:** None (can start immediately)

- [x] A dedicated platform header provides platform detection, debugbreak, and debug assertion macros under `Orc/Core/`
- [x] A dedicated memory header provides smart pointer aliases (`Ref`, `UniquePtr`) and factories (`createRef`, `createUniquePtr`) under `Orc/Core/`
- [x] A dedicated types header provides GLM configuration, math vector/matrix aliases, and core scalar types under `Orc/Core/`
- [x] A dedicated version header provides engine semantic version constants under `Orc/Core/`
- [x] An aggregate umbrella header `Orc/Core/Core.hpp` includes all four sub-headers
- [x] Legacy `Orc/Engine/Core.hpp` forwards directly to `Orc/Core/Core.hpp` to keep existing call sites compiling while migration is in progress
- [x] The engine static library and demo compile with zero warnings or errors
