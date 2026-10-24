Status: resolved
Type: task
Blocked by: 01
# 02: Executable Script Permissions and vcpkg Fallback Auto-Detection

**What to build:** Friction-free script execution on Linux distributions where automation scripts can be invoked directly (`./scripts/build.sh`, `./scripts/check-env.sh`) without `Permission denied` errors, automatically detecting `$HOME/vcpkg` if unset.

**Blocked by:** 01: Standardize C++20 Variadic Macros and Conforming MSVC Preprocessor

**Status:** ready-for-agent

- [ ] Git index tracks executable mode (`100755`) for all POSIX scripts in `scripts/*.sh`
- [ ] `./scripts/check-env.sh` auto-detects `$HOME/vcpkg` if `$VCPKG_ROOT` is unset
- [ ] `./scripts/build.sh` auto-detects `$HOME/vcpkg` if `$VCPKG_ROOT` is unset
- [ ] Direct script execution succeeds without `Permission denied` on Linux

## Answer

Configured executable filemode 100755 in git index for scripts/*.sh and added /home/node1/vcpkg fallback auto-detection to check-env.sh and build.sh.
