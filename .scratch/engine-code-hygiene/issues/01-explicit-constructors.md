# Explicit Constructors for Single-Argument and Default-Valued Constructors

Status: resolved
Type: task

## Description
Single-argument and multi-argument constructors with default parameters that can be invoked with a single argument allow accidental implicit type conversions. All such constructors across the engine must be marked `explicit` (except for intentional type-erasure wrappers like `Event(T&&)`).

### Target Files & Locations
- [OrcEngine/include/Orc/Graphics/Texture.hpp:22](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texture.hpp#L22): `explicit Texture(const FilePath& filePath);`
- [OrcEngine/include/Orc/Graphics/Texturable.hpp:17](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texturable.hpp#L17): `explicit Texturable(Ref<Texture> texture);`
- [OrcEngine/include/Orc/Graphics/Text.hpp:19-20](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Text.hpp#L19-L20): `explicit Text(const std::string& string);`, `explicit Text(Ref<Font> font);`
- [OrcEngine/include/Orc/Graphics/Sprite.hpp:17](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Sprite.hpp#L17): `explicit Sprite(Ref<Texture> texture);`
- [OrcEngine/include/Orc/Graphics/Font.hpp:24](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Font.hpp#L24): `explicit Font(const FilePath& filePath, uint32_t size = 48);`
- [OrcEngine/include/Orc/Graphics/Camera.hpp:11](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Camera.hpp#L11): `explicit Camera(const FloatRect& viewPort);`
- [OrcEngine/include/Orc/Graphics/Shapes/Circle.hpp:15](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shapes/Circle.hpp#L15): `explicit Circle(float radius, const Color& color = Color());`
- [OrcEngine/include/Orc/Graphics/Shapes/Rectangle.hpp:15](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Shapes/Rectangle.hpp#L15): `explicit Rectangle(const Vector2f& size, const Color& color = Color());`
- [OrcEngine/include/Orc/Graphics/Animator.hpp:28](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Animator.hpp#L28): `explicit Animator(Sprite* sprite);`
- [OrcEngine/include/Orc/Graphics/Window.hpp:30](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Window.hpp#L30): `explicit Window(const VideoSettings& videoSettings);`
- [OrcEngine/include/Orc/Audio/Audio.hpp:32](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Audio/Audio.hpp#L32): `explicit Audio(const AudioSettings& audioSettings);`
- [OrcEngine/include/Orc/Engine/Engine.hpp:20](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Engine.hpp#L20): `explicit Engine(const Config& config);`
- [OrcEngine/include/Orc/Engine/ResourceHolder.hpp:23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/ResourceHolder.hpp#L23): `explicit Resources(const FilePath& xmlPath);`
- [OrcEngine/include/Orc/Events/MouseEvents.hpp:12-14, 21-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Events/MouseEvents.hpp#L12-L23): `explicit MouseButtonPressedEvent(Mouse::Button button);`, `explicit MouseButtonReleasedEvent(Mouse::Button button);`
- [OrcEngine/include/Orc/Events/KeyboardEvents.hpp:13-15, 23-25](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Events/KeyboardEvents.hpp#L13-L25): `explicit KeyboardKeyPressedEvent(...)`, `explicit KeyboardKeyReleasedEvent(...)`

### Completion Criteria
1. All listed constructors are marked `explicit`.
2. Engine and demo projects compile cleanly without unexpected implicit conversion breaks.

## Answer

Marked all single-argument and default-valued constructors explicit across engine classes and event structs.
