# 03: Decouple Public Headers from Third-Party SDKs via Forward Declarations

Status: resolved
Type: task
Blocked by: 02

**What to build:** Protect engine consumers from third-party SDK header leakage:
1. In `OrcEngine/include/Orc/Audio/Audio.hpp` and `OrcEngine/include/Orc/Audio/Bus.hpp`:
   - Remove `#include <fmod_studio.hpp>`.
   - Forward-declare `namespace FMOD { class System; namespace Studio { class System; class Bank; class Bus; class EventInstance; } }`.
   - Include `<fmod_studio.hpp>` only in private implementation files (`Audio.cpp`, `Bus.cpp`).
2. In `OrcEngine/include/Orc/Engine/Utility.hpp`:
   - Remove `#include <fmod_common.h>` and `#include <freetype/freetype.h>`.
   - Relocate `fmodCall` and `ftCall` declarations out of the public engine header into internal headers (or internal private scope) so that consumer translation units do not pull in FMOD or FreeType headers.
3. Ensure all headers using `std::source_location` include `<source_location>` directly.
4. Verify that including the umbrella header `OrcEngine/include/Orc/Orc.hpp` in a consumer project introduces zero third-party FMOD or FreeType type leaks.
5. Verify with `./scripts/verify.sh`.

**Blocked by:** 02 (Optimize Precompiled Header and Eliminate Redundant Manual Includes)

- [x] `Audio.hpp` forward-declares `FMOD::Studio::System`, `FMOD::Studio::Bank`, and `FMOD::Studio::Bus` without including `<fmod_studio.hpp>`
- [x] `Bus.hpp` forward-declares `FMOD::Studio::Bus` without including `<fmod_studio.hpp>`
- [x] `Utility.hpp` removes `<fmod_common.h>` and `<freetype/*.h>` headers
- [x] `fmodCall` and `ftCall` are relocated out of public API headers into internal engine scope
- [x] Missing `<source_location>` includes are added where used
- [x] Public headers compile cleanly without leaking external SDK types
- [x] Passes `./scripts/verify.sh`

## Answer

Decoupled public headers from third-party SDKs by forward declaring FMOD Studio types in Audio.hpp and Bus.hpp, moving fmodCall and ftCall to internal AudioUtility.hpp and FontUtility.hpp headers, and removing FMOD/FreeType includes from Utility.hpp.
