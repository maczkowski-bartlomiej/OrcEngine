# Spec: Audio Subsystem Deepening

## Problem Statement

The current audio subsystem in OrcEngine suffers from three architectural frictions that limit caller leverage, leak third-party dependencies, and hinder maintainability:

1. **Third-Party SDK Leakage Across Public Seams**: The public headers `Audio.hpp` and `Bus.hpp` directly `#include <fmod_studio.hpp>`, exposing native FMOD types (`FMOD::Studio::System*`, `FMOD::Studio::Bank*`, `FMOD::Studio::Bus*`) to every client translation unit that includes `<Orc/Orc.hpp>`. Consequently, `CMakeLists.txt` is forced to link `FMOD::Studio` and `FMOD::Core` as `PUBLIC` target dependencies, polluting downstream client compilation units and increasing build times.
2. **Shallow Fire-and-Forget Sound Playback**: The single playback method `Audio::play(const std::string& eventPath)` creates, starts, and immediately releases the FMOD event instance. Callers have zero leverage over playing sounds: they cannot stop a looping sound, pause/resume a track, adjust playback volume, or modulate FMOD event parameters without stopping the entire audio mixing bus.
3. **Shallow Bus Module & Leaky Initialization**: `Bus` is a paper-thin pass-through wrapper whose public `load()` method leaks raw `FMOD::Studio::System*` pointers. Furthermore, `Audio` hardcodes exactly three buses with six repetitive getters, preventing games from defining or querying custom mixing hierarchies (e.g. `"bus:/UI"`, `"bus:/Voice"`, `"bus:/Ambience"`).

## Solution

Deepen the audio subsystem to provide a high-leverage, leak-free, fail-fast interface:

1. **PIMPL Encapsulation**: Encapsulate all native FMOD system handles, loaded banks, and instance caches behind an opaque implementation pointer (`UniquePtr<Audio::Impl> m_impl`) in `Audio.cpp`. Eliminate all FMOD headers from public engine headers and downgrade FMOD to a `PRIVATE` link dependency in CMake.
2. **Generational SoundHandle & Dual Playback Interface**: Provide `playOneShot(path)` for transient sounds (UI clicks, one-off effects) and `play(path) -> SoundHandle` for controllable sound instances. A lightweight `SoundHandle` value type provides caller leverage (`stop()`, `pause()`, `resume()`, `setVolume()`, `setParameter()`), backed by an internal pool in `Audio` where finished sounds are automatically reclaimed during `Audio::update()`.
3. **Dynamic Bus Lookup & Opaque Bus Handle**: Replace the six hardcoded bus getters and leaky `Bus::load()` with dynamic lookup via `audio.getBus(busPath)`, returning an opaque `Bus` handle. Support standard constants (`Bus::Master`, `Bus::SFX`, `Bus::Music`) while enabling games to control arbitrary mixing buses.
4. **Engine-Wide Fail-Fast Philosophy**: Enforce immediate crashes / assertions (`ORC_ASSERT`) whenever invalid audio state or non-existent assets/buses are encountered, aligning with the engine philosophy that invalid states should fail fast to expedite bug detection during development.

## User Stories

1. As a game developer, I want `Audio.hpp` and `Bus.hpp` to contain only standard C++ and OrcEngine types, so that my game does not compile or depend directly on third-party FMOD headers.
2. As a library consumer, I want `CMakeLists.txt` to link FMOD as a `PRIVATE` dependency of `OrcEngine`, so that downstream targets do not inherit unnecessary include paths or linker constraints.
3. As a game layer developer, I want to trigger fire-and-forget sound effects with `playOneShot()`, so that transient sounds execute with minimal overhead without requiring instance tracking.
4. As a game layer developer, I want `play()` to return a `SoundHandle`, so that I can hold an identifier to control persistent sound playback over time.
5. As a game layer developer holding a `SoundHandle`, I want to stop, pause, and resume an individual sound instance, so that background music or ambient loops can be stopped without killing other concurrent sounds.
6. As a game layer developer holding a `SoundHandle`, I want to adjust the volume and pitch of a specific playing sound, so that positional attenuation or dynamic fading can be performed.
7. As a game layer developer holding a `SoundHandle`, I want to set named FMOD event parameters, so that dynamic interactive soundscapes (e.g. engine RPM, player health, musical intensity) can be modulated at runtime.
8. As a game layer developer holding an expired `SoundHandle`, I want operations on that handle to safely no-op without crashing or corrupting memory, so that natural sound termination does not cause dangling pointer bugs.
9. As an engine coordinator running `Engine::run()`, I want `Audio::update()` to automatically detect stopped sound instances and recycle their internal pool slots, so that callers never have to manually manage sound lifecycles.
10. As a sound designer, I want to query arbitrary mixing buses dynamically by path (e.g. `"bus:/UI"`, `"bus:/Ambience"`, `"bus:/Dialogue"`), so that game-specific audio mixing hierarchies are fully supported.
11. As a game developer, I want standard bus path constants (`Bus::Master`, `Bus::SFX`, `Bus::Music`) exposed on `Bus`, so that common bus queries are type-safe and typo-free.
12. As a game developer adjusting audio settings, I want `Bus` to allow querying and setting bus volume, pause state, and stopping all events on that bus without exposing low-level system handles.
13. As an engine developer debugging audio issues, I want the engine to assert and fail fast immediately when an invalid bus or corrupt audio state is accessed, so that bugs are identified and fixed immediately during development.
14. As an interactive demo user in `TestProject`, I want `AudioTest` to provide controls for playing one-shots, playing music with explicit stop/pause/volume handles, and modulating buses, so that all deepened capabilities can be visually validated.
15. As an engine contributor, I want `GLOSSARY.md`, `AGENTS.md`, and an ADR in `docs/adr/` updated to reflect the new architecture and the fail-fast invariant, ensuring complete documentation synchronization.

