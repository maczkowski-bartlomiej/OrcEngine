# 08: Fix Rendering, Text Bounding Box, and Font Management Runtime Defects

Status: resolved
Type: task
Blocked by: 07

**What to build:** Fix critical runtime defects and resource leaks in rendering, text, and font systems:
1. `Texturable` sub-rectangle UV calculation:
   - In `OrcEngine/source/Orc/Graphics/Texturable.cpp` (`Texturable::updateVerticesTexCoords`), fix right and bottom UV coordinates to calculate `(m_textureRect.left + m_textureRect.right) / textureSize.x` and `(m_textureRect.top + m_textureRect.bottom) / textureSize.y` (so that offset frames on sprite sheets sample the correct texture region).
2. `Text` and `Font` bounding box and crash fixes:
   - In `OrcEngine/source/Orc/Graphics/Text.cpp`:
     - Reset `m_localRect` at the start of `Text::updateVertices()` before accumulating glyph bounds.
     - In `Text::getLocalRect()`, trigger `updateVertices()` if dirty so that un-rendered text queries return accurate bounds.
     - Correct multi-line and post-transform bounding box calculations.
     - Cast character code via `static_cast<unsigned char>` before passing or converting to `uint32_t` to avoid sign extension on high-ASCII / UTF-8 lead bytes.
   - In `OrcEngine/source/Orc/Graphics/Font.cpp`:
     - In `Font::getCharacter()`, check `character >= CHARACTER_COUNT` and return a safe fallback glyph (e.g. `'?'` or index 0) instead of throwing an unhandled `std::out_of_range` exception.
     - Check `FT_Load_Char` return codes with error reporting.
     - Remove the unused `buffer` vector in `Font::calculateBitmap()`.
3. Shader object leak:
   - In `OrcEngine/source/Orc/Graphics/Shader.cpp` (`Shader::compile`), call `glDeleteShader(vertexShader)` and `glDeleteShader(fragmentShader)` after `glDetachShader` on the successful link path, preventing driver memory leaks.
4. Verify with `./scripts/verify.sh` and ensure sprite and text demo scenes render accurately.

**Blocked by:** 07 (Format Uniform Control Statement Braces and Secure Debug Macros)

- [x] `Texturable::updateVerticesTexCoords` computes correct sub-rectangle UVs for offset texture regions
- [x] `Text::updateVertices()` resets `m_localRect` before accumulating bounds
- [x] `Text::getLocalRect()` triggers vertex update when dirty
- [x] `Text` calculates accurate multi-line and transformed bounding boxes
- [x] Character conversion casts to `unsigned char` before `uint32_t`
- [x] `Font::getCharacter()` safely handles characters `>= CHARACTER_COUNT` with a fallback glyph
- [x] `FT_Load_Char` return code checked and handled
- [x] Unused `buffer` in `Font::calculateBitmap()` removed
- [x] `Shader::compile` deletes shader objects after detach on successful link path
- [x] Passes `./scripts/verify.sh`

## Answer

Fixed Texturable sub-rect UV calculation, Text bounds accumulation and dirty check, safe Font character access and FT_Load_Char check, and Shader link OpenGL object leak
