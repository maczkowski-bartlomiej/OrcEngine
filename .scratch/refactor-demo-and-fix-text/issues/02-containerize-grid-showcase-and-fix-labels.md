# 02: Containerize Grid Showcase Layers & Fix Label Placement

Status: resolved
Type: task
Blocked by: 01

**What to build:** Refactor `RectanglesTest`, `CirclesTest`, and `SpritesTest` to use structured containers (`std::vector<GridItem>`) and loop rendering instead of 26 loose member variables and 24 manual draw calls each. Position labels inside cell headers ($y = 20.0f, 170.0f, 320.0f$) via `demo::createShowcaseLabel` with top-center origin `(size.x / 2.0f, 0.0f)`, eliminating grid-line intersections and top-window clipping. Keep direct pointers to animated/rotating shapes for fast zero-overhead updates in `onUpdate()`, and consume `demo::drawGrid`, `demo::renderNavigationMenu`, and `demo::handleLayerNavigation`.

## Acceptance Criteria

- [ ] In `RectanglesTest`, replace individual shape/text members (`m_rectangle1..13`, `m_text1..13`) with `std::vector<GridItem>` containing the rectangle and label, rendered via a loop in `onRender()`.
- [ ] In `CirclesTest`, replace individual shape/text members (`m_circle1..13`, `m_text1..13`) with `std::vector<GridItem>` containing the circle and label, rendered via a loop in `onRender()`.
- [ ] In `SpritesTest`, replace individual sprite/text members (`m_sprite1..13`, `m_text1..13`) with `std::vector<GridItem>` containing the sprite and label, rendered via a loop in `onRender()`.
- [ ] All showcase labels are constructed via `demo::createShowcaseLabel` at $y = 20.0f$ (row 0), $y = 170.0f$ (row 1), and $y = 320.0f$ (row 2), ensuring labels hang cleanly below grid lines and are fully visible within their respective cells.
- [ ] Rotating shapes (`m_rectangle4`, `m_circle11`, `m_sprite4`) are tracked via direct shape pointers (`orc::Rectangle*`, `orc::Circle*`, `orc::Sprite*`) and updated in `onUpdate()`.
- [ ] Manual `drawLines()` declarations and definitions are removed from all three layers in favor of `demo::drawGrid(m_layerContext.renderer)`.
- [ ] All three layers call `demo::renderNavigationMenu` in `onGuiRender()` and `demo::handleLayerNavigation` in `onUpdate()`.
- [ ] `.\scripts\verify.ps1` passes 100% (conventions lint, zero-warning build, and 5-second `TestProject` smoke test).

## Comments

## Answer

Refactored RectanglesTest, CirclesTest, and SpritesTest to containerize items in std::vector<GridItem> with loop rendering, anchored labels to top-center at y=20, 170, 320, removed loose members and drawLines, and tracked rotating shapes via direct pointers.
