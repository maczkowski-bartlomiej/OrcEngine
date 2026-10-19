# Engine Code Hygiene and Quality Refactor Map

## Notes
Comprehensive engine-wide quality and hygiene refactoring addressing user inspection items and audit findings across all six core subsystems of OrcEngine.

## Decisions so far
- [14-tab-indentation-and-linter-alignment](issues/14-tab-indentation-and-linter-alignment.md): Converted space-based indentation to tabs across Audio.hpp, Clock.hpp, ResourceHolder.hpp, Utility.hpp, Bus.cpp, Audio.cpp, EntryPoint.cpp, Utility.cpp; removed trailing tab in Config.hpp; enclosed callbacks in namespace orc in EntryPoint.hpp; and updated convention linter to exempt non-symbol units
- [13-rule-of-five-resource-wrappers](issues/13-rule-of-five-resource-wrappers.md): Implemented Rule of 5 move constructor and move assignment operators for VertexBuffer, IndexBuffer, Texture, and Shader, transferring OpenGL IDs and zeroing moved-from instances
- [12-critical-bugs-viewport-and-vbo-binding](issues/12-critical-bugs-viewport-and-vbo-binding.md): Fixed camera viewport resize parameter inversion in GameLayer::onEvent and removed redundant duplicate VBO binding in VertexArray::addVertexBuffer
- [11-logging-macro-standardization](issues/11-logging-macro-standardization.md): Standardized all error logging in Texture.cpp and Shader.cpp to ORC_LOG_ERROR, eliminating direct ORC_ERROR calls
- [10-destructor-override-specifiers](issues/10-destructor-override-specifiers.md): Added explicit override specifier to Texturable destructor in Texturable.hpp
- [09-typographical-and-formatting-fixes](issues/09-typographical-and-formatting-fixes.md): Fixed typo occured -> occurred in Window.cpp, fixed typo positionyY -> positionY in Text.cpp, fixed missing space around assignment in Font.cpp, and removed redundant double semicolon in Logger.hpp
- [08-type-safety-casts-and-vector4d-fix](issues/08-type-safety-casts-and-vector4d-fix.md): Fixed Vector4d definition to 4 components, added static_cast<GLsizei> for stride in VertexArray, removed redundant (void*) casts and passed TextureMode by value in Texture, replaced C-style casts with reinterpret_cast<const char*> in Window.cpp, and replaced (T) with static_cast<T> and marked constructor explicit in Rect.hpp
- [07-shader-parameter-name-synchronization](issues/07-shader-parameter-name-synchronization.md): Synchronized parameter names across Shader.hpp and Shader.cpp to vertexSource and fragmentSource for both loadFromString and compile
- [06-control-flow-and-switch-defaults](issues/06-control-flow-and-switch-defaults.md): Added default case to switch in glfwKeyModsToOrcSpecialKeys, marked unused mods parameter in glfwKeyToOrcKey, used !empty() in VertexArray::addVertexBuffer, and modernized angle normalization to constant-time std::fmod in Transformable::setRotation
- [05-namespace-concatenation-and-cleanup](issues/05-namespace-concatenation-and-cleanup.md): Concatenated nested namespaces to namespace orc::glfw in Mouse.cpp and Keyboard.cpp, removed empty namespace blocks in Debug.hpp and Orc.hpp, and removed stray semicolon in VertexArray.hpp
- [04-translation-unit-linkage](issues/04-translation-unit-linkage.md): Enclosed CapitalizedLevelNamesFormatter in an anonymous namespace in Logger.cpp and verified external linkage of Utility functions.
- [03-const-correctness-and-logical-constness](issues/03-const-correctness-and-logical-constness.md): Const-qualified Texture::bind and Resources::getResource; added const overloads for Sprite::getAnimator, Audio bus getters, and Engine::getLayerContext; preserved logical constness for subsystem coordinators.
- [02-ref-move-and-parameter-passing](issues/02-ref-move-and-parameter-passing.md): Applied std::move to all Ref<T> sinks and converted Renderer observers to const Ref<T>&.
- [01-explicit-constructors](issues/01-explicit-constructors.md): Marked all single-argument and default-valued constructors explicit across engine classes and event structs.
- **Decided**: Make all single-argument constructors (and default-valued multi-argument constructors) `explicit` across engine classes and event structs (except `Event(T&&)`).
- **Decided**: Functions sinking `Ref<T>` by value must use `std::move` into member storage; observer functions that do not retain ownership must accept `const Ref<T>&` to avoid ref-count churn.
- **Decided**: Logical constness takes precedence over handle constness: methods modifying subsystem state, swapping buffers, or uploading GPU memory must remain non-const; pure inspectors and binding calls must be `const`.
- **Decided**: Multi-unit utility functions (`fmodCall`, `ftCall`, `getErrnoMessage`) must retain external linkage in `namespace orc`; local unexported classes (`CapitalizedLevelNamesFormatter`) belong in anonymous namespaces.
- **Decided**: Fix critical bugs uncovered during inspection: `GameLayer::onEvent` camera viewport inverted assignment, `VertexArray::addVertexBuffer` duplicate binding call, and `Vector4d` 3-component typo.
- **Decided**: Implement Rule of 5 move semantics for OpenGL wrapper classes (`VertexBuffer`, `IndexBuffer`, `Texture`, `Shader`).
- **Decided**: Standardize tab indentation and update `lint-conventions.sh` to eliminate false positives on entry points.

## Work Breakdown
- [01-explicit-constructors](issues/01-explicit-constructors.md): Explicit Constructors for Single-Argument and Default-Valued Constructors
- [02-ref-move-and-parameter-passing](issues/02-ref-move-and-parameter-passing.md): Sinking Ref<T> with std::move and Observer Parameter Passing
- [03-const-correctness-and-logical-constness](issues/03-const-correctness-and-logical-constness.md): Const Correctness and Subsystem Logical Constness
- [04-translation-unit-linkage](issues/04-translation-unit-linkage.md): Translation Unit Linkage and Anonymous Namespaces
- [05-namespace-concatenation-and-cleanup](issues/05-namespace-concatenation-and-cleanup.md): Nested Namespace Concatenation and Cleanup
- [06-control-flow-and-switch-defaults](issues/06-control-flow-and-switch-defaults.md): Switch Default Handling and Control Flow Modernization
- [07-shader-parameter-name-synchronization](issues/07-shader-parameter-name-synchronization.md): Parameter Name Synchronization in Shader
- [08-type-safety-casts-and-vector4d-fix](issues/08-type-safety-casts-and-vector4d-fix.md): Explicit Casts, Narrowing Conversions, and Vector4d Bug Fix
- [09-typographical-and-formatting-fixes](issues/09-typographical-and-formatting-fixes.md): Typographical Errors and Code Smells
- [10-destructor-override-specifiers](issues/10-destructor-override-specifiers.md): Derived Class Destructor Override Specifiers
- [11-logging-macro-standardization](issues/11-logging-macro-standardization.md): Logging Macro Standardization to ORC_LOG_ERROR
- [12-critical-bugs-viewport-and-vbo-binding](issues/12-critical-bugs-viewport-and-vbo-binding.md): Critical Viewport Resize and Duplicate VBO Binding Bugs
- [13-rule-of-five-resource-wrappers](issues/13-rule-of-five-resource-wrappers.md): Rule of 5 Move Semantics for OpenGL Wrappers
- [14-tab-indentation-and-linter-alignment](issues/14-tab-indentation-and-linter-alignment.md): Universal Tab Indentation and Convention Linter Alignment

## Fog
- None; all 14 issue areas have been inspected, confirmed, and mapped to specific line ranges.
