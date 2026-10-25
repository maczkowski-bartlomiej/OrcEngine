# 02: Enforce Fail-Fast Crash Contract in Graphics, Assets, and Resource Management

Status: resolved
Type: task
Blocked by: 01

**What to build:** Enforce the universal fail-fast crash contract (`ORC_FATAL_CHECK`, `ORC_FATAL`, and `ORC_FT_CALL`) across font, shader, texture, resource management, renderer, vertex array, and animator modules, converting boolean error returns to `void` and resource lookups to `const Ref<ResourceType>&`:
1. Replace `ftCall` in the internal font utility header with `ORC_FT_CALL(call, message, ...)` wrapping `ORC_FATAL_CHECK` directly via string literal concatenation (`message "\n\tFreeType Error: {}"`).
2. Change `Font::loadFromFile(const FilePath& filePath, uint32_t size)` return type from `bool` to `void`, replace all `if (!ftCall(...))` blocks and manual error-path cleanup with single-line `ORC_FT_CALL` invocations, and check `FT_Load_Char` in `Font::calculateBitmap` via `ORC_FT_CALL` (retaining `ORC_LOG_WARNING` and `'?'` fallback in `Font::getCharacter` for non-ASCII character codes).
3. Change `Shader::loadFromFile`, `Shader::loadFromString`, `Shader::readShader`, and `Shader::compile` return types from `bool` to `void`. In `Shader::readShader`, collapse the `!shaderFile.is_open()` and `!shaderFile.good()` branches into a single `ORC_FATAL_CHECK(shaderFile.is_open(), ...)`. In `Shader::compile`, call `ORC_FATAL` with the shader/program info log when `status == GL_FALSE`.
4. Change `Texture::loadFromFile` and `Texture::loadFromMemory` return types from `bool` to `void`, replacing error logs and `false` returns with `ORC_FATAL_CHECK` (`pixels != nullptr`, `dataFormat != 0 && internalFormat != 0`, and `!data.empty()`).
5. Change `Resources<ResourceType>::loadResources(const FilePath& xmlPath)` return type from `bool` to `void` (keeping the `XML_ERROR_EMPTY_DOCUMENT` allowance for empty manifest files), remove dead `if (loadFromFile(...))` branches, and enforce valid animation frame dimensions (`dimensionString.size() == 4`) and non-empty frame lists (`!frames.empty()`) via `ORC_FATAL_CHECK`.
6. Change `Resources<ResourceType>::getResource(std::string_view name) const` return type from `Ref<ResourceType>` to `const Ref<ResourceType>&`, enforcing `ORC_FATAL_CHECK(find != m_resources.end(), "Requested non-existing resource '{}'.", name)`.
7. Replace all `ORC_ASSERT` calls in `Renderer` and `VertexArray` with `ORC_FATAL_CHECK`, and replace the silent null-texture / null-font early returns in `Renderer::draw(const Sprite&)` and `Renderer::draw(const Text&)` with `ORC_FATAL_CHECK`.
8. In `Animator`, enforce `ORC_FATAL_CHECK(sprite != nullptr, ...)` in `Animator::Animator(Sprite* sprite)` and `Animator::setSprite(Sprite* sprite)`, and replace missing-animation error logs in `playAnimation` and `setDefaultAnimation` with `ORC_FATAL_CHECK(animation != m_animations.end(), ...)`.

**Blocked by:** 01

- [x] `ORC_FT_CALL` wraps `ORC_FATAL_CHECK` directly and is used across `Font::loadFromFile` and `Font::calculateBitmap`
- [x] `Font::loadFromFile`, `Shader::loadFromFile`/`loadFromString`/`readShader`/`compile`, `Texture::loadFromFile`/`loadFromMemory`, and `Resources::loadResources` return `void` and abort via `ORC_FATAL_CHECK` / `ORC_FATAL` on failure
- [x] `Resources::getResource` checks existence via `ORC_FATAL_CHECK` and returns `const Ref<ResourceType>&`
- [x] `Renderer` and `VertexArray` use `ORC_FATAL_CHECK` for `Scene Scope`, vertex buffer layout, and non-null `Sprite` texture / `Text` font
- [x] `Animator` enforces non-null `Sprite*` and valid animation names via `ORC_FATAL_CHECK`
- [x] `./scripts/verify.sh` passes 100%

## Answer

Enforced fail-fast crash contract via ORC_FATAL_CHECK, ORC_FATAL, and ORC_FT_CALL across Font, Shader, Texture, ResourceHolder, Renderer, VertexArray, and Animator.
