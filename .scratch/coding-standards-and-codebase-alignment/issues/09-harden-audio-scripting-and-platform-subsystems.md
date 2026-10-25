# 09: Harden Audio, Scripting, EntryPoint, and Platform Runtime Subsystems

Status: resolved
Type: task
Blocked by: 07

**What to build:** Resolve latent bugs and API gaps across audio, scripting, entry-point, and platform input systems:
1. Audio null safety:
   - In `OrcEngine/source/Orc/Audio/Audio.cpp`:
     - Check FMOD return values and pointer validity (`bank`, `eventDescription`, `eventInstance`) in `Audio::loadBank` and `Audio::play` in both Debug and Release configurations, logging errors and failing gracefully instead of dereferencing null pointers in Release builds.
2. Portable `errno` error reporting:
   - In `OrcEngine/source/Orc/Engine/Utility.cpp` (`getErrnoMessage`):
     - Replace the hardcoded Linux `errno` switch statement with standard C++ `std::system_category().message(error)`, delivering portable error descriptions across both Linux and Windows.
3. Scripting encapsulation:
   - In `OrcEngine/source/Orc/Script/Scripting.cpp` and `OrcEngine/include/Orc/Script/Scripting.hpp`:
     - Move `hostfxr` function pointers from file-scope globals into private member variables of `Scripting`.
4. EntryPoint contract cleanup:
   - In `OrcEngine/include/Orc/Engine/EntryPoint.hpp` and `OrcEngine/include/Orc/Engine/Engine.hpp`:
     - Keep `getEngineConfig()` and `onEngineStart(Engine&)` exclusively in `EntryPoint.hpp`.
     - Remove duplicate declarations of `getEngineConfig()` and `onEngineStart` from `Engine.hpp`.
     - Remove `int main(int argc, char** argv);` declaration from `EntryPoint.hpp` (main is defined in `EntryPoint.cpp`).
5. Transformable & Keyboard fixes:
   - In `OrcEngine/source/Orc/Graphics/Transformable.cpp`:
     - Change `Transformable::scale` to multiply `m_scale` by the given factors (`m_scale.x *= factor.x`, etc.) rather than adding to it.
   - In `OrcEngine/include/Orc/Input/Keyboard.hpp` and `Keyboard.cpp`:
     - Clean up `Keyboard::Key` enum to remove unmapped shifted symbol keys, keeping clear 1:1 mapping with GLFW key codes.
6. Verify with `./scripts/verify.sh`.

**Blocked by:** 07 (Format Uniform Control Statement Braces and Secure Debug Macros)

- [x] `Audio::loadBank` and `Audio::play` validate pointers and FMOD return codes in all build configurations
- [x] `getErrnoMessage` uses `std::system_category().message(error)` for portable error reporting
- [x] `Scripting` stores `hostfxr` function pointers as private instance members
- [x] `EntryPoint.hpp` owns entry-point contracts without duplication in `Engine.hpp` or header `main` declarations
- [x] `Transformable::scale` multiplies scale factors
- [x] `Keyboard::Key` removes unmapped shifted symbol keys
- [x] Passes `./scripts/verify.sh`

## Answer

Hardened Audio null safety and return codes, portable std::system_category errno reporting, encapsulated Scripting hostfxr function pointers as private instance members, cleaned EntryPoint header contract, multiplied scale factors in Transformable::scale, and pruned unmapped shifted keys in Keyboard::Key
