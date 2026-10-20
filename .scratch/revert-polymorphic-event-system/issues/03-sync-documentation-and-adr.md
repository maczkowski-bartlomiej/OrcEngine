# Synchronize Documentation and Record ADR 0005

Status: resolved
Type: task
Blocked by: 02

## Description

Synchronize all project documentation and record the architectural decision in compliance with Mandatory Agent Invariant 4:

1. **ADR 0005**:
   - Create `docs/adr/0005-revert-to-polymorphic-event-hierarchy.md` documenting context, decision to revert from `std::variant` to a polymorphic base class with `Event::Type` and `getEvent<T>()`, and trade-offs.
2. **`AGENTS.md`**:
   - Update Section 4.3 ("Window, Input, and Events Flow") to document the polymorphic `Event` base class, `getType()`, `getEvent<T>()`, explicit constructor requirements, and removal of `std::variant`, `visit`, and `m_handled`.
3. **`GLOSSARY.md`**:
   - Add definition for **Event** and prohibited terms (`EventVariant`, `EventDispatcher`, `InputMessage`).
4. **Unified Verification Gate**:
   - Run `./scripts/verify.sh` to ensure all stages pass with zero errors.

## Completion Criteria
- Documentation is 100% in sync with the codebase.
- ADR 0005 is created and linked.
- `./scripts/verify.sh` passes 100%.

## Answer

Synchronized GLOSSARY.md, AGENTS.md, recorded ADR 0005, and passed all verification gates with 100% success.
