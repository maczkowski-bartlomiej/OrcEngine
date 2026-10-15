# 04: Decouple Engine Lifecycle and Migrate TestProject Layers

**What to build:**
Remove leaked frame lifecycle (`begin`/`end`) from `Engine::run()`, provide access to `Renderer2D`, and migrate all demo layers in `TestProject` to use `renderer2D.beginScene(camera)` / `endScene()`, verifying natural submission order and eliminating inverted z-index hacks.

**Blocked by:** 03: Add Shape Overloads and Unbatched Boundary Flush

**Status:** resolved

- [x] Remove `m_renderer->begin(gameLayer->getCamera())` and `m_renderer->end()` from `Engine::run()`, keeping buffer clear in `Engine`.
- [x] Expose `Renderer2D&` on `Engine` / `GameLayer`.
- [x] Migrate `SpritesTest`, `CirclesTest`, `RectanglesTest`, `AnimationTest`, `Menu`, and other demo layers in `TestProject` to use `Renderer2D`.
- [x] Remove deprecated shape batch structures (`m_sprites`, `m_circles`, `m_rectangles`, `m_glyphs`) and inverted `zIndex` logic from `Renderer`.
- [x] Build and verify on Windows debug and release presets.

## Answer

Decoupled frame lifecycle from `Engine::run()`, exposed `Renderer2D` across engine and layers, migrated all demo screens in `TestProject`, removed legacy batch structures and inverted `zIndex` hacks, and verified clean compilation and execution on `windows-debug` and `windows-release`.
