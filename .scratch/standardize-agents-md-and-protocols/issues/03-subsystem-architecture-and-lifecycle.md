# 03: Subsystem Architecture and Lifecycle (Non-Rendering)

Status: resolved
Type: task
Blocked by: 01

**What to build:** Fill Section 4 of `AGENTS.md` (except the rendering subsystem, which ticket 04 owns) so an agent understands frame sequencing and inter-module communication well enough to put new code in the correct lifecycle phase. Use the older comprehensive `AGENTS.md` (git `7a9cc44`) as a starting point, but every claim must be verified against the current engine source.

## Acceptance criteria

- [ ] An ASCII lifecycle diagram of the main loop reflects the actual current order of operations in `Engine::run()` (timing, layer update, scene scope around layer rendering, GUI, audio tick, display/event polling), using current API names (e.g. the scene scope terms from `GLOSSARY.md`, not obsolete `Renderer::begin(camera)` naming).
- [ ] Engine subsystem initialization order matches the source.
- [ ] `GameLayer` hooks (`onAttach`, `onDetach`, `onUpdate`, `onRender`, `onGuiRender`, `onEvent`) and `GameLayerManager` active-layer behaviour are documented as they currently exist.
- [ ] Event documentation covers the `std::variant`-based `Event`, `visit` with `Overload`, `is<T>`, `getIf<T>` (and any other accessors/dispatchers that actually exist), event propagation order, and input polling.
- [ ] Audio documentation covers FMOD bank loading, the `Master` / `Music` / `SFX` buses, event instance playback and release lifecycle, and the per-frame update.
- [ ] Resource documentation covers the XML manifest-driven caches and allocation-free `std::string_view` lookups via transparent hashing, listing only resource types that actually exist.
- [ ] A clearly marked rendering subsection placeholder is left for ticket 04.
- [ ] No statement in the section contradicts the current source; markdown and diagram render correctly.

## Answer

Populated Section 4 with ASCII lifecycle diagram matching Engine::run(), subsystem initialization hierarchy, GameLayer virtual hooks, std::variant event dispatching, FMOD audio bus routing, XML resource holder caching, and a clear rendering placeholder for Ticket 04.
