# Explicit Casts, Narrowing Conversions, and Vector4d Bug Fix

Status: resolved
Type: task

## Description
Replace C-style casts with explicit C++ casts, prevent narrowing conversions from unsigned integers to signed OpenGL dimensions, pass small enums by value, and resolve a latent vector dimensionality bug in core type definitions.

### Target Files & Locations
- [OrcEngine/include/Orc/Engine/Core.hpp:86](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Core.hpp#L86): Fix `using Vector4d = glm::tvec3<double>;` to `using Vector4d = glm::tvec4<double>;`.
- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:36](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L36): Add `static_cast<GLsizei>(bufferLayout.getStride())` to prevent narrowing warning.
- [OrcEngine/source/Orc/Graphics/Texture.cpp:71, 73](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp#L71-L73): Remove redundant/C-style `(void*)pixels` cast.
- [OrcEngine/source/Orc/Graphics/Window.cpp:125](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Window.cpp#L125): Replace `(char*)glGetString(...)` with `reinterpret_cast<const char*>`.
- [OrcEngine/include/Orc/Graphics/Rect.hpp:19](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Rect.hpp#L19): Replace `(T)other.left` C-style casts with `static_cast<T>`.
- [OrcEngine/include/Orc/Graphics/Texture.hpp:31](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L31) & [OrcEngine/source/Orc/Graphics/Texture.cpp:78](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp#L78): Pass `TextureMode textureMode` by value instead of `const TextureMode&`.

### Completion Criteria
1. `Vector4d` is correctly defined as a 4-component vector.
2. All C-style casts in graphics and math modules are replaced with C++ explicit casts.
3. Narrowing conversions are explicitly cast with `static_cast<GLsizei>`.
4. Compilation succeeds without conversion or narrowing warnings.

## Answer

Fixed Vector4d definition to 4 components, added static_cast<GLsizei> for stride in VertexArray, removed redundant (void*) casts and passed TextureMode by value in Texture, replaced C-style casts with reinterpret_cast<const char*> in Window.cpp, and replaced (T) with static_cast<T> and marked constructor explicit in Rect.hpp
