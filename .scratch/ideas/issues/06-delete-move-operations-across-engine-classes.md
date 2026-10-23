# Delete Move Operations Across Resource and Subsystem Classes

Status: needs-triage
Type: grilling

## Raw Proposal

Delete move constructors and move assignment operators (`= delete`) alongside copy operations across engine subsystems and resource-owning classes (matching `Audio`, `VertexArray`, and `Scripting`) rather than writing manual move boilerplate that zeroes native handles.

## Context

- **Source:** Discovered while reviewing `dotnet-scripting-simplification` (`AGENTS.md` Section 5.4, `Texture.hpp`, `Shader.hpp`, `VertexBuffer.hpp`, `IndexBuffer.hpp`, `Window.hpp`, `Renderer.hpp`, `Gui.hpp`)
- **Captured:** 2026-10-23

## Frontier Seeds

- ❓ Since resources (`Texture`, `Shader`, `VertexBuffer`, `IndexBuffer`, `Font`) are managed on the heap via `Ref<T>` and subsystems (`Window`, `Renderer`, `Gui`, `Audio`, `Scripting`) via `UniquePtr<T>`, are any resource-owning classes ever moved by value?
- ❓ Should `Texture`, `Shader`, `VertexBuffer`, and `IndexBuffer` delete their move constructors and move assignment operators (`= delete`) like `VertexArray` and `Audio`?
- ❓ Should `AGENTS.md` Section 5.4 be updated from requiring custom move constructors/operators on resource-owning classes to deleting both copy and move operations (`= delete`)?
