# Critical Viewport Resize and Duplicate VBO Binding Bugs

Status: resolved
Type: task

## Description
Fix two critical bugs discovered during engine inspection:
1. In `GameLayer::onEvent`, handling of `WindowResizedEvent` incorrectly passes width to `top` and height to `right` in `setViewportSize`.
2. In `VertexArray::addVertexBuffer`, line 56 issues a redundant duplicate call to `glVertexArrayVertexBuffer` that clobbers binding slot 0 when multiple VBOs are attached.

### Target Files & Locations
- [OrcEngine/source/Orc/Engine/GameLayer.cpp:20](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/GameLayer.cpp#L20): Fix parameter order in `m_camera.setViewportSize(...)` to `(0.0f, 0.0f, static_cast<float>(resized->width), static_cast<float>(resized->height))`.
- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:56](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L56): Remove the duplicate `glVertexArrayVertexBuffer(m_rendererID, 0, vertexBuffer->getRendererID(), 0, bufferLayout.getStride());` call.

### Completion Criteria
1. Window resizing updates camera viewport accurately with `(left=0, top=0, right=width, bottom=height)`.
2. `VertexArray` does not overwrite binding slot 0.
3. Smoke test runs with zero errors.

## Answer

Fixed camera viewport resize parameter inversion in GameLayer::onEvent and removed redundant duplicate VBO binding in VertexArray::addVertexBuffer
