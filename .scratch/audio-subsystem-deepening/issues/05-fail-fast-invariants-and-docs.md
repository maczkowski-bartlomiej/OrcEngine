# Document fail-fast invariant, update GLOSSARY/AGENTS, and draft ADR

Status: resolved
Type: task
Blocked by: 04

## Description

Synchronize engine documentation with the architectural improvements and formally record the engine-wide "Fail-Fast / Crash-Early on Invalid State" philosophy.

### Scope of Work:
1. Update `GLOSSARY.md` to add `SoundHandle` and document the **Fail-Fast Invariant**.
2. Update `AGENTS.md`:
   - Section 3: Document the mandatory "Fail-Fast / Crash-Early on Invalid State" philosophy.
   - Section 4.5: Update Audio Subsystem architecture to reflect PIMPL encapsulation, `SoundHandle` instance tracking, and dynamic bus lookup.
3. Author Architectural Decision Record `docs/adr/0008-encapsulate-audio-and-deepen-instance-lifecycle.md` documenting design trade-offs, encapsulation, and instance lifetime management.

### Completion Criteria:
- `GLOSSARY.md` contains ubiquitous definitions for `SoundHandle` and the Fail-Fast invariant.
- `AGENTS.md` Section 3 and Section 4.5 are accurate and up-to-date.
- ADR 0008 is recorded in `docs/adr/`.
- `.\scripts\verify.ps1` confirms all linting and documentation integrity.

## Comments

## Answer

Recorded fail-fast invariant in GLOSSARY.md and AGENTS.md, updated Audio subsystem architecture in AGENTS.md, and recorded ADR 0008
