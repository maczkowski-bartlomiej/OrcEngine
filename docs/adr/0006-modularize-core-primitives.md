# Modularize Core Foundational Primitives

Foundational primitives (smart pointers, math vector and matrix aliases, platform detection and debugbreak macros, and engine version constants) were previously aggregated inside `Orc/Engine/Core.hpp`. This placement improperly located universal Layer 0 primitives inside the high-level Engine lifecycle domain, blurred architectural module boundaries, caused circular conceptual dependencies, and forced consumers to include an all-in-one catch-all header even when only requiring basic math or platform macros.

We decided to relocate and modularize foundational definitions into a dedicated, single-responsibility `Orc/Core/` domain:
- `Platform.hpp`: Platform detection (`ORC_PLATFORM_WINDOWS`, `ORC_PLATFORM_LINUX`), debug assertion flags (`ORC_DEBUG_ASSERTS`), debugbreak intrinsics (`ORC_DEBUGBREAK()`), and platform compiler headers.
- `Memory.hpp`: Smart pointer aliases (`Ref<T>`, `UniquePtr<T>`) and factory functions (`createRef<T>`, `createUniquePtr<T>`).
- `Types.hpp`: GLM configuration, math vector and matrix aliases (`Vector2/3/4`, `Matrix`, `Matrix4`), file path alias (`FilePath`), and driver ID definitions (`RendererID`).
- `Version.hpp`: Engine semantic version constants (`MAJOR_VERSION`, `MINOR_VERSION`, `PATCH_VERSION`) scoped within `orc::version`.
- `Core.hpp`: Umbrella aggregator header exporting all four granular headers for consumers requiring full core facilities.

The monolithic legacy header `Orc/Engine/Core.hpp` was permanently eliminated. All subsystem headers, source files, umbrella headers (`Orc/Orc.hpp`), and build comments were migrated to the new granular headers. This establishes clean unidirectional dependency flow, accelerates compilation times by permitting fine-grained includes, and eliminates confusing layering.
