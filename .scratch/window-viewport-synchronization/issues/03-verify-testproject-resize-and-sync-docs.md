# 03: Verify TestProject Resize Integration and Sync Documentation

Status: completed
Type: task
Blocked by: 02

**What to build:** End-to-end verification of window resizing and layer transitions across demo scenes in `TestProject`, along with complete synchronization of domain documentation and architectural records.

**Blocked by:** 02 (Synchronize GameLayer Camera on Layer Switch)

- [x] Window resizing during interactive demo execution keeps 1:1 pixel coordinates and avoids distortion or clipping across scenes (`CameraTest`, `SpritesTest`, `Menu`)
- [x] Switching layers back and forth after window resize retains correct camera projection and viewport bounds
- [x] `AGENTS.md` is updated with documentation for `Renderer::setViewport`, `Window` framebuffer accessors, and `GameLayerManager` camera synchronization before `onAttach()`
- [x] `GLOSSARY.md` is updated with terms for Viewport and Framebuffer dimensions
- [x] An Architectural Decision Record is created under `docs/adr/` capturing the rationale for centralized viewport ownership and pre-attach camera synchronization
- [x] The full unified verification gate (`.\scripts\verify.ps1`) passes with 100% success (conventions linting, compilation, 5-second smoke test)
