# 02: Dissolve Utility and Relocate Clock, Random & StringViewHash to Core

Status: resolved
Type: task
Blocked by: 01

**What to build:** Dissolve the shallow `Engine` `Utility` grab-bag module, relocate `StringViewHash`, `Clock`, and `Random` from `Engine` into Layer 0 (`Core`), fix `Random::getSeed()` so it returns the actual initial seed prior to `setSeed()`, and sever the remaining upward `#include` dependencies from `Graphics` into `Engine`.

## Acceptance Criteria

- [x] The `Utility` header and implementation in `Engine` are deleted, `StringViewHash` is provided by `Core`, and shader file error reporting calls `std::system_category().message(errno)` directly.
- [x] `Clock` and `Random` are relocated from `Engine` to `Core` and exported through the `Core` and top-level umbrella headers.
- [x] `Random` initializes `m_seed` from `std::random_device{}()` first and seeds `m_engine` with `m_seed` so `Random::getSeed()` returns the active seed even before `Random::setSeed()` is called.
- [x] Zero files in `Core`, `Events`, `Input`, `Graphics`, `Audio`, `Script`, or the precompiled header contain an upward `#include` to `Engine`, and internal `#include` directives in `Engine` and `Core` use consistent quoted root-relative paths.
- [x] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments

## Answer

Dissolved Engine Utility module, relocated StringViewHash, Clock, and Random to Core, fixed Random seed initialization truthfulness, and severed all upward includes to Engine from lower-level subsystems.
