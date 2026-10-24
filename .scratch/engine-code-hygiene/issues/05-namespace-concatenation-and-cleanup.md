# Nested Namespace Concatenation and Cleanup

Status: resolved
Type: task

## Description
Adopt C++17 nested namespace concatenation syntax (`namespace orc::glfw`) to eliminate redundant indentation and remove empty namespace blocks and stray semicolons.

### Target Files & Locations
- [OrcEngine/source/Orc/Input/Mouse.cpp:6-8, 26-27](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Input/Mouse.cpp#L6-L8): Concatenate to `namespace orc::glfw { ... }`.
- [OrcEngine/source/Orc/Input/Keyboard.cpp:6-8, 173-174](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Input/Keyboard.cpp#L6-L8): Concatenate to `namespace orc::glfw { ... }`.
- [OrcEngine/include/Orc/Engine/Debug.hpp:10-12](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Debug.hpp#L10-L12): Remove empty `namespace orc {}` with obsolete comment.
- [OrcEngine/include/Orc/Orc.hpp:52-53](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Orc.hpp#L52-L53): Remove empty `namespace orc {}`.
- [OrcEngine/include/Orc/Graphics/VertexArray.hpp:34](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/VertexArray.hpp#L34): Remove stray semicolon `};` after closing brace of namespace.

### Completion Criteria
1. All nested namespaces use concatenated syntax.
2. Empty namespace blocks and stray semicolons are eliminated.
3. Code compiles with zero warnings.

## Answer

Concatenated nested namespaces to namespace orc::glfw in Mouse.cpp and Keyboard.cpp, removed empty namespace blocks in Debug.hpp and Orc.hpp, and removed stray semicolon in VertexArray.hpp
