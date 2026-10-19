# Rule of 5 Move Semantics for OpenGL Wrappers

Status: resolved
Type: task

## Description
OpenGL resource-owning classes (`VertexBuffer`, `IndexBuffer`, `Texture`, `Shader`) currently have their move constructor and move assignment operators deleted (`= delete;`), contradicting `AGENTS.md` §5.3 Rule of 5: "Resource-owning classes wrapping OpenGL IDs must explicitly define move constructor + move assignment and delete copy operations." Implement proper move semantics that transfer ownership of the OpenGL ID and leave the source in a valid empty state (`m_rendererID = 0`).

### Target Files & Locations
- [OrcEngine/include/Orc/Graphics/VertexBuffer.hpp:24-27](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/VertexBuffer.hpp#L24-L27) & [OrcEngine/source/Orc/Graphics/VertexBuffer.cpp](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexBuffer.cpp)
- [OrcEngine/include/Orc/Graphics/IndexBuffer.hpp:24-27](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/IndexBuffer.hpp#L24-L27) & [OrcEngine/source/Orc/Graphics/IndexBuffer.cpp](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/IndexBuffer.cpp)
- [OrcEngine/include/Orc/Graphics/Texture.hpp:25-28](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L25-L28) & [OrcEngine/source/Orc/Graphics/Texture.cpp](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp)
- [OrcEngine/include/Orc/Graphics/Shader.hpp:18-21](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shader.hpp#L18-L21) & [OrcEngine/source/Orc/Graphics/Shader.cpp](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Shader.cpp)

### Completion Criteria
1. Move constructor and move assignment operators are implemented for `VertexBuffer`, `IndexBuffer`, `Texture`, and `Shader`.
2. Copy operations remain deleted.
3. Moved-from instances leave OpenGL ID as 0 without leaking or double-deleting resources.
4. Clean build and test execution.

## Answer

Implemented Rule of 5 move constructor and move assignment operators for VertexBuffer, IndexBuffer, Texture, and Shader, transferring OpenGL IDs and zeroing moved-from instances
