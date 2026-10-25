# Spec: Universal Fail-Fast Error Handling

Status: ready-for-agent

## Problem Statement

Error handling across `OrcEngine` is inconsistent and mixes multiple conflicting paradigms across subsystems:
- Some modules (`Scripting` and XML manifest loading in `ResourceHolder`) enforce an unconditional fail-fast crash contract (`ORC_FATAL_CHECK`), terminating immediately via `std::abort()` on invalid state.
- Other modules (`Window` and `Gui`) log fatal messages (`ORC_LOG_FATAL`) on bootstrap failure and return early without aborting, leaving the engine in a half-initialized state that crashes later or silently misbehaves.
- Other modules (`Audio`, `Font`, `Shader`, `Texture`, `Animator`, `GameLayerManager`, and asset loading/lookup in `ResourceHolder`) treat missing files, compilation failures, SDK errors, and invalid lookup keys as recoverable runtime errors—logging `ORC_LOG_ERROR`, returning `false` or `nullptr`, and littering methods with defensive state guards (such as `if (!m_system) return;`), while constructors (`Shader`, `Texture`, `Font`, `Window`) ignore the `bool` return values anyway.
- Precondition checks (`Renderer` `Scene Scope`, `VertexArray` layout, `GameLayerManager` layer names/pointers, `Bus` construction) use debug-only `ORC_ASSERT`, which disappears in Release builds, creating ambiguity between `ORC_ASSERT` and `ORC_FATAL_CHECK` while leaving unused macros (`ORC_WARNING`, `ORC_ERROR`, `ORC_ERROR_CHECK`) in the debug facility.
- Third-party C/C++ SDK helpers (`fmodCall` and `ftCall`) log at `Error` level and return `bool`, forcing callers either to wrap calls in `fmt::format(...)` boilerplate and log duplicate error messages, or to ignore return values altogether. Furthermore, multiple call sites perform redundant post-call null checks on out-pointers or pre-call file-existence checks that duplicate the guarantees of the underlying SDK.

## Solution

Establish a single, universal fail-fast error-handling contract across `OrcEngine`: whenever the engine reaches an invalid state—whether during subsystem/SDK bootstrap, asset loading and manifest parsing, or runtime lookups and API precondition checks—log the fatal error with source location context, break in the debugger (in Debug builds), and unconditionally terminate the process via `std::abort()` in both Debug and Release builds. Standardize exclusively on `ORC_FATAL_CHECK` and `ORC_FATAL` macros (plus `ORC_FMOD_CALL` and `ORC_FT_CALL` macro wrappers that delegate to `ORC_FATAL_CHECK` without `fmt::format` boilerplate), convert error-signaling `bool` returns to `void` and resource lookups to `const Ref<T>&`, strip all defensive half-initialized state guards and redundant post-call pointer checks, and synchronize `CODING_STANDARDS.md`, `AGENTS.md`, `GLOSSARY.md`, and a new ADR.

## User Stories

