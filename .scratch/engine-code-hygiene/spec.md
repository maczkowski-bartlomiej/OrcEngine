# Spec: Engine Code Hygiene and Quality Refactor

## Problem Statement

Engine developers and consumers working with OrcEngine encounter inconsistent syntax, accidental implicit conversions, redundant shared pointer ref-count churn, mixed indentation standards, and latent bugs (such as inverted camera viewport dimensions on window resize and duplicate vertex buffer bindings). In addition, unclear constness semantics and improper linkage declarations risk either breaking public API expectations or causing cross-translation-unit link errors. The codebase requires systematic syntax modernization, type-safety hardening, bug fixes, and formalization of these rules in developer guidelines.

## Solution

A cohesive engine-wide hygiene refactor addressing syntax consistency, type safety, ownership transfer semantics, const-correctness, and latent logic bugs across the six core engine subsystems (Engine, Graphics & Renderer, Audio, Input & Events, GUI, and Resources). The refactor enforces `explicit` on single-argument constructors, introduces `std::move` sinks while preserving zero-copy `const Ref<T>&` observers, upholds logical constness for subsystems, corrects critical viewport and buffer binding flaws, and aligns all code with the tab-indentation and namespace conventions validated by automated linting.

## User Stories

1. As an engine consumer, I want single-argument constructors on graphics objects (such as Texture, Sprite, Font, Camera, and Shapes) to be `explicit`, so that accidental implicit conversions from primitive types or paths do not introduce subtle bugs or unexpected allocations.
2. As an engine consumer, I want single-argument constructors on subsystem coordinators (such as Window, Audio, Engine, and Resources) to be `explicit`, so that configuration objects cannot implicitly instantiate full subsystem coordinators.
3. As an engine consumer, I want event struct constructors to be `explicit`, so that raw input enum codes or coordinates are not accidentally converted into dispatched event instances.
4. As an engine developer, I want functions that take ownership of a shared reference (`Ref<T>`) to sink it using `std::move`, so that redundant atomic reference count increments and decrements are eliminated.
5. As an engine developer, I want pure observer methods in the Renderer that do not take ownership to receive shared resources via `const Ref<T>&`, so that transient draw calls and slot queries do not incur reference-counting churn.
6. As an engine consumer, I want inspection methods such as Texture binding and Resource cache queries to be `const`, so that I can inspect and bind resources from const-qualified contexts.
7. As an engine consumer, I want subsystem coordinators and mutators (such as Window display, Audio playback/update, GUI frame markers, and Bus controls) to remain non-const, so that API constness accurately reflects logical mutation and state changes.
8. As an engine developer, I want file-local helper classes not exposed in headers to reside in anonymous namespaces, so that internal symbol pollution across translation units is prevented.
9. As an engine developer, I want cross-module utility functions (such as third-party error wrapping callbacks) to retain public external linkage in the engine namespace, so that dependent subsystems compile and link without unresolved symbol errors.
10. As an engine developer, I want nested namespace declarations to be concatenated, so that boilerplate indentation and visual noise are reduced.
11. As an engine developer, I want empty namespace stubs and stray semicolons removed from headers, so that the codebase remains clean and warning-free.
12. As an engine developer, I want input switch statements to explicitly handle default cases, so that unexpected key mod masks do not fall through unhandled or trigger compiler warnings.
13. As an engine developer, I want unused parameters in input translation functions to be clearly omitted or marked, so that compiler unused-parameter diagnostics are satisfied.
14. As an engine developer, I want collection size checks in debug assertions to use `.empty()` rather than inspecting `.size()` as a truth value, so that intent is explicit and idiomatic.
15. As an engine developer, I want angle normalization in transform hierarchies to utilize standard mathematical modulo operations rather than while loops, so that large angle offsets are normalized in constant time.
16. As an engine developer, I want function parameter names between header declarations and implementation definitions to be synchronized, so that documentation, IDE tooltips, and code readability match precisely.
17. As an engine developer, I want C-style casts replaced with type-safe C++ casts (`static_cast` / `reinterpret_cast`), so that type safety is enforced and narrowing conversions are made explicit.
18. As an engine developer, I want mathematical vector aliases (such as 4D double vectors) in core types to reference correct vector dimensions, so that downstream math operations do not produce truncation bugs.
19. As an engine consumer, I want trivial enum types passed by value rather than const reference, so that call-site overhead is minimized in accordance with engine passing guidelines.
20. As an engine developer, I want typographical errors in log messages, variable names, and formatting spacing corrected, so that log output and source code maintain professional quality.
21. As an engine developer, I want overriding destructors in polymorphic hierarchies to specify `override`, so that inheritance contracts are explicitly checked by the compiler.
22. As an engine developer, I want all logging statements to consistently use standard engine logging macros (`ORC_LOG_INFO`, `ORC_LOG_WARNING`, `ORC_LOG_ERROR`, `ORC_LOG_FATAL`), so that logging behavior is unified.
23. As a game developer, I want the base GameLayer window resize handler to correctly map width and height into camera viewport bounds, so that resizing the window does not invert or distort the scene projection.
24. As a graphics developer, I want VertexArray buffer binding to avoid duplicate binding calls, so that binding multiple vertex buffers does not clobber binding index zero.
25. As an engine developer, I want resource-owning OpenGL classes to implement move semantics and delete copy operations (Rule of 5), so that resource wrappers conform to engine memory management standards.
26. As an engine developer, I want all engine source and header files to use consistent tab indentation and clean newline terminations, so that automated convention linting passes with zero errors and zero warnings.

