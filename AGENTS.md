# AGENTS.md

> [!IMPORTANT]
> ### Mandatory Agent Invariants
> Every agent session and subagent working in this repository **must** strictly abide by the following invariants:
> 1. **Read Core Guidance First**: On session startup, read [`AGENTS.md`](AGENTS.md) and [`GLOSSARY.md`](GLOSSARY.md) before executing any task. Read [`CODING_STANDARDS.md`](CODING_STANDARDS.md) before writing or reviewing code.
> 2. **Consult & Propose First**: Never make unilateral architectural, library, or design assumptions. When multiple approaches exist or requirements are ambiguous, present options with trade-offs and **ask the user** before writing code. Work incrementally, keep changes tightly scoped, and aggressively delete dead or obsolete code ("no legacy cruft").
> 3. **Script-First Toolchain & Unified Verification Gate**: Always build via `.\scripts\build.ps1` (Windows) or `./scripts/build.sh` (Linux). Before completing an implementation turn, resolving a ticket, or committing work, run `.\scripts\verify.ps1` or `./scripts/verify.sh` (conventions lint, build with zero warnings/errors, and 5-second `TestProject` smoke test). All stages must pass 100%.
> 4. **Synchronize Documentation as Ticket Condition**: Any change affecting ubiquitous language, subsystem architecture, or design trade-offs **must** update [`GLOSSARY.md`](GLOSSARY.md), record or update an ADR in `docs/adr/`, and keep [`AGENTS.md`](AGENTS.md) / [`CODING_STANDARDS.md`](CODING_STANDARDS.md) in sync before a ticket is resolved.

---

## 1. Project Overview, Layout & Direction

OrcEngine is a modular 2D game engine built in C++20 using OpenGL 4.6 (Core Profile), GLFW, GLAD, FreeType, FMOD Studio, spdlog, GLM, and .NET 10 CoreCLR (`hostfxr`).

