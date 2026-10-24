# Translation Unit Linkage and Anonymous Namespaces

Status: resolved
Type: task

## Description
Local helper classes that are strictly internal to a single translation unit and not declared in headers should be scoped inside an anonymous namespace. Conversely, utility functions declared in headers for multi-unit consumption must retain external linkage in `namespace orc`.

### Target Files & Locations
- [OrcEngine/source/Orc/Engine/Logger.cpp:15-30](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Logger.cpp#L15-L30): Enclose `CapitalizedLevelNamesFormatter` in an anonymous namespace `namespace { ... }`.
- [OrcEngine/source/Orc/Engine/Utility.cpp:17-176](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Utility.cpp#L17-L176): Verify and preserve external linkage for `fmodCall`, `ftCall`, and `getErrnoMessage` under `namespace orc`. Do not make them static or anonymous.

### Completion Criteria
1. `CapitalizedLevelNamesFormatter` has internal linkage via anonymous namespace.
2. `fmodCall`, `ftCall`, and `getErrnoMessage` remain exported under `namespace orc`.
3. Engine builds and links without unresolved symbol errors.

## Answer

Enclosed CapitalizedLevelNamesFormatter in an anonymous namespace in Logger.cpp and verified external linkage of Utility functions.
