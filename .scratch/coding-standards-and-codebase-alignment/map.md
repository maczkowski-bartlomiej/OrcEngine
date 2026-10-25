# Map: Coding Standards Update & Codebase Alignment

## Overview
Align OrcEngine's documentation, formatter, and linter with authoritative C++20 conventions; optimize the precompiled header and remove manual `#include "OrcPch.hpp"` directives; decouple public headers from third-party SDKs; and systematically fix identified convention deviations and runtime defects across subsystems.

## Decisions so far
- [09-harden-audio-scripting-and-platform-subsystems](issues/09-harden-audio-scripting-and-platform-subsystems.md): Hardened Audio null safety and return codes, portable std::system_category errno reporting, encapsulated Scripting hostfxr function pointers as private instance members, cleaned EntryPoint header contract, multiplied scale factors in Transformable::scale, and pruned unmapped shifted keys in Keyboard::Key
- [08-fix-rendering-text-bounds-and-font-bugs](issues/08-fix-rendering-text-bounds-and-font-bugs.md): Fixed Texturable sub-rect UV calculation, Text bounds accumulation and dirty check, safe Font character access and FT_Load_Char check, and Shader link OpenGL object leak
- [07-format-control-statement-braces-and-macros](issues/07-format-control-statement-braces-and-macros.md): Enforced Allman bracing on all control flow statements across OrcEngine and TestProject; cleaned up case blocks; updated Keyboard and Mouse to namespace orc::glfw; wrapped all multi-statement debug macros in do while(0); passed 100% verification
- [06-modernize-parameter-passing-and-observer-getters](issues/06-modernize-parameter-passing-and-observer-getters.md): Modernized parameter passing across Config, Logger, GameLayerManager, Animator, Text, Window, IndexBuffer, VertexBuffer, Texture, Shader, BufferLayout, Color; updated observer return types in GameLayerManager, Texturable, Text, and Font; added override to TestProject layer destructors; passed 100% verification
- [05-standardize-naming-type-aliases-and-glossary](issues/05-standardize-naming-type-aliases-and-glossary.md): Standardized identifiers and type aliases: TextureMode (Red, Green, Blue, Rgb, Rgba), CHARACTER_COUNT, SPEED, ClockType, TimePoint, StringViewHash, HashType, and Camera viewport / m_viewport.
- [04-harden-rule-of-5-and-ownership-semantics](issues/04-harden-rule-of-5-and-ownership-semantics.md): Enforced Rule of 5 on Engine, Window, Gui, and Renderer by deleting copy/move; specified Bus move semantics; fixed VertexArray operator= return type; and implemented custom Sprite copy/move operations to rebind embedded Animator m_sprite pointers.
- [03-decouple-public-headers-from-third-party-sdks](issues/03-decouple-public-headers-from-third-party-sdks.md): Decoupled public headers from third-party SDKs by forward declaring FMOD Studio types in Audio.hpp and Bus.hpp, moving fmodCall and ftCall to internal AudioUtility.hpp and FontUtility.hpp headers, and removing FMOD/FreeType includes from Utility.hpp.
- [02-optimize-pch-and-remove-manual-includes](issues/02-optimize-pch-and-remove-manual-includes.md): Optimized OrcPch.hpp by removing Windows.h and unused standard headers, added C++20 standard headers, moved lean Windows.h to Scripting.cpp, added fstream in Shader.cpp, removed manual OrcPch.hpp includes across all 25+ cpp files in OrcEngine, and added fallback .NET discovery.
- [01-sync-coding-standards-and-linter](issues/01-sync-coding-standards-and-linter.md): Synchronized CODING_STANDARDS.md, AGENTS.md, .clang-format, and scripts/lint-conventions.ps1 with authoritative C++20 conventions, ADR links, struct vs class naming, Allman braces, and trailing whitespace linting.

## Tickets
- [01: Synchronize Coding Standards, Agent Protocols, Formatter, and Linter](issues/01-sync-coding-standards-and-linter.md) (Status: resolved)
- [02: Optimize Precompiled Header and Eliminate Redundant Manual Includes](issues/02-optimize-pch-and-remove-manual-includes.md) (Status: resolved)
- [03: Decouple Public Headers from Third-Party SDKs via Forward Declarations](issues/03-decouple-public-headers-from-third-party-sdks.md) (Status: resolved)
- [04: Harden Rule of 5 and Subsystem Ownership Semantics](issues/04-harden-rule-of-5-and-ownership-semantics.md) (Status: resolved)
- [05: Standardize Identifier Naming, Type Aliases, and Glossary Vocabulary](issues/05-standardize-naming-type-aliases-and-glossary.md) (Status: resolved)
- [06: Modernize C++20 Parameter Passing, Observer Getters, and Constructor Safety](issues/06-modernize-parameter-passing-and-observer-getters.md) (Status: resolved)
- [07: Format Uniform Control Statement Braces and Secure Debug Macros](issues/07-format-control-statement-braces-and-macros.md) (Status: resolved)
- [08: Fix Rendering, Text Bounding Box, and Font Management Runtime Defects](issues/08-fix-rendering-text-bounds-and-font-bugs.md) (Status: resolved)
- [09: Harden Audio, Scripting, EntryPoint, and Platform Runtime Subsystems](issues/09-harden-audio-scripting-and-platform-subsystems.md) (Status: resolved)