1. As an engine developer, I want every invalid state across subsystem initialization, asset loading, resource lookup, and runtime preconditions to immediately log a fatal message and terminate via `std::abort()`, so that bugs are surfaced instantly at their point of origin and engine code can always assume valid state.
2. As an engine developer, I want `ORC_FATAL_CHECK` and `ORC_FATAL` to be the sole assertion and fatal-error macros active identically in both Debug and Release builds—with `ORC_ASSERT`, `ORC_WARNING`, `ORC_ERROR`, and `ORC_ERROR_CHECK` removed—so that there is never ambiguity about which check macro to use and side-effecting expressions never vanish in Release builds.
3. As an engine developer, I want `ORC_FATAL_CHECK` and `ORC_FATAL` to remain preprocessor macros rather than functions, so that format arguments (such as error-string lookups and path conversions) are lazily evaluated only on failure and `ORC_DEBUGBREAK()` halts the debugger directly on the caller's line.
4. As an engine developer, I want FMOD and FreeType calls wrapped in `ORC_FMOD_CALL` and `ORC_FT_CALL` macros that delegate directly to `ORC_FATAL_CHECK` and accept variadic format arguments via string literal concatenation, so that SDK errors automatically include `FMOD_ErrorString` / `FT_Error_String` without `fmt::format(...)` call-site boilerplate or temporary result variables.
5. As an engine developer, I want standalone `ORC_LOG_ERROR` usages eliminated from engine code (and `ORC_LOG_WARNING` reserved strictly for benign, valid-state notices such as switching to an already-active `GameLayer` or falling back on a non-ASCII character code in `Font`), so that true errors are never silently logged and ignored.
6. As an engine developer, I want subsystem initialization and asset-loading methods (`Logger::init`, `Window::initGLAD`, `Font::loadFromFile`, `Shader::loadFromFile`, `Shader::loadFromString`, `Shader::readShader`, `Shader::compile`, `Texture::loadFromFile`, `Texture::loadFromMemory`, `Resources::loadResources`) to return `void` instead of `bool`, so that callers are never burdened with dead error-return propagation.
7. As an engine consumer, I want `Resources::getResource` to enforce resource existence via `ORC_FATAL_CHECK` and return `const Ref<ResourceType>&` instead of `Ref<ResourceType>` by value, so that resource lookups cannot return `nullptr` and avoid atomic reference-count churn in accordance with observer getter standards.
8. As an engine developer, I want defensive half-initialized state guards (such as `if (!m_system)` across `Audio`) and redundant post-call out-pointer or pre-call file-existence checks removed from `Audio`, `Scripting`, and `Shader`, so that subsystem implementations remain flat and free of dead branches.
9. As an engine developer, I want `Window` creation (`m_glfwWindow != nullptr`), `Gui` context and backend initialization, `GLFW` error callbacks, `Renderer::draw` texture/font presence on `Sprite` and `Text`, `Animator` target `Sprite*` validity, and `GameLayerManager` layer registration/switching to be enforced via `ORC_FATAL_CHECK` / `ORC_FATAL`, so that unconfigured drawables or windowing failures fail fast at the exact point of misuse.
10. As an engine contributor, I want `CODING_STANDARDS.md`, `AGENTS.md`, `GLOSSARY.md`, and a new ADR in `docs/adr/` updated to document the universal fail-fast crash contract, so that future human and agent contributions follow the unified error-handling philosophy.

## Implementation Decisions

- **Core Debug Macros**:
  - Retain only `ORC_FATAL(message, ...)` and `ORC_FATAL_CHECK(check, message, ...)` in the debug header.
  - Delete `ORC_ASSERT`, `ORC_WARNING`, `ORC_ERROR`, and `ORC_ERROR_CHECK`.
  - In Debug builds (`ORC_DEBUG_ASSERTS`), `ORC_FATAL` and `ORC_FATAL_CHECK` log via `ORC_LOG_FATAL`, invoke `ORC_DEBUGBREAK()`, and call `std::abort()`. In Release builds, they log via `ORC_LOG_FATAL` and call `std::abort()`.
  - Remove `ORC_LOG_ERROR` from the logger macro surface (or disallow standalone `ORC_LOG_ERROR` in engine code) so that logging levels used by engine macros are `ORC_LOG_INFO`, `ORC_LOG_WARNING`, and `ORC_LOG_FATAL` (invoked through `ORC_FATAL` / `ORC_FATAL_CHECK`).
