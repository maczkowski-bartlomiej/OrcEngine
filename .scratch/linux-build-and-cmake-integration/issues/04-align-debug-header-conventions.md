Status: resolved
Type: task
Blocked by: 03
# 04: Align Macro Headers with Repository Linter Conventions

**What to build:** Codebase standards conformity across all engine headers so that convention checks pass with zero errors and zero warnings under strict validation pipelines.

**Blocked by:** 03: Multi-Directory FMOD Discovery Hints

**Status:** ready-for-agent

- [ ] Core debug macro headers formatted with tab indentation
- [ ] Core debug macro declarations enclosed within canonical engine namespace
- [ ] `./scripts/lint-conventions.sh --changed-only` exits with code 0 (0 errors, 0 warnings)
- [ ] Clean smoke test execution via `./scripts/build.sh --run --timeout 5`

## Answer

Reformatted Debug.hpp with tab indentation and enclosed in namespace orc, passing conventions linter with 0 errors and 0 warnings.
