# OrcEngine Code Hygiene & Quality Issues Inventory

This inventory groups all issues discovered during the engine codebase inspection (both user-reported issues and additional issues/bugs identified during analysis). Each group is formatted to map directly to an individual ticket for execution.

---

## ISSUE_1: Explicit Constructors for Converting and Default-Valued Single-Argument Constructors
Single-argument or multi-argument constructors with default values that are not declared `explicit` allow accidental implicit type conversions (e.g. assigning or passing a raw path or size string to an object expecting a wrapper).

- [OrcEngine/include/Orc/Graphics/Texture.hpp:22](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L22) (`Texture(const FilePath& filePath)`)
- [OrcEngine/include/Orc/Graphics/Texturable.hpp:17](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texturable.hpp#L17) (`Texturable(Ref<Texture> texture)`)
- [OrcEngine/include/Orc/Graphics/Text.hpp:19-20](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Text.hpp#L19-L20) (`Text(const std::string& string)`, `Text(Ref<Font> font)`)
- [OrcEngine/include/Orc/Graphics/Sprite.hpp:17](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Sprite.hpp#L17) (`Sprite(Ref<Texture> texture)`)
- [OrcEngine/include/Orc/Graphics/Font.hpp:24](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Font.hpp#L24) (`Font(const FilePath& filePath, uint32_t size = 48)`)
- [OrcEngine/include/Orc/Graphics/Camera.hpp:11](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Camera.hpp#L11) (`Camera(const FloatRect& viewPort)`)
- [OrcEngine/include/Orc/Graphics/Shapes/Circle.hpp:15](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shapes/Circle.hpp#L15) (`Circle(float radius, const Color& color = Color())`)
- [OrcEngine/include/Orc/Graphics/Shapes/Rectangle.hpp:15](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shapes/Rectangle.hpp#L15) (`Rectangle(const Vector2f& size, const Color& color = Color())`)
- [OrcEngine/include/Orc/Graphics/Animator.hpp:28](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Animator.hpp#L28) (`Animator(Sprite* sprite)`)
- [OrcEngine/include/Orc/Graphics/Window.hpp:30](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Window.hpp#L30) (`Window(const VideoSettings& videoSettings)`)
- [OrcEngine/include/Orc/Audio/Audio.hpp:32](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Audio/Audio.hpp#L32) (`Audio(const AudioSettings& audioSettings)`)
- [OrcEngine/include/Orc/Engine/Engine.hpp:20](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Engine.hpp#L20) (`Engine(const Config& config)`)
- [OrcEngine/include/Orc/Engine/ResourceHolder.hpp:23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/ResourceHolder.hpp#L23) (`Resources(const FilePath& xmlPath)`)
- [OrcEngine/include/Orc/Events/MouseEvents.hpp:12-14](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Events/MouseEvents.hpp#L12-L14) (`MouseButtonPressedEvent(Mouse::Button button)`)
- [OrcEngine/include/Orc/Events/MouseEvents.hpp:21-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Events/MouseEvents.hpp#L21-L23) (`MouseButtonReleasedEvent(Mouse::Button button)`)
- [OrcEngine/include/Orc/Events/KeyboardEvents.hpp:13-15](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Events/KeyboardEvents.hpp#L13-L15) (`KeyboardKeyPressedEvent(Keyboard::Key key, ...)`)
- [OrcEngine/include/Orc/Events/KeyboardEvents.hpp:23-25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Events/KeyboardEvents.hpp#L23-L25) (`KeyboardKeyReleasedEvent(Keyboard::Key key, ...)`)

---

## ISSUE_2: Sinking `Ref<T>` and Passing Patterns (Move Semantics vs Observers)
Functions that take ownership or sink a `Ref<T>` by value should `std::move` into member variables to eliminate redundant atomic reference count increments and decrements. Conversely, methods that merely observe a `Ref<T>` should accept `const Ref<T>&` (or a direct reference) rather than making a copy by value.

- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:20-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L20-L23) (`m_indexBuffer = std::move(indexBuffer)`)
- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:58](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L58) (`m_vertexBuffers.push_back(std::move(vertexBuffer))`)
- [OrcEngine/source/Orc/Graphics/Texturable.cpp:12-17](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texturable.cpp#L12-L17) (`setTexture(std::move(texture))`)
- [OrcEngine/source/Orc/Graphics/Texturable.cpp:19-21](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texturable.cpp#L19-L21) (`m_texture = std::move(texture)`)
- [OrcEngine/source/Orc/Graphics/Text.cpp:11-20](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Text.cpp#L11-L20) (`setFont(std::move(font))`)
- [OrcEngine/source/Orc/Graphics/Text.cpp:22-26](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Text.cpp#L22-L26) (`m_font = std::move(font)`)
- [OrcEngine/source/Orc/Graphics/Sprite.cpp:13-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Sprite.cpp#L13-L23) (`setTexture(std::move(texture))`, delegating constructor)
- [OrcEngine/source/Orc/Graphics/Animator.cpp:13-16](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Animator.cpp#L13-L16) (`m_animations[name] = std::move(animation)`)
- [OrcEngine/source/Orc/Engine/GameLayerManager.cpp:36-45](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/GameLayerManager.cpp#L36-L45) (`m_layers.try_emplace(name, std::move(gameLayer))`)
- [OrcEngine/include/Orc/Graphics/Renderer.hpp:41, 43, 56, 60](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Renderer.hpp#L41) & [OrcEngine/source/Orc/Graphics/Renderer.cpp:123, 203, 214, 267](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Renderer.cpp#L123) (`drawQuad`, `draw`, `getTextureSlot`: change observer parameters from by-value `Ref<T>` to `const Ref<T>&`)

---

## ISSUE_3: Const-Correctness & Subsystem Logical Constness
Non-mutating inspection methods should be marked `const`. Subsystem lifecycle, mutation, and state-advancing methods must remain non-const to preserve logical constness.

- [OrcEngine/include/Orc/Graphics/Texture.hpp:33](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L33) & [OrcEngine/source/Orc/Graphics/Texture.cpp:118-121](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp#L118-L121) (`void bind(uint32_t slot = 0) const;`)
- [OrcEngine/include/Orc/Engine/ResourceHolder.hpp:25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/ResourceHolder.hpp#L25) & [OrcEngine/source/Orc/Engine/ResourceHolder.cpp:18-29](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/ResourceHolder.cpp#L18-L29) (`Ref<ResourceType> getResource(std::string_view name) const;`)
- [OrcEngine/include/Orc/Graphics/Sprite.hpp:25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Sprite.hpp#L25) & [OrcEngine/source/Orc/Graphics/Sprite.cpp:44-47](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Sprite.cpp#L44-L47) (Add missing `const Animator& getAnimator() const;`)
- [OrcEngine/include/Orc/Audio/Audio.hpp:45-47](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Audio/Audio.hpp#L45-L47) & [OrcEngine/source/Orc/Audio/Audio.cpp:126-140](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Audio/Audio.cpp#L126-L140) (Add const overloads `const Bus& getSfxBus() const;`, `const Bus& getMusicBus() const;`, `const Bus& getMasterBus() const;`)
- [OrcEngine/include/Orc/Engine/Engine.hpp:25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Engine.hpp#L25) & [OrcEngine/source/Orc/Engine/Engine.cpp:125-128](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Engine.cpp#L125-L128) (Add `const LayerContext& getLayerContext() const;`)
- **Logical Constness Preservations**:
  - `Window::display()` and `Window::setCallbacks()` remain non-const (mutates window state via event poll and swap buffers).
  - `Gui::begin()` and `Gui::end()` remain non-const (mutates global ImGui state and executes frame rendering).
  - `Audio::play()` and `Audio::update()` remain non-const (spawns audio instances and advances FMOD mixer state).
  - `Bus::stop()`, `pause()`, `resume()`, `setVolume()` remain non-const (mutates bus playback).
  - `VertexBuffer::setData()` remains non-const (mutates GPU buffer contents).

---

## ISSUE_4: Translation Unit Linkage & Anonymous Namespaces
Local translation-unit classes not declared in headers should be scoped in anonymous namespaces to prevent symbol pollution. Functions declared in headers for multi-file consumption must retain external linkage.

- [OrcEngine/source/Orc/Engine/Logger.cpp:15-30](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Logger.cpp#L15-L30) (Move class `CapitalizedLevelNamesFormatter` into an anonymous namespace)
- **External Linkage Preservation**:
  - `fmodCall`, `ftCall`, `getErrnoMessage` in [OrcEngine/source/Orc/Engine/Utility.cpp:17-176](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Utility.cpp#L17-L176) MUST retain external linkage because they are declared in [OrcEngine/include/Orc/Engine/Utility.hpp:25-28](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Utility.hpp#L25-L28) and called across `Bus.cpp`, `Audio.cpp`, `Font.cpp`, and `Shader.cpp`.

---

## ISSUE_5: Namespace Concatenation & Cleanup
Concatenate C++17 nested namespaces and eliminate redundant empty namespace declarations and trailing semicolons.

- [OrcEngine/source/Orc/Input/Mouse.cpp:6-8, 26-27](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Input/Mouse.cpp#L6-L8) (Concatenate to `namespace orc::glfw { ... }`)
- [OrcEngine/source/Orc/Input/Keyboard.cpp:6-8, 173-174](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Input/Keyboard.cpp#L6-L8) (Concatenate to `namespace orc::glfw { ... }`)
- [OrcEngine/include/Orc/Engine/Debug.hpp:10-12](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Debug.hpp#L10-L12) (Remove redundant empty `namespace orc {}`)
- [OrcEngine/include/Orc/Orc.hpp:52-53](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Orc.hpp#L52-L53) (Remove redundant empty `namespace orc {}`)
- [OrcEngine/include/Orc/Graphics/VertexArray.hpp:34](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/VertexArray.hpp#L34) (Remove stray semicolon `};` after namespace closing brace)

---

## ISSUE_6: Control Flow, Unused Parameters, and Switch Completeness
Ensure full branch coverage in switch statements, clean up unused parameters, and modernize normalization loops.

- [OrcEngine/source/Orc/Input/Keyboard.cpp:157-165](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Input/Keyboard.cpp#L157-L165) (Add `default: break;` in `switch (mods & mask)` inside `glfwKeyModsToOrcSpecialKeys`)
- [OrcEngine/source/Orc/Input/Keyboard.cpp:10](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Input/Keyboard.cpp#L10) (Omit or comment unused parameter name in `glfwKeyToOrcKey(int glfwKey, int /*mods*/)`)
- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:27](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L27) (Use `!elements.empty()` instead of `.size()` check in assertion)
- [OrcEngine/source/Orc/Graphics/Transformable.cpp:47-56](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Transformable.cpp#L47-L56) (Replace while-loops with `std::fmod` for rotation angle normalization)

---

## ISSUE_7: Naming & Parameter Consistency Across Declarations and Definitions
Ensure parameter names match identically between `.hpp` headers and `.cpp` implementation definitions.

- [OrcEngine/include/Orc/Graphics/Shader.hpp:45](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shader.hpp#L45) & [OrcEngine/source/Orc/Graphics/Shader.cpp:112](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Shader.cpp#L112) (Synchronize parameter names to `std::string_view vertexSource, std::string_view fragmentSource`)
- [OrcEngine/include/Orc/Graphics/Shader.hpp:24](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shader.hpp#L24) & [OrcEngine/source/Orc/Graphics/Shader.cpp:30](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Shader.cpp#L30) (Synchronize `loadFromString` parameter names to `vertexSource, fragmentSource`)

---

## ISSUE_8: Type Safety, Casting & Narrowing Conversions
Replace C-style casts with C++ explicit casts (`static_cast` / `reinterpret_cast`) and eliminate narrowing conversions.

- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:36](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L36) (Explicitly cast `static_cast<GLsizei>(bufferLayout.getStride())` to prevent narrowing from `uint32_t` to signed `GLsizei`)
- [OrcEngine/source/Orc/Graphics/Texture.cpp:71, 73](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp#L71) (Replace C-style cast `(void*)pixels` with implicit/type-safe conversion)
- [OrcEngine/source/Orc/Graphics/Window.cpp:125](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Window.cpp#L125) (Replace C-style cast `(char*)glGetString(...)` with `reinterpret_cast<const char*>`)
- [OrcEngine/include/Orc/Graphics/Rect.hpp:19](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Rect.hpp#L19) (Replace C-style casts `(T)other.left` with `static_cast<T>`)
- [OrcEngine/include/Orc/Engine/Core.hpp:86](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Core.hpp#L86) (Fix `using Vector4d = glm::tvec3<double>;` -> `glm::tvec4<double>;`)
- [OrcEngine/include/Orc/Graphics/Texture.hpp:31](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L31) (`const TextureMode& textureMode` -> pass `TextureMode` by value per AGENTS.md §5.2)

---

## ISSUE_9: Typographical Errors and Code Smells
Fix spelling errors in log strings and identifier names, remove redundant punctuation.

- [OrcEngine/source/Orc/Graphics/Window.cpp:24](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Window.cpp#L24) (Typo `occured` -> `occurred` in `glfwErrorCallback`)
- [OrcEngine/source/Orc/Graphics/Text.cpp:115, 170](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Text.cpp#L115) (Typo `positionyY` -> `positionY`)
- [OrcEngine/source/Orc/Graphics/Font.cpp:70](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Font.cpp#L70) (Formatting typo `maxDimension= ` missing space before `=`)
- [OrcEngine/include/Orc/Engine/Logger.hpp:29](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Logger.hpp#L29) (Double semicolon `;;` at end of line)

---

## ISSUE_10: Destructor & Polymorphism Specifiers (`override` vs `virtual`)
Ensure derived class virtual destructors explicitly specify `override`.

- [OrcEngine/include/Orc/Graphics/Texturable.hpp:18](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texturable.hpp#L18) (Change `virtual ~Texturable() = default;` to `~Texturable() override = default;`)

---

## ISSUE_11: Logging Macro Standardization (`ORC_ERROR` vs `ORC_LOG_ERROR`)
Standardize all logging invocations on canonical macros defined in `AGENTS.md` §5.4 (`ORC_LOG_INFO`, `ORC_LOG_WARNING`, `ORC_LOG_ERROR`, `ORC_LOG_FATAL`).

- [OrcEngine/source/Orc/Graphics/Texture.cpp:37, 55, 82](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp#L37) (Replace `ORC_ERROR(...)` with `ORC_LOG_ERROR(...)`)
- [OrcEngine/source/Orc/Graphics/Shader.cpp:93, 98, 129, 148, 168](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Shader.cpp#L93) (Replace `ORC_ERROR(...)` with `ORC_LOG_ERROR(...)`)

---

## ISSUE_12: Critical Bugs & Logic Flaws Discovered During Inspection
Fix functional bugs identified in engine subsystems.

- [OrcEngine/source/Orc/Engine/GameLayer.cpp:20](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/GameLayer.cpp#L20) (`setViewportSize` parameter mismatch: was passing `(0.0f, resized->width, resized->height, 0.0f)` into `(left, top, right, bottom)`, putting `width` into `top` and `height` into `right`. Correct call: `(0.0f, 0.0f, static_cast<float>(resized->width), static_cast<float>(resized->height))`)
- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:56](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L56) (Delete duplicate `glVertexArrayVertexBuffer(m_rendererID, 0, vertexBuffer->getRendererID(), 0, bufferLayout.getStride());` which overwrote binding 0)

---

## ISSUE_13: Rule of 5 for OpenGL Resource Wrappers
Bring OpenGL handle-wrapping classes into compliance with `AGENTS.md` §5.3: explicitly define move constructor + move assignment and delete copy operations.

- [OrcEngine/include/Orc/Graphics/VertexBuffer.hpp:24-27](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/VertexBuffer.hpp#L24-L27)
- [OrcEngine/include/Orc/Graphics/IndexBuffer.hpp:24-27](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/IndexBuffer.hpp#L24-L27)
- [OrcEngine/include/Orc/Graphics/Texture.hpp:25-28](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L25-L28)
- [OrcEngine/include/Orc/Graphics/Shader.hpp:18-21](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shader.hpp#L18-L21)

---

## ISSUE_14: Universal Whitespace, Tab Indentation & Convention Linter Alignment
Standardize tab indentation, clean up space indentation, fix trailing whitespace, and update convention linting script.

- [OrcEngine/include/Orc/Audio/Audio.hpp:55](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Audio/Audio.hpp#L55) (Convert 4 leading spaces to tabs)
- [OrcEngine/include/Orc/Engine/Clock.hpp:11-41](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Clock.hpp#L11-L41) (Convert leading spaces to tabs)
- [OrcEngine/include/Orc/Engine/ResourceHolder.hpp:31](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/ResourceHolder.hpp#L31) (Convert leading spaces to tabs)
- [OrcEngine/include/Orc/Engine/Utility.hpp:17-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Utility.hpp#L17-L23) (Convert leading spaces to tabs)
- [OrcEngine/source/Orc/Audio/Bus.cpp:38, 58, 68](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Audio/Bus.cpp#L38) (Convert spaces to tabs, remove space before `if`)
- [OrcEngine/source/Orc/Audio/Audio.cpp:99](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Audio/Audio.cpp#L99) (Convert spaces to tabs)
- [OrcEngine/source/Orc/Engine/EntryPoint.cpp:6-12](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/EntryPoint.cpp#L6-L12) (Convert spaces to tabs)
- [OrcEngine/source/Orc/Engine/Utility.cpp:19-35](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Utility.cpp#L19-L35) (Convert spaces to tabs)
- [OrcEngine/include/Orc/Engine/Config.hpp:31](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Config.hpp#L31) (Remove trailing tab after namespace brace)
- [scripts/lint-conventions.sh:86-91](file:///home/node1/dev/projects/OrcEngine/scripts/lint-conventions.sh#L86-L91) (Allow entry point / stb_image without false positive `namespace orc` error)
