# Logging Macro Standardization to ORC_LOG_ERROR

Status: resolved
Type: task

## Description
Standardize all logging macro usage in `OrcEngine` source files to the canonical macros documented in `AGENTS.md` §5.4 (`ORC_LOG_INFO`, `ORC_LOG_WARNING`, `ORC_LOG_ERROR`, `ORC_LOG_FATAL`), eliminating residual legacy `ORC_ERROR` macros.

### Target Files & Locations
- [OrcEngine/source/Orc/Graphics/Texture.cpp:37, 55, 82](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp#L37): Replace `ORC_ERROR(...)` with `ORC_LOG_ERROR(...)`.
- [OrcEngine/source/Orc/Graphics/Shader.cpp:93, 98, 129, 148, 168](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Shader.cpp#L93): Replace `ORC_ERROR(...)` with `ORC_LOG_ERROR(...)`.

### Completion Criteria
1. No direct calls to `ORC_ERROR` remain in engine graphics code.
2. Logging calls use canonical `ORC_LOG_*` macros.

## Answer

Standardized all error logging in Texture.cpp and Shader.cpp to ORC_LOG_ERROR, eliminating direct ORC_ERROR calls