- **SDK Call Wrapper Macros (`ORC_FMOD_CALL` and `ORC_FT_CALL`)**:
  - Replace the boolean-returning `fmodCall` and `ftCall` functions in the internal audio and font utility headers with `ORC_FMOD_CALL(call, message, ...)` and `ORC_FT_CALL(call, message, ...)` macros that delegate directly to `ORC_FATAL_CHECK`:
    ```cpp
    #define ORC_FMOD_CALL(call, message, ...) \
    	do \
    	{ \
    		const FMOD_RESULT fmodResult = (call); \
    		ORC_FATAL_CHECK(fmodResult == FMOD_RESULT::FMOD_OK, message "\n\tFMOD Error: {}" __VA_OPT__(, ) __VA_ARGS__, FMOD_ErrorString(fmodResult)); \
    	} while (0)

    #define ORC_FT_CALL(call, message, ...) \
    	do \
    	{ \
    		const FT_Error ftResult = (call); \
    		ORC_FATAL_CHECK(ftResult == FT_Err_Ok, message "\n\tFreeType Error: {}" __VA_OPT__(, ) __VA_ARGS__, FT_Error_String(ftResult)); \
    	} while (0)
    ```
  - Eliminate all `fmt::format(...)` wrappers at FMOD and FreeType call sites, passing format string literals and arguments directly to `ORC_FMOD_CALL` and `ORC_FT_CALL`.
