# 01: Introduce Shared Demo Utilities & Migrate Navigation Across Demo Layers

Status: resolved
Type: task
Blocked by: none

**What to build:** Introduce a shared demo utilities module (`DemoUtilities` under namespace `demo`) in `TestProject` providing a centralized ImGui navigation menu, a 4x4 background grid drawer, a top-center showcase label factory, and an arrow-key layer cycling helper. Migrate non-grid demo layers (`AnimationTest`, `AudioTest`, `CameraTest`, `InputTest`, `Menu`, `ScriptTest`) to use the centralized navigation menu and arrow cycling helper, eliminating hundreds of lines of duplicate boilerplate.

## Acceptance Criteria

- [ ] `DemoUtilities.hpp` and `DemoUtilities.cpp` are created under `TestProject` in namespace `demo`.
- [ ] `demo::renderNavigationMenu(orc::GameLayerManager&)` renders the standard 8-button ImGui `"Navigation Menu"` window.
- [ ] `demo::drawGrid(orc::Renderer&)` draws the 4x4 white coordinate grid (vertical lines at $x = 200, 400, 600$ and horizontal lines at $y = 150, 300, 450$).
- [ ] `demo::createShowcaseLabel(const orc::Ref<orc::Font>&, std::string_view, const orc::Vector2f&)` constructs a `Text` scaled to $0.25$, anchored to top-center `(size.x / 2.0f, 0.0f)`, and positioned at the given coordinate.
- [ ] `demo::handleLayerNavigation(orc::LayerContext&, std::string_view, std::string_view)` handles `Key::Left` and `Key::Right` single-frame transitions to cycle between specified previous and next active layers.
- [ ] `AnimationTest`, `AudioTest`, `CameraTest`, `InputTest`, `Menu`, and `ScriptTest` call `demo::renderNavigationMenu` in `onGuiRender()` and `demo::handleLayerNavigation` in `onUpdate()`, removing duplicate menu code.
- [ ] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments

## Answer

Introduced DemoUtilities.hpp and DemoUtilities.cpp providing centralized ImGui navigation menu, 4x4 coordinate grid drawer, top-center label factory, and arrow-key cycling helper. Migrated non-grid demo layers.
