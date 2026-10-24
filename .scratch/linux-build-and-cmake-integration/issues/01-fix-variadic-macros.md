Status: resolved
Type: task
Blocked by: none
# 01: Standardize C++20 Variadic Macros and Conforming MSVC Preprocessor

**What to build:** Cross-compiler logging and assertion macro compatibility so that calling log and assertion macros without format arguments compiles cleanly on both Linux (GCC/Clang) and Windows (MSVC) without trailing-comma preprocessor syntax errors.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

- [ ] Logging macros expand cleanly with zero arguments using ISO C++20 `__VA_OPT__(,)`
- [ ] Assertion macros expand cleanly with zero arguments using ISO C++20 `__VA_OPT__(,)`
- [ ] MSVC target configuration enables conforming preprocessor flag `/Zc:preprocessor`
- [ ] Compilation succeeds with GCC/Clang on Linux and MSVC on Windows

## Answer

Standardized logging and assertion macros with __VA_OPT__(,) and added /Zc:preprocessor flag for MSVC.
