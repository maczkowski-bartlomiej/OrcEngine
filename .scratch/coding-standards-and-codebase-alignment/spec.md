# Spec: Coding Standards Update & Codebase Alignment

Status: ready-for-agent

## Problem Statement

Engine developers and contributors working across OrcEngine and its demo integration harness face drift between documented coding standards and actual codebase conventions, alongside several high-impact runtime bugs and type-safety gaps. Specifically:
- The coding standards document claims C# coverage without defining C# rules, mandates `m_camelCase` without distinguishing class members from plain data struct fields, requires manual `#include "OrcPch.hpp"` directives even though CMake automatically force-includes the precompiled header via `target_precompile_headers`, and prescribes namespace and control-statement brace rules that conflict with the repository's formatter configuration and actual source code.
- Public engine headers leak third-party library headers (such as FMOD and FreeType) into every translation unit, violating module encapsulation.
- Multiple engine files deviate from established naming, parameter passing (`FilePath`, `std::string_view`, `std::span`, `std::initializer_list`), observer getter return types (`const Ref<T>&`), `explicit` constructor, Rule of 5, and C++20 syntax standards.
- Latent bugs exist across rendering, text/font handling, audio, input, and platform utilities—including dangling `Sprite` pointers inside `Animator` after copy/move assignment, broken sub-rectangle texture UV calculation in `Texturable`, stale bounding boxes and out-of-range crashes in `Text` and `Font`, leaked OpenGL shader objects on successful compilation, release-build null pointer dereferences in `Audio`, non-portable Linux-only `errno` mappings, multiplicative vs. additive scaling in `Transformable`, unmapped keyboard keys, duplicate entry-point declarations, and global macro pollution in the precompiled header.

## Solution

Update the coding standards documentation, architecture documentation, formatter configuration, and linter scripts to accurately reflect the project's C++20 conventions; optimize the precompiled header and remove redundant manual `#include "OrcPch.hpp"` directives across all implementation files; forward-declare third-party types in public headers; and systematically resolve all identified convention violations and runtime bugs across `OrcEngine` and `TestProject` (while leaving deprecated `ResourceHolder` internals and intentionally retained scaffolding untouched).

## User Stories

