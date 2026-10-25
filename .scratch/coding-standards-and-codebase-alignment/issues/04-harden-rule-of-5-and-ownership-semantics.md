# 04: Harden Rule of 5 and Subsystem Ownership Semantics

Status: resolved
Type: task
Blocked by: 02

**What to build:** Enforce RAII, Rule of 5, and pointer integrity across resource and subsystem classes:
1. In `OrcEngine/include/Orc/Graphics/VertexArray.hpp` (and `.cpp`), fix deleted assignment operators to return `VertexArray&` rather than `void`.
2. In handle/callback-owning subsystem classes that cannot be safely copied or moved (`Engine`, `Window`, `Gui`, `Renderer`):
   - Explicitly declare copy constructor, copy assignment, move constructor, and move assignment as `= delete`.
3. In `Bus` (`OrcEngine/include/Orc/Audio/Bus.hpp`), explicitly define or delete copy and move operations appropriately according to its wrapper semantics.
4. In `Sprite` (`OrcEngine/include/Orc/Graphics/Sprite.hpp` and `.cpp`):
   - Implement custom copy constructor, copy assignment operator, move constructor, and move assignment operator.
   - When a `Sprite` is copied or moved, ensure its embedded `Animator` (`m_animator`) rebinds its target `m_sprite` pointer to `this` (the new owning `Sprite`), eliminating dangling pointer dereferences.
5. Verify with `./scripts/verify.sh`.

**Blocked by:** 02 (Optimize Precompiled Header and Eliminate Redundant Manual Includes)

- [x] `VertexArray` assignment operators return `VertexArray& = delete`
- [x] `Engine` explicitly deletes copy and move operations
- [x] `Window` explicitly deletes copy and move operations
- [x] `Gui` explicitly deletes copy and move operations
- [x] `Renderer` explicitly deletes copy and move operations
- [x] `Bus` explicitly specifies copy and move semantics
- [x] `Sprite` implements custom copy and move operations that rebind embedded `Animator` `m_sprite` pointer to `this`
- [x] Passes `./scripts/verify.sh`

## Answer

Enforced Rule of 5 on Engine, Window, Gui, and Renderer by deleting copy/move; specified Bus move semantics; fixed VertexArray operator= return type; and implemented custom Sprite copy/move operations to rebind embedded Animator m_sprite pointers.
