---
name: park
description: Park a raw idea, quick fix, or code proposal for later grilling without analyzing or implementing it. Use when capturing an unvetted thought or asked to park an idea for later.
argument-hint: "The raw idea, fix, or proposal to park"
---

# Park

Capture an unvetted idea, quick fix, or code proposal into `.scratch/` for a future `/grill-me` session. Preserve the proposal verbatim and halt immediately.

## Guardrail

**Silence over commentary.** Withhold evaluation, feedback, pros and cons, refactoring, and code implementation. The idea is unvetted by design; interrogation belongs strictly to a future grilling round, never this turn.

## Process

### 1. Ingest the payload

Extract the raw text, fix, or code proposal from the argument or triggering message. Preserve the author's wording and code blocks verbatim without editing, polishing, or normalising.

### 2. Determine target path

Save to `.scratch/` following the repository's ticket format:

- **Active effort in flight**: If the conversation is already working within an effort in `.scratch/<effort>/`, find the next available number in `.scratch/<effort>/issues/` and save as:
  `.scratch/<effort>/issues/<NN>-<slug>.md`
- **Standalone idea**: If no effort is active, save under `.scratch/ideas/issues/<NN>-<slug>.md` (create `.scratch/ideas/issues/` if needed, numbering from `01`).

### 3. Write the ticket

Format the ticket to match the repository's ticket parser (`scripts/ticket.ps1`):

```markdown
# <Descriptive Title>

Status: needs-triage
Type: grilling

## Raw Proposal

<verbatim payload, including code blocks unmodified>

## Context

- **Source:** <current file, branch, or conversation context, or "Standalone">
- **Captured:** <current date YYYY-MM-DD>

## Frontier Seeds

<!-- 1 to 3 bare decision questions latent in the proposal for the eventual grilling session; do not answer them -->
- ❓ <Decision 1>
- ❓ <Decision 2>
```

### 4. Emit receipt and halt

Conclude the turn with a single-line receipt linking to the created ticket:

> Parked in [`<relative-path>`](file:///<absolute-path>). Run `/grill-me` when ready to stress-test it.

Do not append suggestions, commentary, or follow-up questions. Stop.