1. As an engine developer, I want the coding standards document to clearly scope its rules under a C++20 section without prematurely claiming C# conventions, so that C++ rules are unambiguous until C# scripting conventions are formally defined.
2. As an engine developer, I want the naming standards to distinguish `m_camelCase` private/protected class member variables from unprefixed `camelCase` public fields on plain data structs, so that configuration, event, vertex, rectangle, and `LayerContext` structs conform to the documented standard.
3. As an engine developer, I want the brace formatting rules in the coding standards and clang-format configuration to require Allman braces on classes, functions, and all `if`/`else`/`for`/`while`/`do` control statements (including single-statement bodies), while keeping `namespace` and lambda opening braces on the same line and not requiring braces on `switch` `case` labels, so that control flow formatting is uniform across the codebase.
4. As an engine consumer, I want public engine headers to forward-declare third-party types and keep third-party SDK headers (such as FMOD and FreeType) confined to private implementation files or internal headers, so that including the engine umbrella header does not leak external SDK headers into consumer translation units.
5. As an engine developer, I want observer getters returning shared engine resources (`Ref<T>`) to return `const Ref<T>&` rather than `Ref<T>` by value in both the coding standards and codebase, and logical constness rules to accurately describe why OpenGL bind/uniform operations are `const`, so that transient draw calls and frame loops avoid unnecessary atomic reference-count churn.
6. As an engine developer, I want the precompiled header to contain only clean, frequently used standard library and core engine headers—without polluting Windows builds with `<Windows.h>` or stray `#define NO` macros—and I want manual `#include "OrcPch.hpp"` directives removed from all `.cpp` files in favor of CMake's automatic precompiled header injection, so that compilation is fast, clean, and macro-safe.
7. As an engine developer, I want enum values, constants, type aliases, and `Viewport` member names to strictly follow `PascalCase`, `UPPER_SNAKE_CASE`, and glossary spelling (`viewport` / `m_viewport`), so that identifier naming is 100% consistent.
8. As an engine developer, I want file paths stored and passed as `FilePath`, read-only strings passed as `std::string_view`, contiguous buffers passed via `std::span`, and `std::initializer_list` passed by value, so that parameter passing adheres to modern C++20 zero-overhead guidelines.
9. As an engine consumer, I want `Color` and `BufferLayout` constructors to be marked `explicit`, so that single-number arguments do not silently convert into unintended colors or buffer layouts.
10. As an engine developer, I want overriding destructors in demo layers to be marked `override`, nested namespaces to use concatenated `namespace orc::glfw` syntax, and `.cpp`-local constants and callbacks to reside in anonymous namespaces, so that modern C++20 linkage and inheritance contracts are enforced.
11. As an engine developer, I want classes owning OS, OpenGL, ImGui, or FMOD handles (`Engine`, `Window`, `Gui`, `Bus`, `Renderer`, `VertexArray`) to properly delete or implement copy and move operations with correct reference return types on assignment operators, so that accidental copying or moving cannot corrupt native handles or callback pointers.
12. As a game developer, I want copying or moving a `Sprite` to properly rebind its embedded `Animator` to the destination `Sprite` instance, so that updating animations on assigned sprites does not dereference a dangling stack pointer.
13. As a game developer, I want `Texturable` sub-rectangle UV coordinates to compute right and bottom texture coordinates as `(left + width) / textureWidth` and `(top + height) / textureHeight`, so that sprite sheet frames located at non-zero offsets render the correct region of the texture.
14. As a game developer, I want `Text` and `Font` to reset local bounds before recalculating geometry, trigger lazy geometry updates when querying `getLocalRect()`, compute accurate multi-line and transformed bounding boxes, and safely handle boundary character codes (`>= CHARACTER_COUNT`) without throwing unhandled exceptions, so that text rendering and bounds queries are reliable.
15. As a graphics developer, I want `Shader` compilation to call `glDeleteShader` after `glDetachShader` on the successful link path, so that compiled vertex and fragment shader objects are not leaked in the OpenGL driver.
16. As an engine developer, I want `Audio` bank loading and event playback to validate pointers and return statuses in both Debug and Release configurations, so that missing audio banks or invalid event paths fail gracefully instead of causing null pointer dereferences in Release builds.
17. As an engine developer, I want `getErrnoMessage` to use the standard portable `<system_error>` facility rather than hardcoded Linux error numbers, so that error reporting produces accurate messages on both Windows and Linux.
18. As an engine developer, I want `Scripting` to store its `hostfxr` function pointers as instance members rather than file-scope globals, and `EntryPoint` to own the `getEngineConfig` and `onEngineStart` declarations without duplicating them in `Engine` or declaring `main` in a header, so that subsystem state and entry-point contracts are clean and standards-compliant.
19. As a game developer, I want `Transformable::scale` to multiply scale factors rather than adding them, and `Keyboard` input mapping to clean up unmapped symbol keys and unused reset state, so that transforms and input behave predictably.
20. As an engine developer, I want multi-statement debug macros wrapped in `do { ... } while (0)` blocks, and architecture/linter documentation kept synchronized with actual ADR filenames, directory paths, and trailing-whitespace checks, so that macro expansion and repository tooling are rock-solid.

## Implementation Decisions

- **Coding Standards & Documentation Updates**:
  - Organize existing C++ conventions in `CODING_STANDARDS.md` under an explicit C++20 heading and remove premature claims of C# coverage until C# scripting expands.
  - Clarify in the naming table that `m_camelCase` applies to class member variables, whereas public fields of plain data structs (`Config`, `LayerContext`, `Vertex`, `Color`, `Rect`, `AudioSettings`, `VideoSettings`, `SpecialKeys`, and `Event` types) use unprefixed `camelCase`.
  - Define brace formatting rules to match the project formatter: Allman braces (`{` on a new line) are mandatory for classes, structs, enums, functions, and all `if`, `else`, `for`, `while`, and `do` statements (including single-statement bodies). `namespace` declarations and lambdas place `{` on the same line, and `switch` `case` labels do not use block braces unless needed for variable scoping.
  - Update the smart pointer and logical constness guidelines to specify that observer getters returning `Ref<T>` must return `const Ref<T>&` (avoiding atomic ref-count churn) and clarify that OpenGL bind/uniform methods are marked `const` because they do not mutate the logical state of the C++ asset object.
  - Update the precompiled header rule in `CODING_STANDARDS.md`, `AGENTS.md`, and `.clang-format` to reflect that `OrcPch.hpp` is automatically force-included by CMake (`target_precompile_headers`) and must not be manually `#include`d in `.cpp` files.
  - Require public engine headers to remain free of third-party SDK headers (FMOD, FreeType, OS headers, .NET hosting headers) by using forward declarations in public class headers and keeping SDK-specific helpers internal.
