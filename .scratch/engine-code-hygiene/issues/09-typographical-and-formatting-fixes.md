# Typographical Errors and Code Smells

Status: resolved
Type: task

## Description
Fix spelling errors in error messages and local variables, clean up spacing around assignment operators, and remove duplicate punctuation.

### Target Files & Locations
- [OrcEngine/source/Orc/Graphics/Window.cpp:24](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Window.cpp#L24): Fix typo `occured` -> `occurred` in `glfwErrorCallback`.
- [OrcEngine/source/Orc/Graphics/Text.cpp:115, 170](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Text.cpp#L115): Fix typo `positionyY` -> `positionY` (or `originalPosY`).
- [OrcEngine/source/Orc/Graphics/Font.cpp:70](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Font.cpp#L70): Add missing space in `uint32_t maxDimension = ...`.
- [OrcEngine/include/Orc/Engine/Logger.hpp:29](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Logger.hpp#L29): Remove redundant double semicolon `;;` at end of line.

### Completion Criteria
1. Typos in error logs and variables are corrected.
2. Formatting and punctuation defects are eliminated.

## Answer

Fixed typo occured -> occurred in Window.cpp, fixed typo positionyY -> positionY in Text.cpp, fixed missing space around assignment in Font.cpp, and removed redundant double semicolon in Logger.hpp
