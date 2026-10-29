# Spec: Refactor Demo Code Size and Fix Showcase Text Placement

Triage: ready-for-agent

## Problem Statement

When running the engine demo harness (`TestProject`), developers and testers observe visual defects and severe code duplication:

1. **Text Layout Misalignment**: In the showcase layers (such as the rectangles, circles, and sprites demo scenes), title labels are centered directly on the dividing grid lines ($y = 150$ and $y = 300$), and the first row of labels is clipped above the top edge of the window at $y = 0$. This occurred because previous engine text bounding box refactors properly aligned the local $(0, 0)$ coordinate to the top-left of the glyph bounding box, but the demo scenes still rely on old positions that previously compensated for an ad-hoc engine offset hack.
2. **Massive Demo Code Duplication**:
   - Every single demo layer independently implements an identical ~40-line Dear ImGui `"Navigation Menu"` window in `onGuiRender()`.
   - Grid showcase scenes declare dozens of loose member variables (`m_shape1` through `m_shape13` and `m_text1` through `m_text13`), including unused variables, and execute up to 24 repetitive manual draw calls in `onRender()`.
   - Every showcase scene repeats the same 6-line coordinate drawing routine to render the 4x4 background grid.
   - Every demo layer duplicates identical discrete keyboard input checks (`Key::Left` and `Key::Right`) inside `onUpdate()` to cycle between layers.

This makes the demo harness brittle, bloated, difficult to navigate, and prone to visual regression.

## Solution

1. **Correct Demo Label Anchoring**: Anchor showcase labels to top-center $(w/2, 0)$ and position them within each cell's header region (e.g. $y = 20, 170, 320$), ensuring local $(0, 0)$ remains top-left while labels are centered horizontally above each shape and hang cleanly below the grid lines.
2. **Dedicated Demo Utilities Seam**: Introduce a shared demo helper module within the demo harness namespace to provide:
   - A single, centralized ImGui navigation menu renderer.
   - A shared background grid drawing utility.
   - A factory helper to construct top-center aligned showcase labels in a single line.
   - A shared layer navigation handler that processes Left/Right arrow key cycling in a single line.
3. **Containerized Showcase Items**: Replace loose shape and text member variables in showcase scenes with a structured container of items drawn in a clean loop, while preserving fast direct access for dynamically animated shapes.

## User Stories

1. As an engine developer testing rendering capabilities, I want showcase title labels to be cleanly positioned inside each grid cell without intersecting grid lines, so that I can easily read shape descriptions.
2. As an engine developer, I want the first row of showcase labels to be fully visible below the top window border, so that shape titles are never cut off.
3. As a developer navigating the demo, I want showcase labels to be horizontally centered above each showcase item, so that visual alignment remains balanced and intuitive.
4. As a demo maintainer, I want a single centralized function to render the ImGui navigation menu across all layers, so that changing or adding a layer only requires modifying one menu definition.
5. As a demo maintainer, I want all demo layers to share the same navigation menu appearance and behavior, so that user experience is consistent across every scene.
6. As a demo maintainer, I want showcase items (shape and text) in grid layers to be stored in structured collections rather than loose individual fields, so that class declarations are compact and easy to read.
7. As a demo maintainer, I want showcase items to be drawn via a concise loop in `onRender()`, so that adding, reordering, or removing showcase items does not require writing manual draw calls.
8. As a demo maintainer, I want unused shape and text variables to be eliminated, so that the codebase contains no dead or misleading state.
9. As an engine developer writing animation showcases, I want rotating or animated shapes within containers to be directly accessible without expensive lookups, so that `onUpdate()` remains fast and simple.
10. As a demo maintainer, I want the 4x4 background grid to be drawn via a shared helper function, so that grid line coordinates and styling are not duplicated across multiple showcase scenes.
11. As a demo maintainer, I want a shared helper to construct showcase labels with standard scaling and top-center origin in one call, so that showcase initialization code is concise and standardized.
12. As a demo maintainer, I want Left/Right arrow key layer cycling logic to be encapsulated in a single shared helper, so that `onUpdate()` methods avoid repetitive multi-line branching.
13. As an engine developer, I want the demo harness refactoring to touch only the demo harness and require zero changes to public engine headers or engine subsystems, so that engine architecture remains decoupled.
14. As an engine developer running continuous integration, I want the unified verification gate (`verify.ps1` / `verify.sh`) to pass 100% with zero compiler warnings and clean conventions, so that demo improvements maintain repository standards.