- **Precompiled Header Optimization & Manual Include Removal**:
  - Remove `#include "OrcPch.hpp"` from every `.cpp` file in `OrcEngine/source/Orc/`.
  - Remove `#define NO` and `<Windows.h>` from `OrcPch.hpp`, moving `<Windows.h>` (with lean Win32 defines) into `Scripting.cpp` on Windows (the only translation unit that calls Win32 DLL APIs).
  - Prune unused standard library headers (`<any>`, `<ranges>`, `<unordered_set>`, `<fstream>`) from `OrcPch.hpp` (adding `<fstream>` directly in `Shader.cpp`), and add commonly used standard headers (`<span>`, `<filesystem>`, `<array>`, `<algorithm>`, `<cmath>`, `<optional>`, `<utility>`) to `OrcPch.hpp`.
- **Header Encapsulation & Forward Declarations**:
  - Forward-declare `FMOD::Studio::System`, `FMOD::Studio::Bank`, and `FMOD::Studio::Bus` in the public `Audio` and `Bus` headers instead of including `<fmod_studio.hpp>`.
  - Remove `<fmod_common.h>` and `<freetype/*.h>` from the public `Utility.hpp` header by relocating `fmodCall` and `ftCall` declarations out of public headers (or into internal/private scope).
  - Add missing `<source_location>` includes where `std::source_location` appears in header signatures.
- **Naming, Parameter Passing, Observer Getters, `explicit`, and C++20 Syntax Alignment**:
  - Rename `TextureMode` enum constants from `UPPER_SNAKE_CASE` to `PascalCase` (`Red`, `Green`, `Blue`, `Rgb`, `Rgba`).
  - Rename `M_CHARACTER_COUNT` and `M_SPEED` to `CHARACTER_COUNT` and `SPEED`.
  - Rename `clock`, `time_point`, `string_view_hash`, and `hash_type` to `PascalCase` (`ClockType`, `TimePoint`, `StringViewHash`, `HashType`).
  - Rename `viewPort` and `m_viewPort` in `Camera` to `viewport` and `m_viewport`.
  - Change path fields in `Config` and the `Logger::init` parameter from `std::string` to `FilePath`.
  - Change read-only string parameters in `GameLayerManager`, `Animator`, `Text`, `Window::setTitle`, and `fmodCall` from `const std::string&` to `std::string_view`.
  - Use `std::span<const ...>` for contiguous buffer parameters in `IndexBuffer`, `VertexBuffer`, `Texture::loadFromMemory`, and `Shader::uploadUniformIntArray` (updating `Renderer` call sites accordingly).
  - Pass `std::initializer_list` by value in `BufferLayout` and mark `BufferLayout` and `Color` constructors `explicit` (updating single-argument `Color(255)` call sites in `TestProject`).
  - Change observer getters (`GameLayerManager::getActiveLayer`, `Texturable::getTexture`, `Text::getFont`, `Font::getBitmap`, `Font::getCharacter`) to return `const Ref<T>&` (or `const Character&`).
  - Add `override` to all derived `GameLayer` destructors in `TestProject`, use `namespace orc::glfw` nested namespace syntax in `Keyboard` and `Mouse` headers, wrap all single-statement `if`/`for` bodies in Allman braces across the codebase, remove unnecessary braces on `switch` `case` labels, and wrap `.cpp`-local constants/helpers in anonymous namespaces.
- **Rule of 5 & Subsystem Ownership Safety**:
  - Fix `VertexArray` deleted assignment operators to return `VertexArray&`.
  - Delete copy and move operations on non-movable handle/callback-owning subsystems (`Engine`, `Window`, `Gui`, `Renderer`) and define/delete copy and move operations appropriately on `Bus`.
  - Implement custom copy and move constructors and assignment operators on `Sprite` so that its embedded `Animator` always binds its `m_sprite` pointer to the owning `Sprite` (`this`).
