# Coding Standards

Authoritative C++20 coding conventions for `OrcEngine` and `TestProject`. (C# scripting conventions for `OrcEngine.Scripting` will be formally defined when scripting expands beyond the bootstrap harness). All code must compile with **zero warnings** (`MSVC /W4 /wd4100 /Zc:preprocessor`, `GCC/Clang -Wall -Wextra -Wno-unused-parameter`) and pass `scripts/lint-conventions`.

---

# C++20 Coding Standards

## 1. Naming Conventions

| Category | Convention | Examples |
| :--- | :--- | :--- |
| **Namespaces** | `lowercase_snake_case` | `orc`, `orc::version`, `orc::glfw` |
| **Types / Classes / Structs** | `PascalCase` | `GameLayer`, `VideoSettings`, `Vertex`, `AudioSettings` |
| **Functions / Methods** | `camelCase` | `loadFromFile()`, `isKeyPressed()`, `onAttach()`, `setViewportSize()` |
| **Local Variables & Parameters** | `camelCase` | `elapsed`, `layerContext`, `gameLayer`, `dt` |
| **Class Member Variables** | `m_camelCase` | `m_running`, `m_videoSettings`, `m_glfwWindow`, `m_texture`, `m_camera` |
| **Struct Fields (Data)** | `camelCase` | `position`, `color`, `width`, `height`, `assetsPath`, `event` |
| **Constants & Macros** | `UPPER_SNAKE_CASE` | `MAX_TEXTURE_SLOTS`, `ORC_LOG_INFO`, `ORC_FATAL_CHECK`, `ORC_DEBUG` |
| **Enum Classes & Values** | `PascalCase` | `enum class Key { Space, Escape, LeftShift };` |
| **File Names** | `PascalCase` | `GameLayer.hpp`, `Renderer.cpp`, `Transformable.hpp` |

Private and protected class members use the `m_camelCase` prefix. Public fields of plain data structs (`Config`, `LayerContext`, `Vertex`, `Color`, `Rect`, `AudioSettings`, `VideoSettings`, and `SpecialKeys`) use unprefixed `camelCase`.

Use domain terms strictly as defined in [`GLOSSARY.md`](GLOSSARY.md) and avoid listed synonyms.

---

## 2. Parameter & Argument Passing

- **Strings (Read-Only)**: Pass `std::string_view` for lookups, non-owning inspection, and logging. Use `const std::string&` only when null-terminated C-strings (`.c_str()`) or C APIs (GLFW, GLAD, FMOD) require it.
- **Paths**: Pass `const FilePath&` (`const std::filesystem::path&`).
- **Buffers & Spans**: Pass contiguous memory via `std::span<const T>` or `std::span<const std::byte>` instead of raw pointer + size pairs.
- **Math & Compound Types**: Pass `Vector2f`, `Vector3f`, `Vector4f`, `Matrix`, `Matrix4`, `Color`, and `FloatRect` by `const&`.
- **Primitives & Enums**: Pass `float`, `uint32_t`, `int`, `bool`, `size_t`, small enums, and `std::initializer_list<T>` by value.
- **Smart Pointers (`Ref<T>`)**:
  - **Sinks (Takes Ownership)**: Take `Ref<T>` by value and `std::move(...)` into the destination member or container.
  - **Observers (No Ownership Transfer)**: Pass `const Ref<T>&` (or `const T&`) when inspecting, binding, or drawing without retaining ownership.
  - **Observer Getters**: Methods returning non-owning references to shared engine resources (`Ref<T>`) must return `const Ref<T>&` (e.g. `const Ref<Texture>& getTexture() const`, `const Ref<ResourceType>& getResource(std::string_view name) const`) rather than returning `Ref<T>` by value, preventing redundant atomic reference-count increments and decrements in render loops and per-frame queries.

---

## 3. Constructors, Constness, Linkage & Type Safety

- **Explicit Constructors**: Mark all single-argument constructors (and multi-argument constructors where trailing parameters have defaults) `explicit` across classes and structs, except intentional type-erasure wrappers.
- **In-Class Member Initialization**: Use `=` assignment syntax for default member initializers (e.g., `void* m_hostfxrLib = nullptr;`, `bool m_running = false;`, `uint32_t m_rendererID = 0;`) rather than brace initialization (`{nullptr}`, `{false}`, `{0}`).
- **Logical Constness**: Methods that advance simulation, poll OS events, swap buffers, tick timers, advance audio mixers, or upload GPU buffer data must be non-`const`, even if internal handles are pointers (`GLFWwindow*`, `FMOD::Studio::System*`) or driver IDs (`RendererID`). Conversely, OpenGL bind and uniform upload methods (`Texture::bind()`, `Shader::uploadUniform...`) and state inspectors are marked `const` because they do not mutate the logical state of the C++ asset object.
- **Linkage & Header Hygiene**:
  - Enclose `.cpp`-local helper classes, functions, and formatters in an anonymous namespace.
  - Keep foundational utilities declared in public headers (`StringViewHash`) under `namespace orc`.
  - Keep public headers (`OrcEngine/include/Orc/`) free of third-party SDK headers (such as FMOD `<fmod_studio.hpp>`, `<fmod_common.h>`, FreeType `<freetype/*.h>`, native OS headers `<Windows.h>`, `<dlfcn.h>`, and .NET hosting headers `<nethost.h>`, `<hostfxr.h>`, `<coreclr_delegates.h>`). Forward-declare third-party types in public class headers and isolate SDK-specific helper macros (`ORC_FMOD_CALL`, `ORC_FT_CALL`) in internal engine headers or `.cpp` implementation files.
- **Type Casts**: Use `static_cast` for value/arithmetic conversions (including narrowing conversions such as `uint32_t` to `GLsizei`) and `reinterpret_cast` for opaque pointer reinterpretation. Never use C-style casts.
- **Modern C++20 Syntax**:
  - Use nested namespace declarations (`namespace orc::glfw { ... }`) without trailing semicolons, empty namespace stubs, or `// namespace ...` closing comments (`}` only).
  - Provide an explicit `default: break;` in `switch` statements over bitmasks or integral states.
  - Mark derived destructors overriding a virtual base destructor with `override`.
  - Use `!container.empty()` instead of `.size()` in boolean conditions.

---

## 4. Memory Ownership & RAII

- **Smart Pointer Aliases (`Orc/Core/Memory.hpp`)**:
  - `orc::Ref<T>` (`std::shared_ptr<T>`) via `orc::createRef<T>(...)` for shared assets (`Texture`, `Font`, `Shader`, `GameLayer`).
  - `orc::UniquePtr<T>` (`std::unique_ptr<T>`) via `orc::createUniquePtr<T>(...)` for single-owner subsystems (`Renderer`, `Window`, `Audio`, `GameLayerManager`).
- **Rule of 5**: Classes owning raw driver/OS handles (OpenGL IDs, GLFW handles, FreeType faces, FMOD objects, dynamic library handles) must delete copy operations and either define move constructor/assignment (zeroing moved-from handles) or delete move operations as appropriate.
- **Destructor Teardown**: Let RAII or `m_ptr.reset()` clean up owned resources; never call `m_ptr.release()` in destructors.

---

## 5. Error Handling, Fail-Fast Contract & Logging

- **Logging (`Orc/Core/Logger.hpp`)**: Use ISO C++20 `__VA_OPT__` spdlog macros:
  - `ORC_LOG_INFO(message, ...)` — normal diagnostic events.
  - `ORC_LOG_WARNING(message, ...)` — benign, non-fatal runtime notices where the engine remains in a valid state (e.g., requesting a switch to the already-active `GameLayer` or falling back on a non-ASCII character code in `Font`).
  - `ORC_LOG_FATAL(message, ...)` — unrecoverable critical errors (invoked via `ORC_FATAL` and `ORC_FATAL_CHECK`). Standalone `ORC_LOG_ERROR` is not used.
- **Universal Fail-Fast Contract (`Orc/Core/Debug.hpp`)**: Whenever the engine enters an invalid state—across subsystem/SDK bootstrap, asset loading and manifest parsing, resource lookups, or API preconditions—log the fatal error and terminate immediately:
  - Use `ORC_FATAL_CHECK(check, message, ...)` and `ORC_FATAL(message, ...)` across all subsystems. Both macros log via `ORC_LOG_FATAL`, invoke `ORC_DEBUGBREAK()` in Debug builds (`ORC_DEBUG_ASSERTS`), and call `std::abort()` in both Debug and Release builds.
  - Use `ORC_FMOD_CALL(call, message, ...)` (`Audio/AudioUtility.hpp`) and `ORC_FT_CALL(call, message, ...)` (`Graphics/Font.cpp`) for FMOD and FreeType SDK calls; both delegate directly to `ORC_FATAL_CHECK` and accept variadic format arguments natively without `fmt::format` boilerplate.
  - **Assume Valid State**: Never return `false` or `nullptr` to signal an error, never add defensive "half-initialized state" guards (such as `if (!m_system)`), and omit redundant post-call out-pointer checks when the SDK return code already guarantees validity.

---

## 6. Formatting & File Layout

- **Indentation**: Tabs (enforced by `scripts/lint-conventions`).
- **Line Wrapping**: Keep statements, function calls, casts, and logging/fatal-check macro invocations (`ORC_LOG_*`, `ORC_FATAL`, `ORC_FATAL_CHECK`) on a single line (`ColumnLimit: 0` in `.clang-format`). Do not artificially wrap arguments across multiple lines. For SDK call wrappers (`ORC_FMOD_CALL`, `ORC_FT_CALL`), wrap arguments across two lines: the first line for the SDK call and the second line for the error message (including any variadic format arguments):
  ```cpp
  ORC_FMOD_CALL(
  	FMOD::Studio::System::create(&m_system),
  	"Failed to create FMOD system.");
  ```
- **Braces**: Allman style (opening brace on a new line) is mandatory for classes, structs, enums, functions, and all control statements (`if`, `else`, `for`, `while`, and `do`, including single-statement bodies). `namespace` declarations and lambdas place opening braces on the same line (`namespace orc {`, `[]() {`). `switch` `case` labels do not use block braces unless required for variable scoping.
- **Header Guards**: `#pragma once` at the top of every `.hpp` and `.inl` file.
- **Precompiled Header**: `OrcPch.hpp` is automatically force-included by CMake (`target_precompile_headers`) across all engine translation units; do not manually `#include "OrcPch.hpp"` in `.cpp` files.
- **Umbrella Header**: Expose new public engine headers in `OrcEngine/include/Orc/Orc.hpp`.
- **File Extensions**: `.hpp` for headers, `.cpp` for implementations, `.inl` for template/inline definitions.
- **No Legacy Cruft**: Aggressively remove dead, obsolete, or unused methods, typedefs, and structs. Update `TestProject` in the same change when modifying engine APIs.
