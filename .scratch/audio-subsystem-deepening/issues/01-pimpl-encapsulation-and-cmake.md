# Encapsulate Audio behind PIMPL and downgrade FMOD to PRIVATE

Status: resolved
Type: task

## Description

Eliminate third-party FMOD header leaks from all engine public headers by encapsulating FMOD handles inside `Audio` behind a PIMPL (`UniquePtr<Audio::Impl> m_impl`).

### Scope of Work:
1. Remove `#include <fmod_studio.hpp>` from `OrcEngine/include/Orc/Audio/Audio.hpp`.
2. Declare `struct Impl;` and `UniquePtr<Impl> m_impl;` in `Audio.hpp`.
3. Declare `~Audio()` non-inline in `Audio.hpp` and implement it in `OrcEngine/source/Orc/Audio/Audio.cpp` where `Audio::Impl` is a complete type.
4. Confine all FMOD header inclusions (`<fmod_studio.hpp>`, `<fmod_common.h>`) strictly to `.cpp` translation units.
5. In `CMakeLists.txt`, move `FMOD::Studio` and `FMOD::Core` from `PUBLIC` to `PRIVATE` in `target_link_libraries(OrcEngine ...)`.

### Completion Criteria:
- No public header in `OrcEngine/include/` includes any FMOD header or references `FMOD::` types.
- `OrcEngine` compiles as a static library with `FMOD` as a private dependency.
- Downstream targets like `TestProject` compile without needing FMOD in their include directories.

## Comments

## Answer

Encapsulated Audio behind PIMPL with UniquePtr<Impl>, removed fmod_studio.hpp from public headers, and downgraded FMOD to PRIVATE in CMakeLists.txt
