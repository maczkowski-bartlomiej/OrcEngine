# 02: Synchronize GameLayer Camera on Layer Switch

Status: completed
Type: task
Blocked by: 01

**What to build:** Automatic camera synchronization for inactive game layers upon layer transition, ensuring that layers sleeping during a resize event awaken with up-to-date viewport dimensions before lifecycle execution.

**Blocked by:** 01 (Add Renderer setViewport and Window Framebuffer Support)

- [x] `GameLayer` provides `syncCameraWithWindow()` to align `m_camera` bounds with current window dimensions
- [x] `GameLayer::onEvent` delegates `WindowResizedEvent` to update camera bounds consistently
- [x] `GameLayerManager::setActiveLayer` automatically invokes `syncCameraWithWindow()` on the newly activated layer immediately before calling `onAttach()`
- [x] Inactive layers switched into after a resize render with updated orthographic bounds rather than stale initial dimensions
- [x] The engine static library and demo compile and run with zero errors
