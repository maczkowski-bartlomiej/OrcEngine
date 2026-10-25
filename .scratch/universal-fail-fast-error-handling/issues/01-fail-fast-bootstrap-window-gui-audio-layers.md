# 01: Enforce Fail-Fast Crash Contract in Bootstrap, Windowing, GUI, Audio, and Layer Management

Status: resolved
Type: task
Blocked by: none

**What to build:** Enforce the universal fail-fast crash contract (`ORC_FATAL_CHECK`, `ORC_FATAL`, and `ORC_FMOD_CALL`) across engine bootstrap, scripting, windowing, GUI, audio, and layer management subsystems, eliminating boolean error returns, defensive half-initialized state guards, and redundant post-call pointer checks:
1. Update `Logger::init(const FilePath& logPath)` to return `void` instead of `bool`, and call `std::abort()` after `ORC_DEBUGBREAK()` in the `catch (const spdlog::spdlog_ex&)` block. Update `scripts/run-demo.sh` so `DOTNET_ROOT` auto-detection prefers `/usr/share/dotnet` when present on Linux.
2. In `Scripting::initializeRuntime`, remove the redundant post-call out-pointer checks (`&& context != nullptr`, `&& loadAssemblyDelegate != nullptr`, and `&& functionPtr != nullptr`) where `rc == 0` already guarantees non-null output.
3. Update `Window::initGLAD()` to return `void` instead of `bool`, replace `if (!errorResult)` + `ORC_LOG_FATAL` + `return false;` with `ORC_FATAL_CHECK` for `glfwInit()` and `gladLoadGLLoader(...)`, add `ORC_FATAL_CHECK(m_glfwWindow != nullptr, "Failed to create GLFW window.")` after `glfwCreateWindow(...)`, and change `glfwErrorCallback` to call `ORC_FATAL("GLFW error occurred...\n\tCode: {}\n\tDescription: {}", code, description)`.
4. In `Gui::Gui`, replace the `if (!context)`, `if (!ImGui_ImplGlfw_InitForOpenGL(...))`, and `if (!ImGui_ImplOpenGL3_Init(...))` checks and early returns with `ORC_FATAL_CHECK`.
5. Replace `fmodCall` in the internal audio utility header with `ORC_FMOD_CALL(call, message, ...)` wrapping `ORC_FATAL_CHECK` directly via string literal concatenation (`message "\n\tFMOD Error: {}"`), eliminating `fmt::format` boilerplate at all call sites.
6. In `Audio` and `Bus`, replace all `fmodCall` usages with `ORC_FMOD_CALL`, replace `ORC_ASSERT(m_bus, ...)` in `Bus::Bus` with `ORC_FATAL_CHECK(m_bus != nullptr, ...)`, delete all `if (!m_system)` guards across `Audio`, delete the pre-call `std::filesystem::exists` check in `Audio::loadBank`, and delete all redundant post-call FMOD out-pointer checks (`!m_system`, `!bus`, `!bank`, `if (!bank) continue;` in `~Audio`, `!eventDescription`, `!eventInstance`).
7. In `GameLayerManager`, replace `ORC_ASSERT(!name.empty(), ...)` and `ORC_ASSERT(gameLayer, ...)` with `ORC_FATAL_CHECK`, enforce `ORC_FATAL_CHECK(find != m_layers.end(), ...)` in `setActiveLayer` instead of logging an error, enforce `ORC_FATAL_CHECK` on duplicate layer insertion in `addLayer`, and retain `ORC_LOG_WARNING` + `return;` when `setActiveLayer` is called with the already-active layer name.

**Blocked by:** None (can start immediately)

- [x] `Logger::init` returns `void` and aborts on initialization failure
- [x] `Scripting::initializeRuntime` omits redundant out-pointer checks when `rc == 0`
- [x] `Window::initGLAD` returns `void`, checks `glfwInit`, `m_glfwWindow != nullptr`, and `gladLoadGLLoader` via `ORC_FATAL_CHECK`, and `glfwErrorCallback` invokes `ORC_FATAL`
- [x] `Gui::Gui` checks ImGui context creation and GLFW/OpenGL3 backend initialization via `ORC_FATAL_CHECK`
- [x] `ORC_FMOD_CALL` wraps `ORC_FATAL_CHECK` directly without `fmt::format` call-site boilerplate
- [x] `Audio` and `Bus` use `ORC_FMOD_CALL` / `ORC_FATAL_CHECK` with zero `if (!m_system)` guards, pre-call `exists` checks, or redundant out-pointer null checks
- [x] `GameLayerManager` uses `ORC_FATAL_CHECK` for preconditions, missing layers, and duplicate layer registration while keeping `ORC_LOG_WARNING` for switching to the already-active layer
- [x] `./scripts/verify.sh` passes 100%

## Answer

Enforced fail-fast crash contract via ORC_FATAL_CHECK, ORC_FATAL, and ORC_FMOD_CALL across Logger, Scripting, Window, Gui, Audio, Bus, and GameLayerManager.
