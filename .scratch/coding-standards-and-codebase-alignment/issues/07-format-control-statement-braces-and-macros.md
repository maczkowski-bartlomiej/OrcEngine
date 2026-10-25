# 07: Format Uniform Control Statement Braces and Secure Debug Macros

Status: resolved
Type: task
Blocked by: 06

**What to build:** Enforce Allman bracing on all control flow statements and secure debug macro expansions:
1. Enclose all single-statement control flow bodies across `OrcEngine` and `TestProject` in Allman braces (`{` on a new line):
   - Wrap single-statement `if`, `else`, `for`, `while`, and `do` bodies in `{ ... }`.
2. Clean up `switch` `case` blocks:
   - Remove unnecessary block braces around `case` labels unless required for variable scoping.
   - Ensure an explicit `default: break;` is provided where appropriate.
3. Nested namespaces:
   - In `OrcEngine/include/Orc/Input/Keyboard.hpp`, `Keyboard.cpp`, `Mouse.hpp`, and `Mouse.cpp`, use modern C++20 `namespace orc::glfw { ... }` syntax without empty namespace stubs or trailing semicolons.
4. Debug macros:
   - In `OrcEngine/include/Orc/Engine/Debug.hpp`, wrap all multi-statement debug macros (`ORC_ASSERT`, `ORC_FATAL`, `ORC_FATAL_CHECK`, etc.) in standard `do { ... } while (0)` blocks to ensure safe syntactic expansion in unbraced or macro-nested contexts.
5. Verify with `./scripts/lint-conventions.sh` and `./scripts/verify.sh`.

**Blocked by:** 06 (Modernize C++20 Parameter Passing, Observer Getters, and Constructor Safety)

- [x] All single-statement `if`, `else`, `for`, `while`, and `do` blocks across `OrcEngine` and `TestProject` use Allman braces
- [x] Redundant `switch` `case` block braces removed where variable scoping is not needed
- [x] `Keyboard` and `Mouse` use `namespace orc::glfw` syntax
- [x] Multi-statement debug macros in `Debug.hpp` are wrapped in `do { ... } while (0)`
- [x] Passes `./scripts/lint-conventions.sh` and `./scripts/verify.sh`

## Answer

Enforced Allman bracing on all control flow statements across OrcEngine and TestProject; cleaned up case blocks; updated Keyboard and Mouse to namespace orc::glfw; wrapped all multi-statement debug macros in do while(0); passed 100% verification
