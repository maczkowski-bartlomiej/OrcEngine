# 04: Decouple Engine Lifecycle and Migrate TestProject Layers

**What to build:**
Remove leaked frame lifecycle (`begin`/`end`) from `Engine::run()`, provide access to `Renderer2D`, and migrate all demo layers in `TestProject` to use `renderer2D.beginScene(camera)` / `endScene()`, verifying natural submission order and eliminating inverted z-index hacks.

**Blocked by:** 03: Add Shape Overloads and Unbatched Boundary Flush

**Status:** ready-for-agent

- [ ] Remove `m_renderer->begin(gameLayer->getCamera())` and `m_renderer->end()` from `Engine::run()`, keeping buffer clear in `Engine`.
- [ ] Expose `Renderer2D&` on `Engine` / `GameLayer`.
- [ ] Migrate `SpritesTest`, `CirclesTest`, `RectanglesTest`, `AnimationTest`, `Menu`, and other demo layers in `TestProject` to use `Renderer2D`.
- [ ] Remove deprecated shape batch structures (`m_sprites`, `m_circles`, `m_rectangles`, `m_glyphs`) and inverted `zIndex` logic from `Renderer`.
- [ ] Build and verify on Windows debug and release presets.
