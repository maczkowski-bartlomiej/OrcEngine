# Refactor Demo and Fix Text Map

## Notes
Consolidate repetitive demo code in TestProject, introduce shared DemoUtilities, containerize grid showcase layers, and fix showcase label positioning and origin anchoring.

## Decisions so far
- [02-containerize-grid-showcase-and-fix-labels](issues/02-containerize-grid-showcase-and-fix-labels.md): Refactored RectanglesTest, CirclesTest, and SpritesTest to containerize items in std::vector<GridItem> with loop rendering, anchored labels to top-center at y=20, 170, 320, removed loose members and drawLines, and tracked rotating shapes via direct pointers.
- [01-shared-demo-utilities-and-navigation](issues/01-shared-demo-utilities-and-navigation.md): Introduced DemoUtilities.hpp and DemoUtilities.cpp providing centralized ImGui navigation menu, 4x4 coordinate grid drawer, top-center label factory, and arrow-key cycling helper. Migrated non-grid demo layers.
- [01-shared-demo-utilities-and-navigation](issues/01-shared-demo-utilities-and-navigation.md): 01: Introduce Shared Demo Utilities & Migrate Navigation Across Demo Layers
- [02-containerize-grid-showcase-and-fix-labels](issues/02-containerize-grid-showcase-and-fix-labels.md): 02: Containerize Grid Showcase Layers & Fix Label Placement

## Fog
None.
