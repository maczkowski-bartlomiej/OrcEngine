# Update AudioTest harness and run unified verification gate

Status: resolved
Type: task
Blocked by: 02, 03

## Description

Update the integration harness `TestProject/source/AudioTest.cpp` to validate the new Audio capabilities (`playOneShot`, `SoundHandle` operations, dynamic `getBus`), and verify the entire build against the unified verification gate `.\scripts\verify.ps1`.

### Scope of Work:
1. Update `TestProject/source/AudioTest.cpp` to use the new API:
   - Use `playOneShot()` for SFX buttons.
   - Use `play()` for music tracks, storing `SoundHandle` members on `AudioTest` to allow individual stop, pause, resume, and volume control.
   - Update bus control ImGui panels to use `audio.getBus(Bus::SFX)`, `audio.getBus(Bus::Music)`, and `audio.getBus(Bus::Master)`.
2. Run `.\scripts\verify.ps1` to ensure conventions linting, compilation, and the smoke test all pass with 100% success (zero errors).

### Completion Criteria:
- `TestProject` builds cleanly without warnings or errors.
- `AudioTest` provides interactive verification of `playOneShot`, `SoundHandle` manipulation (individual stop/pause/volume), and dynamic bus modulation.
- `.\scripts\verify.ps1` completes with 100% success across all stages.

## Comments

## Answer

Updated AudioTest demo layer to exercise playOneShot, SoundHandle controls, and dynamic getBus, passing verify.ps1 100%
