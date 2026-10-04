# AGENTS.md

## Project overview

OrcEngine is a C++20 game engine built as a static library. `TestProject` is the demo and the main integration harness for engine changes.

- Public engine headers: `OrcEngine/include/Orc/`
- Engine implementations: `OrcEngine/source/Orc/`
- Demo headers and sources: `TestProject/include/` and `TestProject/source/`
- Demo runtime assets: `TestProject/assets/` and `TestProject/shaders/`
- Build configuration: `CMakeLists.txt`, `CMakePresets.json`, `cmake/`, and `vcpkg.json`

Keep reusable functionality in `OrcEngine`; use `TestProject` for examples, experiments, and integration coverage.

## Build prerequisites

The supported presets require CMake 3.25 or newer, Ninja, a C++20 compiler, the Linux system packages listed in `README.md`, and two paths:

- `VCPKG_ROOT` (environment variable) points to a vcpkg checkout. Open-source dependencies come from `vcpkg.json`.
- `FMOD_ROOT` (environment variable or CMake cache variable) points to a manually installed FMOD SDK with the layout documented in `README.md`.

Do not commit machine-specific paths, generated build trees, `vcpkg_installed/`, or `CMakeUserPresets.json`.

## Build and run

Run commands from the repository root.

```bash
cmake --preset linux-debug
cmake --build --preset linux-debug
```

For an optimized build:

```bash
cmake --preset linux-release
cmake --build --preset linux-release
```

Run the demo from its build directory so relative asset paths resolve:

```bash
cd build/linux-debug
./TestProject
```

To compile only the engine, configure with `-DORC_BUILD_TEST_PROJECT=OFF` in a separate build directory or local user preset. Keep local preset files untracked.

## Validation

There is currently no automated test target. For code changes:

1. Configure and build `linux-debug`.
2. Run `TestProject` when the environment supports graphics and audio.
3. Exercise the affected demo screen and watch the console or `logs/TestProject.log` for errors.
4. Also build `linux-release` for changes involving configuration-specific code, optimization-sensitive behavior, or packaging.

If FMOD, display, or audio access prevents runtime validation, report that limitation and still perform the strongest available compile check.

## C++ conventions

Follow the style of the surrounding file; the repository has no enforced formatter.

- Use C++20 and keep compiler extensions disabled.
- Put engine code in the `orc` namespace.
- Use PascalCase for types and camelCase for functions, local variables, and data members.
- Use `.hpp` for headers, `.cpp` for implementations, and `.inl` for template or inline implementation files.
- Start headers with `#pragma once`.
- Prefer existing engine ownership aliases and helpers, such as `orc::Ref` and `orc::createRef`, over introducing a parallel ownership pattern.
- Use the existing `ORC_LOG_*` macros for engine and demo logging.
- Preserve the tab-based indentation and brace layout used by nearby code.
- Keep public includes reachable through `OrcEngine/include/Orc/Orc.hpp` when they are intended to be part of the umbrella API.

Avoid broad cleanup or formatting changes in files touched for a focused fix.

## Dependencies and build files

Add open-source packages to `vcpkg.json` and declare their CMake targets in `CMakeLists.txt`. Keep FMOD discovery in `cmake/FindFMOD.cmake` because FMOD is not managed through vcpkg.

The source lists use `GLOB_RECURSE` with `CONFIGURE_DEPENDS`, so new files under the existing engine and demo source trees are picked up automatically. Update CMake explicitly when introducing a new source root, generated source, dependency, compile definition, or resource-copy step.

Dependencies exposed in public engine headers belong in `OrcEngine`'s `PUBLIC` link interface. Implementation-only dependencies should remain `PRIVATE`.

## Assets and generated files

`TestProject/assets/` and `TestProject/shaders/` are copied next to the demo executable after each build, together with the FMOD runtime libraries. Keep asset manifest paths relative and verify renamed or added files in a clean build directory. The renderer loads its shaders from `shaders/` relative to the working directory; update all references required by the runtime loader when moving them.

Do not edit or commit generated build output. Treat checked-in DLLs and other binary SDK artifacts as intentional legacy files; replace them only when the task explicitly requires a binary update and document their source/version.

## Change discipline

- Keep engine API changes backward-compatible where practical and update `TestProject` usages in the same change.
- Check event, renderer, window, and audio lifetime assumptions before retaining references across frames or layer transitions.
- Add a focused demo path for new visible engine behavior when no automated test can cover it.
- Update `README.md` when prerequisites, build commands, runtime steps, or advertised capabilities change.