### Repository Layout
- **Public Engine Headers**: `OrcEngine/include/Orc/` (umbrella header: `OrcEngine/include/Orc/Orc.hpp`)
- **Engine Source**: `OrcEngine/source/Orc/` (precompiled header: `OrcEngine/include/Orc/OrcPch.hpp`)
- **Managed C# Scripting**: `OrcEngine.Scripting/` (`OrcEngine.Scripting.csproj`, compiled into the executable directory)
- **Demo & Integration Harness**: `TestProject/include/`, `TestProject/source/`, `TestProject/assets/`, `TestProject/shaders/`
- **Standards & Domain Docs**: [`GLOSSARY.md`](GLOSSARY.md) (ubiquitous language), [`CODING_STANDARDS.md`](CODING_STANDARDS.md) (C++20/C# rules), `docs/adr/` (architecture decisions), `docs/agents/` (agent workflow protocols)
- **Local Issue Tracker**: `.scratch/` (managed via `scripts/ticket.ps1` / `scripts/ticket.sh`)
- **Build & Automation**: `scripts/`, `CMakeLists.txt`, `CMakePresets.json`, `cmake/`, `vcpkg.json`

### Project Trajectory
`Refactor architecture` $\rightarrow$ `Refactor codebase (especially rendering)` $\rightarrow$ `Add C# scripting` $\rightarrow$ `Build and release a game in a separate repository, letting real game requirements drive engine growth`. Keep reusable engine features in `OrcEngine` and validation/demos in `TestProject`.

---

## 2. Subsystem Architecture & Lifecycle

### Initialization Order (`Engine::Engine`)
`Engine` constructs and initializes subsystems in strict order:
1. `Logger::init` (spdlog console + file logging)
2. `Scripting` (.NET 10 CoreCLR runtime via `hostfxr`, assembly loading, and `[UnmanagedCallersOnly]` delegate binding)
3. `GameLayerManager` (layer registry and active layer pointer)
4. `Input` (tracks keyboard, mouse buttons, cursor deltas, scroll wheel deltas, and modifier state)
5. `Window` (GLFW window, OpenGL 4.6 context, GLAD loader, event callbacks, direct `Input` feeding)
6. `Renderer` (dynamic quad batcher, vertex/index buffers, composite shader, initial `setViewport`)
7. `Gui` (ImGui GLFW + OpenGL 3 backend)
8. `Audio` (FMOD Studio system, `bus:/`, `bus:/Music`, `bus:/SFX`, and bank pre-loading)
9. `Resources` (`FontResources`, `TextureResources`, `AnimationResources` from XML manifests)
10. `LayerContext` (bundles subsystem references for constructor injection into `GameLayer` instances)

### Frame Loop (`Engine::run`)
1. `Clock::restart()` $\rightarrow$ delta time (`elapsed`)
2. `Renderer::clear()`
3. `GameLayerManager::getActiveLayer()`
4. `activeLayer->onUpdate(elapsed)`
5. `activeLayer->onRender()` (bounded by `renderer.beginScene(m_camera)` ... `renderer.endScene()`)
6. `Gui::begin()` $\rightarrow$ `activeLayer->onGuiRender()` $\rightarrow$ `Gui::end()`
7. `Audio::update()`
8. `Window::display()` (`glfwPollEvents()` & `glfwSwapBuffers()`)

### Subsystem Context Pointers (Read Before Modifying a Subsystem)
Always check existing ADRs before proposing architectural changes, and flag any contradiction explicitly:
- **Core Primitives (`Orc/Core/`)**: Layer 0 platform macros, `Ref<T>`/`UniquePtr<T>`, math/path aliases, and version constants $\rightarrow$ [ADR 0006](docs/adr/0006-modularize-core-primitives.md).
- **2D Rendering & Quad Batcher (`Orc/Graphics/`)**: Unified `orc::Vertex`, non-templated `Texturable`, 32-slot texture batching, lines as oriented quads, and natural submission order $\rightarrow$ [ADR 0001](docs/adr/0001-collapse-2d-render-submission-pipeline.md), [ADR 0002](docs/adr/0002-unify-vertex-and-untemplate-texturable.md).
- **Font Lifecycle & Platform SDKs (`Orc/Graphics/Font.hpp`)**: ISO C++20 `__VA_OPT__` macros and multi-tier SDK discovery $\rightarrow$ [ADR 0003](docs/adr/0003-cross-platform-preprocessor-and-multi-tier-sdk-discovery.md).
- **Game Layers & `LayerContext` (`Orc/Engine/`)**: Constructor-injected `LayerContext&` and singleton-free layer lifecycle $\rightarrow$ [ADR 0004](docs/adr/0004-decouple-layer-runtime-from-engine-singleton.md).
- **Events & Input (`Orc/Events/`, `Orc/Input/Input.hpp`)**: Deepened frame-snapshot `Input`, direct `Window` feeding, and pure-enum `Event` lifecycle $\rightarrow$ [ADR 0010](docs/adr/0010-deepen-input-and-collapse-event-to-pure-enum.md).
- **Viewport & Camera Sync (`Orc/Graphics/`, `Orc/Graphics/Window.hpp`)**: Authoritative `Renderer::setViewport` from `Window::getFramebufferSize()` and automatic camera viewport synchronization on resize/layer switch $\rightarrow$ [ADR 0007](docs/adr/0007-centralize-viewport-and-synchronize-camera.md).
- **C# Scripting (`Orc/Script/`, `OrcEngine.Scripting/`)**: .NET 10 CoreCLR hosting via `nethost`/`hostfxr`, direct class layout without PIMPL, and unconditional `ORC_FATAL_CHECK` fail-fast semantics $\rightarrow$ [ADR 0008](docs/adr/0008-hostfxr-dotnet-coreclr-scripting.md).
- **Universal Fail-Fast Error Handling (`Orc/Engine/Debug.hpp`, `Orc/Audio/AudioUtility.hpp`, `Orc/Graphics/FontUtility.hpp`)**: Unconditional `ORC_FATAL_CHECK` / `ORC_FATAL` (`std::abort()` in Debug and Release), `ORC_FMOD_CALL` / `ORC_FT_CALL` macro wrappers, and zero `bool`/`nullptr` error returns or defensive half-initialized state guards $\rightarrow$ [ADR 0009](docs/adr/0009-universal-fail-fast-error-handling.md).

---

## 3. Coding Standards & Conventions

Read [`CODING_STANDARDS.md`](CODING_STANDARDS.md) when writing, refactoring, or reviewing C++ or C# code. Key invariants:
- **Formatting**: Tabs for indentation, Allman braces (new line), `#pragma once` in all headers, automatic CMake `target_precompile_headers` injection of `OrcPch.hpp` without manual includes in engine `.cpp` files, and new public headers added to `OrcEngine/include/Orc/Orc.hpp`.
- **Naming**: `orc` lowercase namespace, `PascalCase` types/enums/files, `camelCase` methods/locals, `m_camelCase` member variables, `UPPER_SNAKE_CASE` constants/macros.
- **Signatures & Safety**: `explicit` single-arg constructors, `std::string_view` for read-only strings, `const FilePath&` for paths, `Ref<T>` by value + `std::move` for sinks and `const Ref<T>&` for observers, logical `const` correctness, anonymous namespaces for `.cpp`-local helpers, unconditional fail-fast checks (`ORC_FATAL_CHECK`, `ORC_FATAL`, `ORC_FMOD_CALL`, `ORC_FT_CALL`), and zero compiler warnings.

---

## 4. Build, Verification & Issue Tracking

| Task | Windows (PowerShell) | Linux (Bash) |
| :--- | :--- | :--- |
| **Unified Verify Gate** | `.\scripts\verify.ps1` | `./scripts/verify.sh` |
| **Build (Debug)** | `.\scripts\build.ps1` | `./scripts/build.sh` |
| **Build (Release)** | `.\scripts\build.ps1 -Preset windows-release` | `./scripts/build.sh --preset linux-release` |
| **Lint Conventions** | `.\scripts\lint-conventions.ps1 -ChangedOnly` | `./scripts/lint-conventions.sh --changed-only` |
| **Run Demo Smoke Test** | `.\scripts\run-demo.ps1 -TimeoutSeconds 5 -FailOnErrorLog` | `./scripts/run-demo.sh --timeout 5 --fail-on-error-log` |
| **Check Environment** | `.\scripts\check-env.ps1` | `./scripts/check-env.sh` |
| **Ticket CLI** | `.\scripts\ticket.ps1 <list\|frontier\|claim\|resolve\|triage>` | `./scripts/ticket.sh <list\|frontier\|claim\|resolve\|triage>` |

- **Dependencies & CMake**: Open-source packages live in `vcpkg.json`; FMOD discovery lives in `cmake/FindFMOD.cmake`; .NET hosting discovery lives in `CMakeLists.txt`. Keep implementation-only libraries `PRIVATE` on `OrcEngine`. Do not commit `build/`, `vcpkg_installed/`, or `CMakeUserPresets.json`.
- **Issue Tracking & Wayfinding**: When working with `.scratch/` specs, maps, or tickets, read [`docs/agents/issue-tracker.md`](docs/agents/issue-tracker.md) and [`docs/agents/triage-labels.md`](docs/agents/triage-labels.md).
- **Domain Modeling**: When introducing or updating domain concepts or ADRs, read [`docs/agents/domain.md`](docs/agents/domain.md).
