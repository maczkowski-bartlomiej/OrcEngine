# 05: Standardize Identifier Naming, Type Aliases, and Glossary Vocabulary

Status: resolved
Type: task
Blocked by: 02

**What to build:** Align all identifiers, enum values, type aliases, and constants with project naming standards and domain glossary:
1. In `OrcEngine/include/Orc/Graphics/Texture.hpp` (and call sites across `OrcEngine` and `TestProject`):
   - Rename `TextureMode` enum values from `UPPER_SNAKE_CASE` to `PascalCase`: `Red`, `Green`, `Blue`, `Rgb`, `Rgba`.
2. In `OrcEngine/include/Orc/Graphics/Font.hpp`:
   - Rename `M_CHARACTER_COUNT` to `CHARACTER_COUNT`.
3. In `TestProject/source/AnimationTest.cpp` (and any other occurrences):
   - Rename `M_SPEED` to `SPEED` (or local constant convention).
4. In `OrcEngine/include/Orc/Engine/Clock.hpp` and `OrcEngine/include/Orc/Core/Types.hpp`:
   - Rename typedefs/aliases to `PascalCase`: `clock` -> `ClockType`, `time_point` -> `TimePoint`, `string_view_hash` -> `StringViewHash`, `hash_type` -> `HashType`.
5. In `OrcEngine/include/Orc/Graphics/Camera.hpp` (and `.cpp` and call sites):
   - Rename `viewPort` to `viewport` and `m_viewPort` to `m_viewport`, aligning with ubiquitous glossary spelling.
6. Verify with `./scripts/verify.sh`.

**Blocked by:** 02 (Optimize Precompiled Header and Eliminate Redundant Manual Includes)

- [x] `TextureMode` enum values are `PascalCase` (`Red`, `Green`, `Blue`, `Rgb`, `Rgba`)
- [x] `Font` constant `M_CHARACTER_COUNT` renamed to `CHARACTER_COUNT`
- [x] `AnimationTest` constant `M_SPEED` renamed to `SPEED`
- [x] Clock and hash type aliases renamed to `PascalCase` (`ClockType`, `TimePoint`, `StringViewHash`, `HashType`)
- [x] `Camera` members and parameters use `viewport` / `m_viewport`
- [x] All call sites across `OrcEngine` and `TestProject` updated
- [x] Passes `./scripts/verify.sh`

## Answer

Standardized identifiers and type aliases: TextureMode (Red, Green, Blue, Rgb, Rgba), CHARACTER_COUNT, SPEED, ClockType, TimePoint, StringViewHash, HashType, and Camera viewport / m_viewport.
