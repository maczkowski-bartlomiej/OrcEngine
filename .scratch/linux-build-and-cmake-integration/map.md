# -Title Map

## Notes
Initial exploratory notes.

## Decisions so far
- [04-align-debug-header-conventions](issues/04-align-debug-header-conventions.md): Reformatted Debug.hpp with tab indentation and enclosed in namespace orc, passing conventions linter with 0 errors and 0 warnings.
- [03-expand-fmod-discovery-hints](issues/03-expand-fmod-discovery-hints.md): Expanded _fmod_hints in cmake/FindFMOD.cmake to include standard Linux and Windows paths, and added auto-detection in build.sh and check-env.sh.
- [02-set-script-permissions-and-vcpkg-fallback](issues/02-set-script-permissions-and-vcpkg-fallback.md): Configured executable filemode 100755 in git index for scripts/*.sh and added /home/node1/vcpkg fallback auto-detection to check-env.sh and build.sh.
- [01-fix-variadic-macros](issues/01-fix-variadic-macros.md): Standardized logging and assertion macros with __VA_OPT__(,) and added /Zc:preprocessor flag for MSVC.

## Fog
Unknowns and risks to explore.
