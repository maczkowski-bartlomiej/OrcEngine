# Audio Subsystem Deepening Map

## Notes
Initial exploratory notes.

## Decisions so far
- [05-fail-fast-invariants-and-docs](issues/05-fail-fast-invariants-and-docs.md): Recorded fail-fast invariant in GLOSSARY.md and AGENTS.md, updated Audio subsystem architecture in AGENTS.md, and recorded ADR 0008
- [04-audiotest-harness-and-verification](issues/04-audiotest-harness-and-verification.md): Updated AudioTest demo layer to exercise playOneShot, SoundHandle controls, and dynamic getBus, passing verify.ps1 100%
- [03-sound-handle-and-instance-lifecycle](issues/03-sound-handle-and-instance-lifecycle.md): Implemented generational SoundHandle, active instance pool, dual playback interface (playOneShot and play), and automatic instance reaping in update()
- [02-dynamic-bus-management](issues/02-dynamic-bus-management.md): Refactored Bus into an opaque proxy without FMOD headers, added standard constants Bus::Master/SFX/Music, and replaced 6 getters with dynamic audio.getBus()
- [01-pimpl-encapsulation-and-cmake](issues/01-pimpl-encapsulation-and-cmake.md): Encapsulated Audio behind PIMPL with UniquePtr<Impl>, removed fmod_studio.hpp from public headers, and downgraded FMOD to PRIVATE in CMakeLists.txt

## Fog
Unknowns and risks to explore.
