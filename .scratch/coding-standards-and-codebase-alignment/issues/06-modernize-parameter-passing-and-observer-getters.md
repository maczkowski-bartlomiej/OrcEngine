# 06: Modernize C++20 Parameter Passing, Observer Getters, and Constructor Safety

Status: resolved
Type: task
Blocked by: 04, 05

**What to build:** Refactor parameter types, observer return types, and constructor safety across the engine and demo harness:
1. File paths:
   - In `OrcEngine/include/Orc/Engine/Config.hpp`, change string path fields (`assetsPath`, `shadersPath`) from `std::string` to `FilePath`.
   - In `OrcEngine/include/Orc/Engine/Logger.hpp` and `.cpp`, change `Logger::init` path parameter to `const FilePath&`.
2. Read-only strings:
   - Change string parameters in `GameLayerManager` (`getLayer`, `setActiveLayer`, `removeLayer`), `Animator` (`playAnimation`, `addAnimation`), `Text` (`setString`), `Window::setTitle`, and internal `fmodCall` to `std::string_view`.
3. Contiguous memory buffers:
   - In `IndexBuffer` and `VertexBuffer`, change raw pointer + size / count buffer parameters to `std::span<const uint32_t>` and `std::span<const float>` (or `std::span<const Vertex>`), updating `Renderer.cpp` and `VertexArray.cpp` call sites.
   - In `Texture::loadFromMemory`, accept `std::span<const unsigned char>`.
   - In `Shader::uploadUniformIntArray`, accept `std::span<const int>`.
4. Initializer lists and explicit constructors:
   - In `BufferLayout`, pass `std::initializer_list<BufferElement>` by value and mark `BufferLayout` constructors `explicit`.
   - In `Color`, mark single-argument `explicit Color(uint32_t r)` (and other single-argument constructors), updating call sites like `Color(255)` in `TestProject`.
5. Observer getters:
   - Change `GameLayerManager::getActiveLayer` to return `const Ref<GameLayer>&`.
   - Change `Texturable::getTexture` to return `const Ref<Texture>&`.
   - Change `Text::getFont` to return `const Ref<Font>&`.
   - Change `Font::getBitmap` to return `const Ref<Texture>&`.
   - Change `Font::getCharacter` to return `const Character&`.
6. Inheritance and Linkage:
   - Add `override` to derived `GameLayer` destructors in `TestProject` (`Menu`, `AnimationTest`, `AudioTest`, etc.).
   - Scope `.cpp`-local helper classes and formatters in anonymous namespaces.
7. Verify with `./scripts/verify.sh`.

**Blocked by:** 04 (Harden Rule of 5 and Subsystem Ownership Semantics), 05 (Standardize Identifier Naming, Type Aliases, and Glossary Vocabulary)

- [x] Path parameters and fields in `Config` and `Logger::init` use `FilePath` / `const FilePath&`
- [x] Read-only strings in `GameLayerManager`, `Animator`, `Text`, and `Window::setTitle` pass `std::string_view`
- [x] `IndexBuffer`, `VertexBuffer`, `Texture::loadFromMemory`, and `Shader::uploadUniformIntArray` pass `std::span`
- [x] `BufferLayout` accepts `std::initializer_list` by value and has `explicit` constructors
- [x] `Color` single-argument constructors are marked `explicit` and call sites updated
- [x] Observer getters in `GameLayerManager`, `Texturable`, `Text`, and `Font` return `const Ref<T>&` or `const Character&`
- [x] Derived `GameLayer` destructors in `TestProject` are marked `override`
- [x] Translation-unit-local helpers and formatters reside in anonymous namespaces
- [x] Passes `./scripts/verify.sh`

## Answer

Modernized parameter passing across Config, Logger, GameLayerManager, Animator, Text, Window, IndexBuffer, VertexBuffer, Texture, Shader, BufferLayout, Color; updated observer return types in GameLayerManager, Texturable, Text, and Font; added override to TestProject layer destructors; passed 100% verification
