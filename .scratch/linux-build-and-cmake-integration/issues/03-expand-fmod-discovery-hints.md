Status: resolved
Type: task
Blocked by: 02
# 03: Multi-Directory FMOD Discovery Hints

**What to build:** Flexible SDK discovery where CMake automatically locates FMOD Core and Studio from standard Linux and Windows system/user paths without requiring developers to manually define `FMOD_ROOT` across every shell session.

**Blocked by:** 02: Executable Script Permissions and vcpkg Fallback Auto-Detection

**Status:** ready-for-agent

- [ ] CMake FMOD discovery hints include standard Linux directories (`/opt/fmod`, `/usr/local/fmod`, `/usr/local/opt/fmod`, `$ENV{HOME}/fmod`, `$ENV{HOME}/dev/libs/cpp/fmod`)
- [ ] CMake FMOD discovery hints include standard Windows directories (`C:/Program Files/...`, `$ENV{USERPROFILE}/fmod`, etc.)
- [ ] Explicit `FMOD_ROOT` variable retains top search priority
- [ ] Presets configure cleanly on Linux and Windows without mandatory manual environment exports

## Answer

Expanded _fmod_hints in cmake/FindFMOD.cmake to include standard Linux and Windows paths, and added auto-detection in build.sh and check-env.sh.
