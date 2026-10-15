# 01: Agent Protocol Header and Script-First Build Section

Status: resolved
Type: task
Blocked by: none

**What to build:** Restructure `AGENTS.md` into the spec's seven-section skeleton (Overview & Mission, Project Direction, Agent Behavior & Consultation Protocol, Subsystem Architecture & Lifecycle, Code Patterns/Naming/Style, Script-First Build/Run/Validation, Issue Tracking & Domain Docs). Open the document with the **Mandatory Agent Invariant** block, and fill in Section 6 so an agent following it builds, lints, and smoke-runs the demo exclusively through the `scripts/` toolchain.

Existing content of the current `AGENTS.md` (prerequisites, dependency/build-file rules, asset rules, change discipline, helper scripts) must be preserved by moving it into the matching skeleton section; sections owned by later tickets may hold the carried-over content or a short placeholder until those tickets land.

## Acceptance criteria

- [ ] The document starts with a binding Mandatory Agent Invariant block that applies to every session and subagent, requiring: reading `AGENTS.md` and `GLOSSARY.md` on startup; building via `scripts/build.ps1` / `scripts/build.sh`; running `scripts/lint-conventions.*` (at least `-ChangedOnly`) before finishing; and synchronizing `GLOSSARY.md`, ADRs, and affected `AGENTS.md` sections as a condition of ticket completion.
- [ ] All seven section headings exist in the agreed order.
- [ ] Section 6 presents `build`, `lint-conventions`, and `run-demo` scripts (Windows and Linux forms) as the primary commands, with raw `cmake --preset` commands kept only as a documented fallback.
- [ ] Section 6 contains the validation checklist (debug build, release build when relevant, demo smoke run with log check, lint clean), with claims about compiler warning flags verified against the actual presets/CMake configuration rather than copied from the old document.
- [ ] The "No legacy cruft" rule is present verbatim in intent (aggressively prune dead code; breaking engine API changes are fine if `TestProject` is updated in the same change).
- [ ] No guidance from the current `AGENTS.md` is lost.
- [ ] Following Section 6 word for word on Windows produces a successful `windows-debug` build and a clean `lint-conventions` run.
- [ ] Markdown structure is valid (headings, code fences, tables render correctly).

## Answer

Restructured AGENTS.md with the 7-section skeleton, established binding Mandatory Agent Invariants block at the top, and authored complete script-first Section 6 build/run/validation discipline.
