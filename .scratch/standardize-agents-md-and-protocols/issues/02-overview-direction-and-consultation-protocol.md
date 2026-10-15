# 02: Overview, Project Direction, and Consultation Protocol

Status: resolved
Type: task
Blocked by: 01

**What to build:** Fill Sections 1–3 of `AGENTS.md` so an agent understands what OrcEngine is, where it is heading, and how it must collaborate with the user. Restore the overview, roadmap, and consultation principles from the older comprehensive `AGENTS.md` (git `7a9cc44`), correcting anything that no longer matches the repository.

## Acceptance criteria

- [ ] Section 1 lists core technologies and the repository layout; stale references (e.g. the removed `dev-docs/` directory, Linux-only library artifact names) are corrected or removed, and docs locations (`GLOSSARY.md`, `docs/adr/`, `docs/agents/`, `scripts/`) are included.
- [ ] Section 2 documents the roadmap: architecture cleanup → codebase refactor (especially rendering) → C# scripting → build and release a game in a separate repository, letting the engine grow from real game requirements; reusable features belong in the engine, experiments in `TestProject`.
- [ ] Section 3 documents the consultation principles: zero unilateral assumptions, consult and propose options with trade-offs before coding, incremental execution with verification between steps, and preserving scope.
- [ ] Section 3 is consistent with the Mandatory Agent Invariant from ticket 01 (no contradictions, no duplication beyond a cross-reference).
- [ ] Markdown structure is valid, including the roadmap diagram.

## Answer

Populated Sections 1, 2, and 3 with comprehensive technology overview, repository layout, roadmap diagram, and four core consultation principles.