## Implementation Decisions

- **PIMPL Implementation Mechanics**:
  - `Audio` declares `struct Impl;` and stores `UniquePtr<Impl> m_impl;`.
  - The constructor `Audio::Audio(const AudioSettings&)` initializes `m_impl = createUniquePtr<Impl>()` in `Audio.cpp`.
  - The destructor `Audio::~Audio()` is defined in `Audio.cpp` where `Audio::Impl` is a complete type, satisfying `std::default_delete<Impl>` invariants.
  - Native FMOD headers (`<fmod_studio.hpp>`, `<fmod_common.h>`, etc.) are included exclusively in `Audio.cpp` and `Bus.cpp`.
- **Private CMake Dependencies**:
  - In `CMakeLists.txt`, `FMOD::Studio` and `FMOD::Core` are moved from `PUBLIC` to `PRIVATE` in `target_link_libraries(OrcEngine ...)`.
- **SoundHandle Architecture & Lifetime**:
  - `SoundHandle` is a lightweight value-type struct storing a reference/pointer to `Audio`, a slot index, and a generation counter (`uint32_t`).
  - Active sound instances are stored inside `Audio::Impl` in a generational slot pool.
  - When `play(path)` is called, a new event instance is created and started, assigned an active slot, and a `SoundHandle` is returned.
  - During `Audio::update()`, active slots are polled for playback state. If an instance has stopped, it is released and its generation counter is incremented, immediately invalidating any outstanding `SoundHandle` copies.
  - `SoundHandle` exposes helper methods (`stop(bool immediate = false)`, `pause()`, `resume()`, `setVolume(float volume)`, `setParameter(std::string_view name, float value)`, `isValid()`) that delegate directly to `Audio`.
- **Dynamic Bus Architecture**:
  - `Bus` encapsulates an opaque internal handle or pointer without exposing FMOD types.
  - The public `Bus::load(FMOD::Studio::System*, const std::string&)` method is deleted.
  - `Audio::getBus(std::string_view busPath)` performs dynamic lookup and caching of buses from the FMOD system.
  - Predefined constants `Bus::Master`, `Bus::SFX`, and `Bus::Music` provide convenient access to default buses.
  - Redundant getters (`getSfxBus`, `getMusicBus`, `getMasterBus` and const overloads) on `Audio` are deleted.
- **Fail-Fast Philosophy Invariant**:
  - In line with the engine philosophy ("when we get invalid state we should crash the game/engine so we can fix the bug quickly"), requesting an invalid bus path or performing invalid operations triggers an `ORC_ASSERT` or fatal log, halting execution immediately rather than silently failing.

## Testing Decisions

- **Definition of Good Test**:
  - Tests must verify external behavior through the highest public seam (`LayerContext.audio` and `SoundHandle`) without inspecting private FMOD internals.
  - Verification includes:
    1. Validating that `playOneShot()` triggers sound effects cleanly.
    2. Validating that `play()` returns an active `SoundHandle` whose volume, pause, resume, and stop operations directly affect playback state.
    3. Validating that stopped sound handles report `isValid() == false` after `Audio::update()`.
    4. Validating dynamic bus retrieval and volume/pause control across master, music, sfx, and custom buses.
- **Modules Tested**:
  - `Audio`, `Bus`, `SoundHandle`, `LayerContext`, and `TestProject` (`AudioTest`).
- **Prior Art**:
  - Existing `TestProject/source/AudioTest.cpp` interactive demo layer and the unified verification gate (`.\scripts\verify.ps1`).

## Out of Scope

- 3D spatial listener positioning and Doppler calculations (deferred to future 3D audio layer extensions).
- Custom DSP audio filter graphs or procedural audio synthesis.
- Hot-reloading of audio banks at runtime.

## Further Notes

- Completes deepening candidates #1, #2, and #3 identified during the Audio architecture review.
- All code changes must pass the unified verification gate `.\scripts\verify.ps1` (conventions linting, build preset, and 5-second smoke test) with 100% success.
