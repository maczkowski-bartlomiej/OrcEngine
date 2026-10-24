# 02: Migrate Includes and Remove Legacy Engine/Core.hpp

Status: completed
Type: task
Blocked by: 01

**What to build:** Complete the contract phase of the expand-contract refactor by migrating all include sites throughout the engine and demo harness to the new `Orc/Core/` headers, followed by deleting the legacy header so no obsolete paths remain.

**Blocked by:** 01 (Create Orc/Core Modular Headers)

- [x] All `#include "Engine/Core.hpp"` directives in public engine headers are replaced by `#include "Core/Core.hpp"` or specific focused core headers
- [x] All `#include "Engine/Core.hpp"` directives in engine source translation units are updated to `#include "Core/Core.hpp"` or specific focused core headers
- [x] Top-level umbrella header `Orc/Orc.hpp` exports `Core/Core.hpp`
- [x] All references in `TestProject` (and CMake comment markers) are updated to reference the new headers
- [x] The legacy file `OrcEngine/include/Orc/Engine/Core.hpp` is permanently deleted
- [x] The codebase builds cleanly with zero errors on all presets
