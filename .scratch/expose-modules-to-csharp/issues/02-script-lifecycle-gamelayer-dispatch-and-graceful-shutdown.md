# 02: Implement Script Lifecycle, GameLayer Dispatch, Automatic Scene Scope, Leak Verification & Window::close Graceful Shutdown

Status: ready-for-agent
Type: task
Blocked by: 01

**What to build:** Complete the managed script lifecycle and `GameLayer` integration from creation and constructor context injection through per-frame update/render dispatch and graceful teardown verification. See spec §4, §5, and §8.
- **Script & context:** Add move-only RAII `orc::Script` (`GCHandle`), `Orc.Script` (`OnAttach`, `OnDetach`, `OnUpdate`, `OnRender`, `OnDestroy`), and `Orc.ScriptContext` populated before the script constructor runs via a `[ThreadStatic]` pending slot.
- **Layer dispatch & Scene Scope:** Inject `Scripting&` into `LayerContext`, add `std::vector<Script>` and `addScript` to `orc::GameLayer`, drive `attachScripts`, `detachScripts`, `updateScripts`, and `renderScripts` (bracketed in `beginScene(m_camera)` / `endScene()`) from `GameLayerManager` and `Engine::run()`, and guard `Renderer::beginScene` against nesting.
- **Leak check & graceful shutdown:** Track live scripts and owning wrappers in `ScriptRuntime`, verify zero survivors via `Scripting::verifyShutdown()` in `Engine::~Engine`, and add `Window::close()` + `ORC_SMOKE_TEST_SECONDS` so smoke tests exit cleanly and run full teardown on every `scripts/verify` invocation.

## Acceptance Criteria

- [ ] **`LayerContext` & `orc::Script` RAII wrapper**
  - `orc::LayerContext` includes `Scripting& scripting`.
  - Move-only `orc::Script` (`Script(const Script&) = delete`, `Script(Script&&) noexcept`, `~Script()`) holds `Scripting*` and a normal (non-pinned) `GCHandle` (`void*`), calling `destroyScript` on destruction when non-null.
  - `destroyScript` invokes `Orc.Script.OnDestroy()` before freeing the `GCHandle`.
- [ ] **`Orc.Script` & constructor-time `ScriptContext`**
  - `Orc.Script` is `abstract` with virtual `OnAttach()`, `OnDetach()`, `OnUpdate(float deltaTime)`, `OnRender()`, `OnDestroy()`, and `protected ScriptContext Context { get; }`.
  - `ScriptRuntime.CreateScript` resolves the type via `m_scriptAssembly.GetType(typeName)` (failing fast if no script assembly is loaded, or if the type is missing, abstract, or not a subclass of `Orc.Script`), stores the new `ScriptContext` in a `[ThreadStatic]` pending slot before `Activator.CreateInstance`, and clears it immediately after.
  - The protected `Orc.Script()` constructor consumes the pending slot and fails fast if it is null, guaranteeing `Context` is valid inside derived constructors and preventing standalone `new` construction outside the engine.
- [ ] **`GameLayer` script ownership & engine-driven dispatch**
  - `orc::GameLayer` holds `std::vector<Script> m_scripts` and `bool m_isAttached = false`, and provides `void addScript(std::string_view typeName)` (which also calls `script.onAttach()` immediately if `m_isAttached` is already true).
  - `GameLayer` exposes engine hooks `attachScripts()`, `detachScripts()`, `updateScripts(float deltaTime)`, and `renderScripts()`.
  - `GameLayerManager::setActiveLayer` calls `detachScripts()` → `onDetach()` on the outgoing layer and `syncCameraWithWindow()` → `onAttach()` → `attachScripts()` on the incoming layer; `GameLayerManager::clear` calls `detachScripts()` → `onDetach()`.
  - `Engine::run()` copies the active layer `Ref<GameLayer>` by value at the start of the frame and invokes `onUpdate(elapsed)` → `updateScripts(elapsed)` and `onRender()` → `renderScripts()`.
- [ ] **Automatic `Scene Scope` & nesting guard**
  - When `!m_scripts.empty()`, `GameLayer::renderScripts()` brackets script `OnRender()` execution in `m_layerContext.renderer.beginScene(m_camera)` and `m_layerContext.renderer.endScene()`.
  - `Renderer::beginScene` checks `ORC_FATAL_CHECK(!m_isSceneActive, ...)` so nested scenes fail fast.
- [ ] **Live-wrapper registry & `verifyShutdown`**
  - `ScriptRuntime` tracks live script instances and per-type counts of live owning native wrappers.
  - `Engine::~Engine` calls `m_scripting->verifyShutdown()` after `m_gameLayerManager->clear()` and before resource `clear()` calls; `verifyShutdown` fails fast listing surviving type names and counts if any remain, and logs `Scripting: shutdown verified` when clean.
- [ ] **Graceful shutdown (`Window::close`) & smoke-test teardown**
  - `Window::close()` is added to `orc::Window` and dispatches `Event::WindowClosed` through the window event callback.
  - `Menu` reads `ORC_SMOKE_TEST_SECONDS` at construction and calls `m_layerContext.window.close()` in `onUpdate` once elapsed time reaches that duration.
  - When `-TimeoutSeconds` / `--timeout` is positive, `scripts/run-demo.ps1` and `scripts/run-demo.sh` set `ORC_SMOKE_TEST_SECONDS` to `max(1, timeout - 2)` and fail if the process times out or exits with a non-zero code (keeping the hard kill only as a hang backstop).
- [ ] **Smoke test & showcase scripts**
  - `Menu` attaches `SmokeTestScript : Orc.Script` (in `TestProject.Scripts`), which logs `SmokeTestScript: lifecycle OK` once after its first `OnRender()`.
  - The existing `ScriptTest` layer attaches `ScriptTestScript : Orc.Script`.
  - `scripts/verify.ps1` and `scripts/verify.sh` require all three markers: `Scripting: managed runtime initialized`, `SmokeTestScript: lifecycle OK`, and `Scripting: shutdown verified`.
- [ ] **Documentation (Ticket 02 deliverables)**
  - `GLOSSARY.md` adds **Script**, **ScriptContext**, and **Native Wrapper**.
  - `AGENTS.md` updates `LayerContext` contents, the frame loop (C++ layers bracket their own `onRender()`; `GameLayer::renderScripts()` brackets script rendering), and graceful shutdown via `Window::close()`.
  - `docs/adr/0012-csharp-module-bindings-and-script-lifecycle.md` is updated with the engine-driven `std::vector<Script>` lifecycle, automatic `Scene Scope`, constructor-time context injection, `IDisposable`-without-finalizer leak verification, and `Window::close()` teardown verification.
- [ ] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and graceful `TestProject` smoke test verifying all three lifecycle/shutdown markers).

## Comments