- **Runtime Bug Fixes**:
  - **Texturable UVs**: Fix `Texturable::updateVerticesTexCoords` to compute right and bottom UVs as `(m_textureRect.left + m_textureRect.right) / textureSize.x` and `(m_textureRect.top + m_textureRect.bottom) / textureSize.y`.
  - **Text & Font**: Reset `m_localRect` at the start of `Text::updateVertices()`, trigger `updateVertices()` inside `Text::getLocalRect()` when dirty, fix multi-line and post-transform bounding box calculations, cast `char` via `static_cast<unsigned char>` before `uint32_t`, check `character >= CHARACTER_COUNT` in `Font::getCharacter()` with a safe fallback glyph instead of throwing `std::out_of_range`, check `FT_Load_Char` with `ftCall`, and remove the unused `buffer` vector in `Font::calculateBitmap()`.
  - **Shader Leak**: Call `glDeleteShader(vertexShader)` and `glDeleteShader(fragmentShader)` after `glDetachShader` on the successful compilation path in `Shader::compile`.
  - **Audio Null Safety**: Check FMOD return values and pointer validity (`bank`, `eventDescription`, `eventInstance`) in `Audio::loadBank` and `Audio::play` in both Debug and Release builds.
  - **Portable Errno**: Replace the hardcoded Linux `errno` switch in `getErrnoMessage` with `std::system_category().message(error)`.
  - **Scripting & EntryPoint**: Move `hostfxr` function pointers from file-scope globals in `Scripting.cpp` into private members of `Scripting`. Keep `getEngineConfig()` and `onEngineStart(Engine&)` exclusively in `EntryPoint.hpp`, removing their duplicate declarations from `Engine.hpp` and removing `int main(int argc, char** argv);` from `EntryPoint.hpp`.
  - **Transformable & Input**: Change `Transformable::scale` to multiply `m_scale` rather than adding to it. Remove unmapped shifted symbol keys from `Keyboard::Key`.
  - **Debug Macros**: Wrap multi-statement macros in `Debug.hpp` in `do { ... } while (0)`.
- **Documentation & Linter Synchronization**:
  - Fix broken ADR and directory references in `AGENTS.md` (updating the ADR 0003/0004 links, `OrcPch.hpp` path, and `Orc/Events/` + `Orc/Input/` + `Orc/Graphics/Window.hpp` paths).
  - Synchronize `scripts/lint-conventions.ps1` with `scripts/lint-conventions.sh` so both check trailing whitespace.

## Testing Decisions

- **Seam 1: Unified Verification Gate (`scripts/verify.sh` / `scripts/verify.ps1`)**:
  - Executes the three authoritative project verification stages in sequence:
    1. **Conventions Linter (`scripts/lint-conventions.sh` / `scripts/lint-conventions.ps1`)**: Verifies `#pragma once`, `namespace orc` enclosure, tab indentation, and zero trailing whitespace across all `OrcEngine` and `TestProject` files with 0 errors and 0 warnings.
    2. **Zero-Warning Build (`scripts/build.sh` / `scripts/build.ps1`)**: Compiles `OrcEngine`, `OrcEngine.Scripting`, and `TestProject` with strict compiler warnings (`-Wall -Wextra` / `/W4`), verifying CMake automatic `OrcPch.hpp` injection without manual includes, all header forward declarations, `explicit` constructors, `const` correctness, `std::span` / `std::string_view` / `FilePath` signatures, and Rule of 5 declarations.
    3. **Runtime Demo Smoke Test (`scripts/run-demo.sh` / `scripts/run-demo.ps1`)**: Launches `TestProject` for 5 seconds with `--fail-on-error-log`, exercising CoreCLR scripting initialization, window/OpenGL creation, shader compilation and teardown, font atlas generation, text and sprite rendering, audio bank loading, and clean shutdown.

## Out of Scope

- **C# Coding Standards**: Defining C# coding conventions in `CODING_STANDARDS.md` is deferred until C# scripting grows beyond bootstrap scaffolding.
- **`ResourceHolder` Refactoring**: `ResourceHolder` will be replaced by C#-side asset loading in a future milestone and is excluded from error-handling refactoring.
- **`Animator` Architectural Redesign**: Beyond ensuring `Sprite` copy/move operations maintain a valid `m_sprite` pointer, redesigning the `Animator` / `Sprite` relationship is deferred.
- **Dead Code Removal**: Unused fields in `Config`, `ShaderResources`, unused macros in `Debug.hpp`, and placeholder layers/members in `TestProject` are intentionally retained as-is.