## Implementation Decisions

- **Architectural Scoping & Modular Seams**: Changes are organized into 14 cohesive units mapping to specific quality dimensions across the Engine, Graphics, Audio, Input, GUI, and Resource subsystems.
- **Explicit Constructor Invariant**: All constructors that can be called with a single argument across engine classes and event structures must be marked `explicit`. The only permitted exception is type-erasure and polymorphic variant wrapper constructors (such as `Event(T&&)`).
- **Ref Ownership Transfer Contract**:
  - Sink functions (constructors and setters that store a `Ref<T>` into a member field or collection) must accept `Ref<T>` by value and use `std::move` into member storage.
  - Pure observer functions (methods that only query, bind, or inspect a `Ref<T>` without storing it) must accept `const Ref<T>&` or a direct reference (`const T&`).
- **Logical Constness Standard**:
  - Member functions that mutate subsystem state, poll OS events, swap buffers, tick timers, advance audio streams, or upload GPU data must remain non-const, even if internal handles are pointers or driver IDs.
  - Pure inspectors, resource lookups, and OpenGL binding operations that do not modify client-observable state must be `const`.
- **Linkage Rules**:
  - Unexported helper classes defined exclusively within a `.cpp` file must be enclosed in an anonymous namespace.
  - Multi-unit helper functions declared in public engine headers must retain external linkage in `namespace orc`.
- **Modern Syntax & Cleanups**:
  - Nested namespaces must use C++17 concatenation syntax.
  - Redundant namespace declarations, double semicolons, and stray namespace closing semicolons must be removed.
  - Switch statements over bitmasks must include `default: break;`.
  - Transform angle normalization must use `std::fmod` rather than loops.
  - Parameter names in declarations and definitions must match identically.
- **Type Safety & Bug Fixes**:
  - Replace all C-style casts with `static_cast` or `reinterpret_cast`.
  - Fix the 4D double-precision vector alias in core definitions to reference 4 components.
  - Correct the camera viewport dimension assignment in the game layer resize event handler.
  - Remove duplicate VBO binding calls in VertexArray attribute layout configuration.
- **Documentation & Linter Synchronization**:
  - Update `AGENTS.md` to formally document parameter passing (`Ref<T>` sinks vs observers), logical constness invariants, explicit constructor requirements, and casting rules.
  - Update `lint-conventions.sh` to prevent false positive namespace errors on entry points while enforcing tab indentation across all files.

## Testing Decisions

- **Seam 1: Convention Linter (`scripts/lint-conventions.sh`)**:
  - Validates that every header and source file uses tab indentation, contains `#pragma once` where appropriate, and adheres to namespace enclosure rules. Must pass with 0 errors and 0 warnings.
- **Seam 2: Compiler & Linker Verification (`scripts/build.sh`)**:
  - High-level compile and link seam validating that all explicit constructors, const qualifiers, synchronized parameter names, external linkage functions, and type conversions build cleanly under `linux-debug` and `linux-release` presets.
- **Seam 3: Runtime Demo Smoke Test (`scripts/run-demo.sh --timeout 5 --fail-on-error-log`)**:
  - End-to-end runtime seam launching `TestProject` for 5 seconds. Verifies window creation, GLAD/GLFW initialization, dynamic quad batch rendering, camera projection, audio bank playback, and clean teardown with zero errors in `logs/TestProject.log`.

## Out of Scope

- Demo application code (`TestProject/`): Any refactoring of demo assets, test layers, or sandbox scenes.
- Third-party vendor dependencies (`glad`, `glfw`, `imgui`, `stb_image`, `tinyxml2`, `fmod`).
- Major architectural redesigns (such as introducing an ECS, changing the OpenGL renderer to Vulkan, or integrating scripting engines).

## Further Notes

- The detailed line-by-line file locations for each issue group are recorded in `.scratch/code-hygiene-inventory.md`.
- Implementation will proceed through individual tickets scaffolded under `.scratch/engine-code-hygiene/issues/`.
