# 03: Clean Up Engine Nomenclature

Status: resolved
Type: task
Blocked by: 02

**What to build:** Rename legacy Mono identifiers in `Engine.hpp` and `Engine.cpp` to canonical `Scripting` nomenclature.

**Blocked by:** 02 (needs stable `Scripting` implementation)

- [x] `Engine.hpp` renames member variable `m_mono` to `m_scripting`
- [x] `Engine.cpp` updates instantiation to `m_scripting = createUniquePtr<Scripting>(config.scriptingConfigPath)`
- [x] `Engine.cpp` updates destruction to `m_scripting.reset()`
- [x] Engine static library and TestProject build cleanly without warnings

## Answer

Renamed m_mono to m_scripting in Engine.hpp and Engine.cpp, ensuring consistent canonical Scripting nomenclature across engine lifecycle coordination.
