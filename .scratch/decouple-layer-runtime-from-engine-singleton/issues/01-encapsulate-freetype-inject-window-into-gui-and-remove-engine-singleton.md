# 01: Encapsulate FreeType in Font, Inject Window into Gui, and Remove Engine Singleton

Status: resolved
Type: task
Blocked by: none

**What to build:** Encapsulate FreeType library and font face initialization/cleanup inside `Font::loadFromFile` so the standalone `FTLibrary` class and public FreeType header dependency can be deleted, inject `Window&` directly into `Gui` at construction time, and remove `Engine::get()`, `Engine::m_instance`, and all individual subsystem getters from `Engine`, leaving `Engine::getLayerContext()` as the sole subsystem accessor.

- [x] `Font::loadFromFile` initializes a local `FT_Library` and `FT_Face`, passes the face handle to `calculateBitmap`, and calls `FT_Done_Face` and `FT_Done_FreeType` before returning.
- [x] `Font` no longer stores an `FT_Face` member or includes the FreeType header in its public interface, and the standalone `FTLibrary` class is removed from the engine and build configuration.
- [x] `Gui` accepts `Window& window` in its constructor (`explicit Gui(Window& window)`), stores `Window& m_window`, and uses `m_window` instead of `Engine::get().getWindow()`.
- [x] `Engine::get()`, `Engine::m_instance`, and all individual subsystem getters (`getAudio`, `getWindow`, `getRenderer`, `getFTLibary`, `getGameLayerManager`, `getInput`, `getFontResources`, `getTextureResources`, `getAnimationResources`) are removed from `Engine`.
- [x] Application startup in `TestProject` accesses `gameLayerManager` via `engine.getLayerContext().gameLayerManager`.
- [x] The project builds cleanly, passes the demo smoke test (`--run --timeout 5`) with fonts and ImGui rendering properly, and passes the conventions linter.

## Answer

Encapsulated local FT_Library/FT_Face in Font::loadFromFile, deleted FTLibrary, injected Window& into Gui, and removed Engine::get() and individual getters from Engine.