- **Subsystem-by-Subsystem Error Handling & API Signatures**:
  - **Logger**: Change `Logger::init(const FilePath& logPath)` return type from `bool` to `void`. In the `catch (const spdlog::spdlog_ex&)` block, call `std::abort()` after `ORC_DEBUGBREAK()`.
  - **Scripting**: Keep `ORC_FATAL_CHECK` across `loadHostFxr` and `initializeRuntime`, and remove redundant out-pointer checks (`&& context != nullptr`, `&& loadAssemblyDelegate != nullptr`, `&& functionPtr != nullptr`) where `rc == 0` already guarantees validity.
  - **Window**: Change `Window::initGLAD()` return type from `bool` to `void`. Replace `if (!errorResult)` + `ORC_LOG_FATAL` + `return false;` with `ORC_FATAL_CHECK` for `glfwInit()` and `gladLoadGLLoader(...)`. Add `ORC_FATAL_CHECK(m_glfwWindow != nullptr, "Failed to create GLFW window.")` immediately after `glfwCreateWindow(...)`. Change `glfwErrorCallback` to call `ORC_FATAL("GLFW error occurred...\n\tCode: {}\n\tDescription: {}", code, description)`.
  - **Gui**: Replace the `if (!context)`, `if (!ImGui_ImplGlfw_InitForOpenGL(...))`, and `if (!ImGui_ImplOpenGL3_Init(...))` checks and early returns with `ORC_FATAL_CHECK`.
  - **Audio & Bus**:
    - In `Audio::Audio`, use `ORC_FMOD_CALL` for `FMOD::Studio::System::create(&m_system)` and `m_system->initialize(...)`.
    - Delete all `if (!m_system)` guards across `Audio::getBus`, `Audio::loadBank`, `Audio::play`, `Audio::update`, and `Audio::~Audio`.
    - Delete the pre-call `std::filesystem::exists(filePath)` assertion and check in `Audio::loadBank` (FMOD's `loadBankFile` natively reports missing files).
    - Delete all redundant post-call null checks on FMOD out-pointers (`!m_system`, `!bus`, `!bank`, `if (!bank) continue;` in `~Audio`, `!eventDescription`, `!eventInstance`).
    - In `Bus`, replace `ORC_ASSERT(m_bus, ...)` with `ORC_FATAL_CHECK(m_bus != nullptr, ...)` and replace all `fmodCall(...)` invocations with `ORC_FMOD_CALL(...)`.
  - **Font**:
    - Change `Font::loadFromFile(const FilePath& filePath, uint32_t size)` return type from `bool` to `void`.
    - Replace all `if (!ftCall(...))` blocks (and manual error-path `FT_Done_Face` / `FT_Done_FreeType` cleanup) with single-line `ORC_FT_CALL(...)` invocations for `FT_Init_FreeType`, `FT_New_Face`, `FT_Set_Pixel_Sizes`, `FT_Done_Face`, and `FT_Done_FreeType`.
    - In `Font::calculateBitmap`, check `FT_Load_Char` using `ORC_FT_CALL`.
    - In `Font::getCharacter`, retain `ORC_LOG_WARNING` and the `'?'` fallback glyph when a non-ASCII character code (`>= CHARACTER_COUNT`) is requested.
  - **Shader**:
    - Change `loadFromFile`, `loadFromString`, `readShader`, and `compile` return types from `bool` to `void`.
    - In `Shader::readShader`, collapse `!shaderFile.is_open()` and `!shaderFile.good()` into a single `ORC_FATAL_CHECK(shaderFile.is_open(), "Failed to load shader at path '{}'\n\tReason: {}", filePath.string(), getErrnoMessage(errno))`.
    - In `Shader::compile`, if vertex compilation, fragment compilation, or program linking fails (`status == GL_FALSE`), query the info log and call `ORC_FATAL(...)`.
  - **Texture**:
    - Change `loadFromFile` and `loadFromMemory` return types from `bool` to `void`.
    - In `Texture::loadFromFile`, enforce `ORC_FATAL_CHECK(pixels != nullptr, "Failed to load texture at path '{}'\n\tReason: {}", filePath.string(), stbi_failure_reason())` and `ORC_FATAL_CHECK(dataFormat != 0 && internalFormat != 0, "Failed to load texture at path '{}'\n\tReason: Texture format not supported", filePath.string())`.
    - In `Texture::loadFromMemory`, enforce `ORC_FATAL_CHECK(!data.empty(), "Failed to load texture from memory\n\tReason: Data is empty")`.
  - **ResourceHolder**:
    - Change `Resources<ResourceType>::loadResources(const FilePath& xmlPath)` return type from `bool` to `void` (retaining the `XML_ERROR_EMPTY_DOCUMENT` allowance for empty manifest files).
    - Change `Resources<ResourceType>::getResource(std::string_view name) const` return type from `Ref<ResourceType>` to `const Ref<ResourceType>&`, using `ORC_FATAL_CHECK(find != m_resources.end(), "Requested non-existing resource '{}'.", name)`.
    - In `loadResources` specializations for `Font`, `Shader`, and `Texture`, construct the resource and call `loadFromFile` directly without `if (resource->loadFromFile(...))` branches.
    - In `Resources<Animation>::loadResources`, replace `if (dimensionString.size() != 4)` + `ORC_LOG_ERROR` + `break` with `ORC_FATAL_CHECK(dimensionString.size() == 4, "Invalid dimension for animation '{}'", name)` and replace `if (frames.size()) ... else ORC_LOG_ERROR(...)` with `ORC_FATAL_CHECK(!frames.empty(), "Failed to load resource '{}'.", name)`.
  - **Renderer & VertexArray**:
    - Replace all `ORC_ASSERT(m_isSceneActive, ...)` checks in `Renderer` with `ORC_FATAL_CHECK(m_isSceneActive, ...)`.
    - In `Renderer::draw(const Sprite& sprite)`, replace `if (!sprite.getTexture()) return;` with `ORC_FATAL_CHECK(sprite.getTexture() != nullptr, "Cannot draw Sprite without a valid Texture!")`.
    - In `Renderer::draw(const Text& text)`, replace `if (!text.getFont() || !text.getFont()->getBitmap()) return;` with `ORC_FATAL_CHECK(text.getFont() != nullptr && text.getFont()->getBitmap() != nullptr, "Cannot draw Text without a valid Font!")`.
    - In `VertexArray::addVertexBuffer`, replace `ORC_ASSERT(!vertexBuffer->getLayout().getElements().empty(), ...)` with `ORC_FATAL_CHECK(!vertexBuffer->getLayout().getElements().empty(), ...)`.
  - **GameLayerManager & Animator**:
    - In `GameLayerManager::setActiveLayer`, replace `ORC_ASSERT(!name.empty(), ...)` with `ORC_FATAL_CHECK(!name.empty(), ...)`, replace the missing-layer `else { ORC_LOG_ERROR(...); }` with `ORC_FATAL_CHECK(find != m_layers.end(), "Requested switch to non-existing game layer '{}'.", name)`, and keep `ORC_LOG_WARNING` + `return;` when switching to the already-active layer.
    - In `GameLayerManager::addLayer`, replace `ORC_ASSERT` checks with `ORC_FATAL_CHECK(!name.empty(), ...)` and `ORC_FATAL_CHECK(gameLayer != nullptr, ...)`, and replace the duplicate-layer `ORC_LOG_WARNING` with `ORC_FATAL_CHECK(m_layers.try_emplace(std::string(name), std::move(gameLayer)).second, "Game layer '{}' is already added.", name)`.
    - In `Animator`, check `ORC_FATAL_CHECK(sprite != nullptr, ...)` in `Animator::Animator(Sprite* sprite)` and `Animator::setSprite(Sprite* sprite)`, and replace missing-animation `ORC_LOG_ERROR` + `return;` in `playAnimation` and `setDefaultAnimation` with `ORC_FATAL_CHECK(animation != m_animations.end(), "Animation with name '{}' does not exist!", name)`.
- **Documentation & Standards Synchronization**:
  - Record an ADR (`docs/adr/0009-universal-fail-fast-error-handling.md`) documenting the transition from mixed recoverable/debug-assert error handling to the universal fail-fast crash contract.
  - Update `CODING_STANDARDS.md` Section 5 ("Error Handling, Assertions & Logging") to mandate `ORC_FATAL_CHECK`, `ORC_FATAL`, `ORC_FMOD_CALL`, and `ORC_FT_CALL`, document the removal of `ORC_ASSERT` and recoverable `bool`/`nullptr` error returns, and specify the roles of `ORC_LOG_INFO` and `ORC_LOG_WARNING`.
  - Update `AGENTS.md` and `GLOSSARY.md` to reference the new ADR and keep terminology consistent.

## Testing Decisions

- **Seam 1: Unified Verification Gate (`scripts/verify.sh` / `scripts/verify.ps1`) & Multi-Preset Build**:
  - All changes will be verified through the repository's single authoritative verification seam (`scripts/verify.sh` on Linux / `scripts/verify.ps1` on Windows), plus a Release preset build (`./scripts/build.sh --preset linux-release`):
    1. **Conventions Linter (`scripts/lint-conventions.sh`)**: Verifies `#pragma once`, `namespace orc`, tab indentation, and zero trailing whitespace across all modified headers and source files.
    2. **Zero-Warning Debug & Release Compilation (`scripts/build.sh`)**: Verifies that `OrcEngine` and `TestProject` compile with zero warnings under `-Wall -Wextra` in both Debug (`ORC_DEBUG_ASSERTS` enabled) and Release (`ORC_DEBUG_ASSERTS` disabled) presets, confirming that no unused variables or unreferenced parameters are introduced when `ORC_DEBUGBREAK()` is compiled out.
    3. **Runtime Demo Smoke Test (`scripts/run-demo.sh --timeout 5 --fail-on-error-log`)**: Executes `TestProject` end-to-end across `Logger`, `Scripting`, `GameLayerManager`, `Window`, `Renderer`, `Gui`, `Audio` (`ORC_FMOD_CALL`), `Input`, `Font` (`ORC_FT_CALL`), `Shader`, `Texture`, and `ResourceHolder` with zero fatal or error logs.

## Out of Scope

- **C# Managed Exception Policy**: Error-handling conventions inside C# assemblies beyond the native-managed `hostfxr` hosting boundary are deferred until the C# scripting API surface expands.
- **Asset Hot-Reloading or Fallback Placeholders**: Graceful fallback textures/shaders for missing or corrupt assets are intentionally excluded by the fail-fast philosophy.
- **`ResourceHolder` Architectural Replacement**: Replacing XML manifests with C#-driven resource management remains a separate future milestone.

## Further Notes

- Ensure `scripts/run-demo.sh` prefers `/usr/share/dotnet` when discovering `DOTNET_ROOT` if multiple .NET runtimes exist on the host machine so the smoke test consistently matches the system `libnethost` installation.
