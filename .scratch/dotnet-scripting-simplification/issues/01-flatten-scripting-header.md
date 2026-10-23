# 01: Flatten Scripting Header and Remove PIMPL

Status: ready-for-agent
Type: task
Blocked by: none

**What to build:** Remove `struct Impl` and `m_impl` from `Scripting.hpp`. Directly store `FilePath m_configPath` and `void* m_hostfxrLib{nullptr}` as private member variables. Declare private helper methods `void loadHostFxr()` and `void initializeRuntime()`. Delete both copy and move operations (`= delete`). Ensure zero native OS or .NET SDK headers are exposed.

**Blocked by:** None (can start immediately)

- [ ] `Scripting.hpp` removes `struct Impl` forward declaration and `UniquePtr<Impl> m_impl` (and removes unused `Core/Memory.hpp` include)
- [ ] `Scripting.hpp` directly declares `FilePath m_configPath` and `void* m_hostfxrLib{nullptr}`
- [ ] `Scripting.hpp` declares `void loadHostFxr()` and `void initializeRuntime()` as private methods
- [ ] Constructor signature remains `explicit Scripting(const FilePath& configPath = "OrcEngine.Scripting.runtimeconfig.json")`
- [ ] Both copy and move operations (`Scripting(const Scripting&)`, `operator=(const Scripting&)`, `Scripting(Scripting&&)`, `operator=(Scripting&&)`) are explicitly `= delete`
- [ ] Public header contains zero platform headers (`<Windows.h>`, `<dlfcn.h>`) and zero .NET hosting headers (`<nethost.h>`, `<hostfxr.h>`, `<coreclr_delegates.h>`)
- [ ] Conforms to conventions linter
