# 02: Untemplate Texturable and Refactor 2D Shapes

Status: ready-for-agent
Type: task

**What to build:**
Refactor `Texturable` into a clean, non-templated `Transformable` base class holding `std::array<Vertex, 4>`. Prune dead methods `setVertices` and `getVertex`. Move implementation logic to `Texturable.cpp` and delete `Texturable.inl`. Refactor `Sprite`, `Rectangle`, and `Circle` to inherit from non-templated `Texturable` and configure their shape vertex parameters directly in `m_vertices`.

**Blocked by:** 01

## Acceptance criteria

- [ ] `class Texturable : public Transformable` is non-templated in `OrcEngine/include/Orc/Graphics/Texturable.hpp`.
- [ ] `Texturable` holds `mutable std::array<Vertex, 4> m_vertices`.
- [ ] Dead methods `setVertices` and `getVertex` are pruned from `Texturable`.
- [ ] `Texturable.inl` is deleted and methods implemented in `OrcEngine/source/Orc/Graphics/Texturable.cpp`.
- [ ] `Sprite` inherits from `Texturable` and directly sets vertex color and transformed quad positions.
- [ ] `Rectangle` inherits from `Texturable` and directly sets `fillColor`, `secondaryColor` (`borderColor`), and `borderThickness` with `shapeType = 1.0f`.
- [ ] `Circle` inherits from `Texturable` and sets `fillColor`, `secondaryColor` (`borderColor`), `radius`, `innerRadius`, `borderThickness`, and `localPosition` with `shapeType = 2.0f`.
- [ ] `getVertices()` on shapes returns `const std::array<Vertex, 4>&`.

## Comments
