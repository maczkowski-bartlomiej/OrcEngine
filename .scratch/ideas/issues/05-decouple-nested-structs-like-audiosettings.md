# Decouple Nested Structs Like AudioSettings

Status: needs-triage
Type: grilling

## Raw Proposal

should structs like AudioSettings that lays in parent class should be decoupled or not

## Context

- **Source:** Standalone (`OrcEngine/include/Orc/Audio/Audio.hpp`, `OrcEngine/include/Orc/Graphics/Window.hpp`)
- **Captured:** 2026-10-22

## Frontier Seeds

- ❓ Should nested configuration structs (e.g., `Audio::AudioSettings`, `Window::VideoSettings`) remain scoped within their host classes or be extracted to standalone headers?
- ❓ Does extracting these structs reduce header dependencies for aggregator headers such as `Orc/Engine/Config.hpp`?
- ❓ How does decoupling affect API ergonomic conventions and namespace organization across the engine?
