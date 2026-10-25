# 02: Optimize Precompiled Header and Eliminate Redundant Manual Includes

Status: resolved
Type: task
Blocked by: 01

**What to build:** Streamline compilation and precompiled header usage:
1. In `OrcEngine/include/Orc/OrcPch.hpp`:
   - Remove `<Windows.h>` and `#define NO` and all surrounding Win32 macros.
   - Prune unused standard headers (`<any>`, `<ranges>`, `<unordered_set>`, `<fstream>`).
   - Add commonly used C++20 standard library headers (`<span>`, `<filesystem>`, `<array>`, `<algorithm>`, `<cmath>`, `<optional>`, `<utility>`).
2. Move `<Windows.h>` (with lean Win32 defines `WIN32_LEAN_AND_MEAN`, `NOMINMAX`, etc.) into `OrcEngine/source/Orc/Script/Scripting.cpp` inside `#ifdef _WIN32` blocks, keeping OS macro pollution strictly private to the Windows dynamic loading call site.
3. In `OrcEngine/source/Orc/Graphics/Shader.cpp`, add `#include <fstream>` directly for file shader loading.
4. Remove `#include "OrcPch.hpp"` from every `.cpp` file in `OrcEngine/source/Orc/`, relying on CMake's `target_precompile_headers(OrcEngine PRIVATE ...)` to force-include the header uniformly.
5. In `CMakeLists.txt` (and `scripts/build.sh`), add fallback discovery for .NET CoreCLR hosting on Linux when `DOTNET_ROOT` is not in the environment, checking standard installation paths (including JetBrains Toolbox Rider/CLion .NET runtime paths).
6. Verify with `./scripts/verify.sh` that compilation passes with 0 warnings and the demo executes cleanly.

**Blocked by:** 01 (Synchronize Coding Standards, Agent Protocols, Formatter, and Linter)

- [x] `OrcPch.hpp` removes `<Windows.h>` and `#define NO` macro pollution
- [x] `OrcPch.hpp` prunes `<any>`, `<ranges>`, `<unordered_set>`, `<fstream>`
- [x] `OrcPch.hpp` adds `<span>`, `<filesystem>`, `<array>`, `<algorithm>`, `<cmath>`, `<optional>`, `<utility>`
- [x] `Scripting.cpp` encapsulates lean `<Windows.h>` under `#ifdef _WIN32`
- [x] `Shader.cpp` includes `<fstream>` directly
- [x] All `.cpp` files in `OrcEngine/source/Orc/` remove `#include "OrcPch.hpp"`
- [x] `CMakeLists.txt` / build tooling robustly discovers .NET CoreCLR hosting on Linux
- [x] Passes `./scripts/verify.sh` (conventions lint, 0-warning build, 5-second smoke test)

## Answer

Optimized OrcPch.hpp by removing Windows.h and unused standard headers, added C++20 standard headers, moved lean Windows.h to Scripting.cpp, added fstream in Shader.cpp, removed manual OrcPch.hpp includes across all 25+ cpp files in OrcEngine, and added fallback .NET discovery.
