# 05: Code Patterns, Naming, and Style

Status: resolved
Type: task
Blocked by: 01

**What to build:** Fill Section 5 of `AGENTS.md` so new code written by an agent matches existing style and ownership/performance contracts without guesswork. Restore the naming table and conventions from the older comprehensive `AGENTS.md` (git `7a9cc44`), merged with the current document's C++ conventions, and verified against the actual codebase and the conventions linter.

## Acceptance criteria

- [ ] A naming table covers namespaces, types, functions, member variables, constants/macros, enums, and file names; each row is confirmed against real code (e.g. confirm whether data members actually use `m_camelCase`), and any discrepancy with the current document's wording is resolved explicitly.
- [ ] Parameter passing conventions are documented: `std::string_view` for read-only strings (with the C-string exception), `const FilePath&` for paths, `std::span` for contiguous buffers, compound math types by `const&`, primitives by value, pass-by-value + `std::move` sinks.
- [ ] Ownership/RAII rules are documented: Rule of 5 for resource owners, `Ref<T>` / `createRef`, `UniquePtr<T>` / `createUniquePtr` (only aliases that actually exist), and never calling `release()` in destructors.
- [ ] Logging and assertion macros list only macros that exist, with their intended severity use; recoverable failures are handled without crashing.
- [ ] Formatting rules (tabs, Allman braces, `#pragma once`, umbrella header exposure, `.hpp`/`.cpp`/`.inl`) agree with what `lint-conventions` enforces.
- [ ] "Follow the surrounding file" and "avoid broad cleanup in focused fixes" guidance from the current document is retained.

## Answer

Populated Section 5 with verified naming conventions table, parameter/argument passing rules, RAII and smart pointer aliases, logging/assertion macros, and formatting discipline.
