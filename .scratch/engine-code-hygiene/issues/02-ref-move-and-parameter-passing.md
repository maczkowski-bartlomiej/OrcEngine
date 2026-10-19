# Sinking Ref<T> with std::move and Observer Parameter Passing

Status: resolved
Type: task

## Description
Functions taking ownership of `Ref<T>` by value should `std::move` into member variables to eliminate redundant atomic reference count increments and decrements. Observer methods in `Renderer` that do not retain ownership of `Ref<T>` should accept `const Ref<T>&` instead of copying `shared_ptr` by value.

### Target Files & Locations
- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:20-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L20-L23): `m_indexBuffer = std::move(indexBuffer);`
- [OrcEngine/source/Orc/Graphics/VertexArray.cpp:58](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/VertexArray.cpp#L58): `m_vertexBuffers.push_back(std::move(vertexBuffer));`
- [OrcEngine/source/Orc/Graphics/Texturable.cpp:12-21](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texturable.cpp#L12-L21): `setTexture(std::move(texture));` and `m_texture = std::move(texture);`
- [OrcEngine/source/Orc/Graphics/Text.cpp:11-26](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Text.cpp#L11-L26): `setFont(std::move(font));` and `m_font = std::move(font);`
- [OrcEngine/source/Orc/Graphics/Sprite.cpp:13-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Sprite.cpp#L13-L23): `setTexture(std::move(texture));` and `Sprite(std::move(texture))`
- [OrcEngine/source/Orc/Graphics/Animator.cpp:13-16](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Animator.cpp#L13-L16): `m_animations[name] = std::move(animation);`
- [OrcEngine/source/Orc/Engine/GameLayerManager.cpp:41](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/GameLayerManager.cpp#L41): `m_layers.try_emplace(name, std::move(gameLayer));`
- [OrcEngine/include/Orc/Graphics/Renderer.hpp:41, 43, 56, 60](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Renderer.hpp#L41-L60) & [OrcEngine/source/Orc/Graphics/Renderer.cpp:123, 203, 214, 267](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Renderer.cpp#L123-L267): Change observer parameters from `Ref<T>` by value to `const Ref<T>&`.

### Completion Criteria
1. All sink functions move their `Ref<T>` arguments into member storage.
2. Renderer observer functions take `const Ref<T>&` without reference counting churn.
3. Code compiles cleanly and smoke test runs with 0 errors.

## Answer

Applied std::move to all Ref<T> sinks and converted Renderer observers to const Ref<T>&.
