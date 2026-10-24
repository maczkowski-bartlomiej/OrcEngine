# Revert to Polymorphic Event System Map

## Notes
Reversion of the `std::variant`-based event system back to the classic, lightweight polymorphic `Event` base class hierarchy with `getEvent<T>()` helper, removing template visitation bloat, cyclic header dependencies, and dead handled state.

## Decisions so far
- [03-sync-documentation-and-adr](issues/03-sync-documentation-and-adr.md): Synchronized GLOSSARY.md, AGENTS.md, recorded ADR 0005, and passed all verification gates with 100% success.
- [02-update-subsystem-dispatch-and-test-project](issues/02-update-subsystem-dispatch-and-test-project.md): Updated Engine, GameLayer, Input, and all TestProject layers to dispatch and handle events using event.getType() and getEvent<T>()
- [01-revert-event-base-and-concrete-structs](issues/01-revert-event-base-and-concrete-structs.md): Reverted Event base class to polymorphic hierarchy with getEvent helper and updated concrete event structs.
- Base `Event` is a lightweight polymorphic base class with `enum class Type`, `explicit Event(Type type)`, and `virtual ~Event() = default;`.
- Downcasting uses free function `getEvent<EventType>(const Event&)` with debug RTTI assertion (`dynamic_cast`) and `static_cast` in release.
- Unused `m_handled` state (`setHandled`, `isHandled`) eliminated.
- Concrete event structs inherit publicly from `Event` and declare plain (non-const) fields.
- Subsystem integration retains decoupled `Input::onEvent` and `GameLayer::onEvent` camera resizing.

## Tickets
- [01-revert-event-base-and-concrete-structs](issues/01-revert-event-base-and-concrete-structs.md)
- [02-update-subsystem-dispatch-and-test-project](issues/02-update-subsystem-dispatch-and-test-project.md)
- [03-sync-documentation-and-adr](issues/03-sync-documentation-and-adr.md)

## Fog
None. Architecture and design decisions fully explored and settled.
