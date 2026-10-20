# Implement SoundHandle, instance pooling, and dual playback interface

Status: resolved
Type: task
Blocked by: 01

## Description

Deepen sound playback by introducing a generational `SoundHandle` value type and a dual playback interface distinguishing between transient fire-and-forget sounds and controlled sound instances.

### Scope of Work:
1. Create `OrcEngine/include/Orc/Audio/SoundHandle.hpp` and `OrcEngine/source/Orc/Audio/SoundHandle.cpp`.
2. Implement `SoundHandle` as a lightweight value type holding `uint32_t index`, `uint32_t generation`, and a back-pointer to `Audio`.
3. Provide methods on `SoundHandle`:
   - `bool isValid() const;`
   - `void stop(bool immediate = false);`
   - `void pause();`
   - `void resume();`
   - `void setVolume(float volume);`
   - `void setParameter(std::string_view name, float value);`
4. In `Audio`, implement:
   - `void playOneShot(std::string_view eventPath);` (fire-and-forget: creates, starts, and releases immediately).
   - `SoundHandle play(std::string_view eventPath);` (registers into active instance pool and returns handle).
5. In `Audio::update()`, scan the active instance pool, detect stopped sounds (`FMOD_STUDIO_PLAYBACK_STOPPED`), release the FMOD event instance, and increment the pool slot's generation counter.
6. Include `SoundHandle.hpp` in `OrcEngine/include/Orc/Orc.hpp`.

### Completion Criteria:
- Callers can play transient one-shots via `audio.playOneShot(path)`.
- Callers can play controllable sounds via `SoundHandle s = audio.play(path)`.
- Sounds can be stopped, paused, resumed, and parameterized via the handle.
- Natural sound termination invalidates expired handles without dangling pointers.
- Active instances are automatically cleaned up in `Audio::update()`.

## Comments

## Answer

Implemented generational SoundHandle, active instance pool, dual playback interface (playOneShot and play), and automatic instance reaping in update()
