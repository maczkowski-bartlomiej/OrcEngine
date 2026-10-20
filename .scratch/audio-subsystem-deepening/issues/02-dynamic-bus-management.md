# Refactor Bus into opaque wrapper and implement dynamic bus lookup

Status: resolved
Type: task
Blocked by: 01

## Description

Refactor `Bus` into a clean, opaque wrapper that does not leak FMOD types across its public interface, and replace hardcoded bus getters on `Audio` with dynamic lookup.

### Scope of Work:
1. Remove `#include <fmod_studio.hpp>` and `void load(FMOD::Studio::System*, ...)` from `OrcEngine/include/Orc/Audio/Bus.hpp`.
2. Make `Bus` an opaque wrapper or proxy that stores an internal handle/pointer managed internally without public FMOD types.
3. Expose standard bus path constants:
   - `static constexpr std::string_view Master = "bus:/";`
   - `static constexpr std::string_view SFX = "bus:/SFX";`
   - `static constexpr std::string_view Music = "bus:/Music";`
4. In `Audio`, delete the 6 hardcoded getters (`getSfxBus`, `getMusicBus`, `getMasterBus` and const overloads).
5. Implement `Bus getBus(std::string_view busPath)` in `Audio` with internal caching of `FMOD::Studio::Bus*`.
6. Apply the fail-fast philosophy: if an invalid bus path is queried, trigger `ORC_ASSERT` so mistakes are caught immediately.

### Completion Criteria:
- `Bus.hpp` contains zero FMOD headers or types.
- Callers can query any named bus via `audio.getBus(...)`.
- `Bus` allows calling `stop()`, `pause()`, `resume()`, `getVolume()`, `setVolume()`, `isPaused()`.
- Invalid bus access fails fast.

## Comments

## Answer

Refactored Bus into an opaque proxy without FMOD headers, added standard constants Bus::Master/SFX/Music, and replaced 6 getters with dynamic audio.getBus()