## Implementation Decisions

1. **Top-Center Showcase Label Alignment**:
   - Labels in grid showcase scenes will have their origin set to `(globalWidth / 2.0f, 0.0f)`.
   - Vertical positions will be placed at $y = 20.0f$ for row 0, $y = 170.0f$ for row 1, and $y = 320.0f$ for row 2, placing labels inside the header gap above the shapes ($y = 75, 225, 375$).
   - Horizontal positions will remain at column centers ($x = 100, 300, 500, 700$).

2. **Demo Utilities Module (`demo` namespace in demo harness)**:
   - Introduce a shared demo header and implementation file inside the demo harness source tree.
   - Encapsulate all shared demo functions within the `demo` namespace.
   - Provide `demo::renderNavigationMenu(GameLayerManager&)` to render the ImGui layer selection window.
   - Provide `demo::drawGrid(Renderer&)` to draw the standard 4x4 grid lines.
   - Provide `demo::createShowcaseLabel(const Ref<Font>&, string_view, const Vector2f&)` returning a scaled ($0.25$), top-center anchored `Text` object.
   - Provide `demo::handleLayerNavigation(LayerContext&, string_view prevLayer, string_view nextLayer)` to cycle active layers on Left/Right arrow key presses.

3. **Showcase Containerization (`RectanglesTest`, `CirclesTest`, `SpritesTest`)**:
   - In each grid showcase layer, define a local `GridItem` structure pairing the respective drawable shape and its label.
   - Store items in a `std::vector<GridItem>`.
   - In `onRender()`, iterate over the container to draw all shapes and labels.
   - For items animated in `onUpdate()` (such as rotating shapes), store a direct pointer to the shape within the container during setup, allowing `onUpdate()` to call `.rotate(...)` without search overhead.
   - Replace manual `drawLines()` with `demo::drawGrid(m_layerContext.renderer)`.
   - Replace manual ImGui menu code with `demo::renderNavigationMenu(m_layerContext.gameLayerManager)`.
   - Replace manual arrow key branches with `demo::handleLayerNavigation(m_layerContext, prev, next)`.

4. **Engine Isolation**:
   - All changes are strictly confined to the demo harness. No changes are permitted to `OrcEngine` headers or source files.

## Testing Decisions

- **Testing Seam**: The highest-level existing testing seam is the unified verification gate (`scripts/verify.ps1` on Windows, `scripts/verify.sh` on Linux):
  - Stage 1: C++ coding conventions linter checks that all new and modified demo files follow tab indentation, Allman braces, and header guards.
  - Stage 2: CMake build confirms zero warnings and zero compilation/linker errors across both engine and demo targets.
  - Stage 3: Demo smoke test launches the demo executable with a 5-second timeout and fails on any crash or error-level log.
- **Visual Smoke Verification**: Launch the demo executable and switch to `RectanglesTest`, `CirclesTest`, and `SpritesTest` to verify that labels are cleanly legible, completely visible, and do not collide with grid lines.

## Out of Scope

- Modifying core engine rendering, text geometry, font rasterization, or transform matrix mathematics.
- Altering public engine interfaces or `LayerContext`.
- Modifying non-grid demo scenes beyond consuming the centralized navigation menu and arrow navigation helpers.
- Creating an engine-level scene graph or generalized UI framework (the helpers are strictly demo-level utilities).

## Further Notes

- This refactor directly resolves the visual defect observed after the `Text` bounding box cleanup and reduces repetitive demo code by an estimated 400+ lines.
- Does not require a new Architecture Decision Record (ADR) as it does not affect engine-level architecture or public APIs, but should be documented in the ticket tracker.
