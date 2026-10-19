# Const Correctness and Subsystem Logical Constness

Status: resolved
Type: task

## Description
Pure inspection and binding methods should be marked `const`, while methods that mutate subsystem state, swap buffers, advance mixer playback, or upload GPU data must remain non-const to preserve logical constness over physical handle constness.

### Target Files & Locations
- [OrcEngine/include/Orc/Graphics/Texture.hpp:33](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L33) & [OrcEngine/source/Orc/Graphics/Texture.cpp:118-121](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Texture.cpp#L118-L121): Mark `void bind(uint32_t slot = 0) const;`
- [OrcEngine/include/Orc/Engine/ResourceHolder.hpp:25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/ResourceHolder.hpp#L25) & [OrcEngine/source/Orc/Engine/ResourceHolder.cpp:18-29](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/ResourceHolder.cpp#L18-L29): Mark `Ref<ResourceType> getResource(std::string_view name) const;`
- [OrcEngine/include/Orc/Graphics/Sprite.hpp:25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Sprite.hpp#L25) & [OrcEngine/source/Orc/Graphics/Sprite.cpp:44-47](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Graphics/Sprite.cpp#L44-L47): Add const overload `const Animator& getAnimator() const;`
- [OrcEngine/include/Orc/Audio/Audio.hpp:45-47](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Audio/Audio.hpp#L45-L47) & [OrcEngine/source/Orc/Audio/Audio.cpp:126-140](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Audio/Audio.cpp#L126-L140): Add const overloads `const Bus& getSfxBus() const;`, `const Bus& getMusicBus() const;`, `const Bus& getMasterBus() const;`
- [OrcEngine/include/Orc/Engine/Engine.hpp:25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Engine.hpp#L25) & [OrcEngine/source/Orc/Engine/Engine.cpp:125-128](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Engine.cpp#L125-L128): Add const overload `const LayerContext& getLayerContext() const;`
- **Subsystem Logical Constness Preservations**:
  - `Window::display()` and `Window::setCallbacks()` remain non-const.
  - `Gui::begin()` and `Gui::end()` remain non-const.
  - `Audio::play()` and `Audio::update()` remain non-const.
  - `Bus::stop()`, `pause()`, `resume()`, `setVolume()` remain non-const.
  - `VertexBuffer::setData()` remains non-const.

### Completion Criteria
1. Pure inspection methods are const-qualified and missing const overloads are added.
2. Logical constness is maintained across all engine subsystem coordinators.
3. Code compiles cleanly with 0 errors.

## Answer

Const-qualified Texture::bind and Resources::getResource; added const overloads for Sprite::getAnimator, Audio bus getters, and Engine::getLayerContext; preserved logical constness for subsystem coordinators.
