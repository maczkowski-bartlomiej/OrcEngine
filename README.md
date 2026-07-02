# OrcEngine

OrcEngine is a C++20 game engine project with a demo application in `TestProject`.

## Engine capabilities
- Logging
- Batch rendering of: sprites, text, basic shapes
- FMOD integration

## Planned features
- C# Scripting
- Sprite animations

## Requirements

Set variable `VCPKG_ROOT` to vcpkg directory.


Set vaiarble `FMOD_ROOT` to fmod directory.
```text
FMOD/api/core/inc/fmod.hpp
FMOD/api/core/inc/fmod_common.h
FMOD/api/studio/inc/fmod_studio.hpp
FMOD/api/studio/inc/fmod_studio_common.h
FMOD/api/core/lib/x86_64/libfmod.so
FMOD/api/studio/lib/x86_64/libfmodstudio.so
```

## Dependencies

Open-source dependencies are managed by `vcpkg.json`:

- GLFW
- GLAD
- GLM
- spdlog
- tinyxml2
- stb
- FreeType
- ImGui with GLFW and OpenGL3 bindings

FMOD is handled separately through `FMOD_ROOT` because it is a manually installed SDK.

## Configure

From the repository root:

```bash
cmake --preset linux-debug
cmake --build --preset linux-debug
```

Release build:

```bash
cmake --preset linux-release
cmake --build --preset linux-release
```

## Run

The demo executable is `TestProject`.

```bash
cd build/linux-debug
./TestProject
```
