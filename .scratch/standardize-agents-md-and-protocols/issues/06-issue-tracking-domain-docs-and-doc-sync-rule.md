# 06: Issue Tracking, Domain Docs, and Documentation Sync Rule

Status: resolved
Type: task
Blocked by: 01

**What to build:** Fill Section 7 of `AGENTS.md` so an agent can find, claim, work, and close tickets according to repo conventions, and knows exactly which documentation must be updated before a ticket counts as complete.

## Acceptance criteria

- [ ] Documents the `.scratch/<feature-slug>/` layout (spec, map, one-file-per-ticket issues), the `Status:` / `Type:` / `Blocked by:` line format that `ticket.ps1` / `ticket.sh` parse, and the frontier → claim → resolve workflow with the script commands.
- [ ] Documents the canonical triage labels, linking to `docs/agents/triage-labels.md` and `docs/agents/issue-tracker.md` rather than duplicating them wholesale.
- [ ] Documents the domain docs: `GLOSSARY.md` (use its vocabulary, respect `_Avoid_` terms) and `docs/adr/` (respect ADRs in the touched area; record a new ADR when a real trade-off is made).
- [ ] States the formal completion rule: code changes affecting terminology, architecture, or subsystems must update `GLOSSARY.md`, add an ADR where trade-offs occur, and update the relevant `AGENTS.md` sections — consistent with the Mandatory Agent Invariant from ticket 01.
- [ ] Commands shown actually work when run against the current `.scratch/` tree (e.g. `ticket.ps1 frontier standardize-agents-md-and-protocols`).

## Answer

Populated Section 7 with .scratch/ directory conventions, ticket header format, wayfinding CLI workflows, triage roles, domain documentation rules, and mandatory documentation sync requirements.
