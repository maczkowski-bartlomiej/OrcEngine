# Agent & Developer Scripts

This directory contains automation scripts designed for AI agents and developers working on **OrcEngine**. Both PowerShell (`.ps1` for Windows) and POSIX Bash (`.sh` for Linux/macOS) versions are provided.

---

## Quick Reference

| Tool | Windows (PowerShell) | Linux / macOS (Bash) | Purpose |
| :--- | :--- | :--- | :--- |
| **Environment Doctor** | `.\scripts\check-env.ps1` | `./scripts/check-env.sh` | Verify CMake, Ninja, C++20 compiler, `VCPKG_ROOT`, `FMOD_ROOT`, Git |
| **Issue Tracker / Wayfinder** | `.\scripts\ticket.ps1 <cmd>` | `./scripts/ticket.sh <cmd>` | Manage `.scratch/` feature specs, tickets, wayfinding frontier, and triage |
| **Conventions Linter** | `.\scripts\lint-conventions.ps1` | `./scripts/lint-conventions.sh` | Enforce `#pragma once`, tab indentation, and `namespace orc` rules from `AGENTS.md` |
| **Build Runner** | `.\scripts\build.ps1` | `./scripts/build.sh` | Auto-detect toolchain (MSVC/Ninja), configure, compile presets |
| **Demo Runner** | `.\scripts\run-demo.ps1` | `./scripts/run-demo.sh` | Run `TestProject` from build dir with timeout and log error checks |

---

## 1. Environment Doctor (`check-env.ps1` / `check-env.sh`)

Checks system prerequisites required by OrcEngine as documented in `AGENTS.md`:
- CMake 3.25+
- Ninja build system
- C++20 compiler (MSVC, GCC 11+, or Clang 13+)
- `$env:VCPKG_ROOT` and valid vcpkg toolchain
- `$env:FMOD_ROOT` and FMOD Core / Studio header layout
- Git branch and uncommitted files

```powershell
# Windows
.\scripts\check-env.ps1
```

```bash
# Linux
./scripts/check-env.sh
```

Exit code: `0` if all checks pass or warnings only; `1` on critical missing prerequisite.

---

## 2. Issue Tracker & Wayfinder CLI (`ticket.ps1` / `ticket.sh`)

Implements the local markdown ticket system described in [`docs/agents/issue-tracker.md`](../docs/agents/issue-tracker.md) and canonical triage labels from [`docs/agents/triage-labels.md`](../docs/agents/triage-labels.md).

### Commands

- **List tickets**:
  ```powershell
  .\scripts\ticket.ps1 list
  .\scripts\ticket.ps1 list <feature-slug>
  ```
- **Find next unblocked ticket (Wayfinding Frontier)**:
  Scans `.scratch/<effort>/issues/` for tickets that are open, unblocked, and unclaimed:
  ```powershell
  .\scripts\ticket.ps1 frontier
  .\scripts\ticket.ps1 frontier <feature-slug>
  ```
- **Claim a ticket**:
  Sets `Status: claimed` before starting work:
  ```powershell
  .\scripts\ticket.ps1 claim .scratch/feature/issues/01-task.md
  ```
- **Resolve a ticket**:
  Sets `Status: resolved`, appends answer to `## Answer`, and updates `map.md` Decisions-so-far:
  ```powershell
  .\scripts\ticket.ps1 resolve .scratch/feature/issues/01-task.md -Answer "Implemented feature and verified build."
  ```
- **Scaffold a new feature**:
  Creates `.scratch/<feature-slug>/spec.md` and `.scratch/<feature-slug>/map.md`:
  ```powershell
  .\scripts\ticket.ps1 new-feature audio-system -Title "FMOD Audio Integration"
  ```
- **Scaffold a new issue ticket**:
  Creates `.scratch/<feature-slug>/issues/<NN>-<slug>.md` with correct next sequential number:
  ```powershell
  .\scripts\ticket.ps1 new-issue audio-system bus-management -Title "Implement audio bus routing" -Type task -BlockedBy "01"
  ```
- **Triage overview**:
  Displays tickets grouped by canonical roles (`needs-triage`, `ready-for-agent`, `ready-for-human`, `needs-info`, `wontfix`, etc.):
  ```powershell
  .\scripts\ticket.ps1 triage
  ```

---

## 3. C++ Conventions Linter (`lint-conventions.ps1` / `lint-conventions.sh`)

Automates rule verification from `AGENTS.md`:
- Every header (`.hpp`, `.inl`) must start with `#pragma once`.
- Engine code in `OrcEngine/` must be wrapped in `namespace orc`.
- Tab-based indentation must be preserved (flags space indentation).
- File extensions must follow conventions (`.hpp`, `.cpp`, `.inl`).

### Flags

- `-ChangedOnly`: Only lints files modified or staged in git (recommended for agents prior to committing or finishing turn).
- `-Fix`: Automatically inserts missing `#pragma once` headers.
- `-Paths <dir/file>`: Targets specific directories or files.

```powershell
# Pre-commit / pre-completion check for agent
.\scripts\lint-conventions.ps1 -ChangedOnly

# Full codebase check
.\scripts\lint-conventions.ps1

# Auto-fix missing pragmas
.\scripts\lint-conventions.ps1 -Fix
```

---

## 4. Build Runner (`build.ps1` / `build.sh`)

Handles environment setup, CMake preset configuration, and compilation. On Windows, it automatically discovers and activates Visual Studio Developer Shell (`Launch-VsDevShell.ps1`), `ninja.exe`, and `cmake.exe` if not present in the default shell `PATH`.

```powershell
# Build debug preset (default: windows-debug on Windows)
.\scripts\build.ps1

# Build release preset
.\scripts\build.ps1 -Preset windows-release

# Clean build
.\scripts\build.ps1 -Clean

# Build specific target only
.\scripts\build.ps1 -Target OrcEngine

# Build and immediately launch demo
.\scripts\build.ps1 -Run -TimeoutSeconds 5
```

---

## 5. Demo Runner (`run-demo.ps1` / `run-demo.sh`)

As mandated by `AGENTS.md`, `TestProject` must be executed with its working directory set to `build/<preset>` so relative asset paths (`assets/` and `shaders/`) resolve properly.

```powershell
# Run demo interactively
.\scripts\run-demo.ps1 -Preset windows-debug

# Smoke test for automated agents (runs for 5 seconds and inspects logs)
.\scripts\run-demo.ps1 -TimeoutSeconds 5 -FailOnErrorLog
```
- Automatically tails recent entries from `logs/TestProject.log`.
- Flags crashes, assert failures, and `[error]` / `[critical]` log lines.
