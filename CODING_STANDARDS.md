# Coding Standards

Authoritative C++20 and C# coding conventions for `OrcEngine`, `OrcEngine.Scripting`, and `TestProject`. All code must compile with **zero warnings** (`MSVC /W4 /wd4100 /Zc:preprocessor`, `GCC/Clang -Wall -Wextra -Wno-unused-parameter`) and pass `scripts/lint-conventions`.

---

## 1. Naming Conventions

| Category | Convention | Examples |
| :--- | :--- | :--- |
| **Namespaces** | `lowercase_snake_case` | `orc`, `orc::version`, `orc::glfw` |
| **Types / Classes / Structs** | `PascalCase` | `GameLayer`, `WindowResizedEvent`, `Vertex`, `AudioSettings` |
| **Functions / Methods** | `camelCase` | `loadFromFile()`, `isKeyPressed()`, `onAttach()`, `setViewportSize()` |
| **Local Variables & Parameters** | `camelCase` | `elapsed`, `windowResizedEvent`, `gameLayer`, `dt` |
| **Member Variables** | `m_camelCase` | `m_running`, `m_videoSettings`, `m_glfwWindow`, `m_texture`, `m_camera` |
| **Constants & Macros** | `UPPER_SNAKE_CASE` | `MAX_TEXTURE_SLOTS`, `ORC_LOG_INFO`, `ORC_ASSERT`, `ORC_DEBUG` |
| **Enum Classes & Values** | `PascalCase` | `enum class Key { Space, Escape, LeftShift };` |
| **File Names** | `PascalCase` | `GameLayer.hpp`, `Renderer.cpp`, `Transformable.hpp` |

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

---

## 3. Constructors, Constness, Linkage & Type Safety

- **Explicit Constructors**: Mark all single-argument constructors (and multi-argument constructors where trailing parameters have defaults) `explicit` across classes and event structs, except intentional type-erasure wrappers.
- **Logical Constness**: Methods that mutate subsystem state, poll OS events, swap buffers, tick timers, advance audio mixers, or upload GPU buffer data must be non-`const`, even if internal handles are pointers (`GLFWwindow*`, `FMOD::Studio::System*`) or driver IDs (`RendererID`). Mark pure state inspectors (`Texture::bind()`, `Resources::getResource()`, accessors) `const`.
- **Linkage & Header Hygiene**:
  - Enclose `.cpp`-local helper classes, functions, and formatters in an anonymous namespace.
  - Keep cross-module utilities declared in public headers (`fmodCall`, `ftCall`, `getErrnoMessage`) under `namespace orc`.
  - Keep public headers (`OrcEngine/include/Orc/`) free of native OS headers (`Windows.h`, `dlfcn.h`) and third-party hosting headers (`nethost.h`, `hostfxr.h`, `coreclr_delegates.h`).
- **Type Casts**: Use `static_cast` for value/arithmetic conversions (including narrowing conversions such as `uint32_t` to `GLsizei`) and `reinterpret_cast` for opaque pointer reinterpretation. Never use C-style casts.
- **Modern C++20 Syntax**:
  - Use nested namespace declarations (`namespace orc::glfw { ... }`) without trailing semicolons or empty namespace stubs.
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

## 5. Error Handling, Assertions & Logging

- **Logging (`Orc/Engine/Logger.hpp`)**: Use ISO C++20 `__VA_OPT__` spdlog macros:
  - `ORC_LOG_INFO(message, ...)` — normal diagnostic events.
  - `ORC_LOG_WARNING(message, ...)` — recoverable unexpected states or fallbacks.
  - `ORC_LOG_ERROR(message, ...)` — recoverable operation failures (e.g., missing asset file).
  - `ORC_LOG_FATAL(message, ...)` — unrecoverable critical errors.
- **Debug Assertions (`Orc/Engine/Debug.hpp`)**: Use `ORC_ASSERT(condition, message, ...)` for debug-only invariants (`ORC_DEBUG_ASSERTS`).
- **Unconditional Fail-Fast (`Orc/Engine/Debug.hpp`)**: Use `ORC_FATAL(message, ...)` and `ORC_FATAL_CHECK(condition, message, ...)` for unrecoverable bootstrap/hosting invariants (such as .NET CoreCLR `hostfxr` initialization); these log, break in debuggers, and call `std::abort()` in both Debug and Release builds.
- **Recoverable Runtime Failures**: Return `false`, `nullptr`, or `std::nullopt` for non-fatal asset/runtime errors.

---

## 6. Formatting & File Layout

- **Indentation**: Tabs (enforced by `scripts/lint-conventions`).
- **Braces**: Allman style (opening brace on a new line) for namespaces, classes, functions, and control blocks.
- **Header Guards**: `#pragma once` at the top of every `.hpp` and `.inl` file.
- **Precompiled Header**: Engine `.cpp` files under `OrcEngine/source/Orc/` begin with `#include "OrcPch.hpp"`.
- **Umbrella Header**: Expose new public engine headers in `OrcEngine/include/Orc/Orc.hpp`.
- **File Extensions**: `.hpp` for headers, `.cpp` for implementations, `.inl` for template/inline definitions.
- **No Legacy Cruft**: Aggressively remove dead, obsolete, or unused methods, typedefs, and structs. Update `TestProject` in the same change when modifying engine APIs.
