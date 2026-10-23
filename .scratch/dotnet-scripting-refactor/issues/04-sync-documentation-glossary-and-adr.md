# 04: Sync Documentation, Glossary, and Record ADR 0008

Status: resolved
Type: task
Blocked by: 03

**What to build:** Synchronize domain documentation and architectural records with the newly refactored .NET 10 CoreCLR Scripting subsystem.

**Blocked by:** 03 (needs complete implementation of scripting subsystem and engine cleanup)

- [x] `GLOSSARY.md` includes canonical definition for **Scripting** and lists deprecated terms to avoid (`Mono`, `ScriptEngine`, `DotNetManager`, `CSharpRuntime`)
- [x] `AGENTS.md` updates subsystem list and adds Section 4.8 detailing the Scripting Subsystem lifecycle, hosting sequence, and `[UnmanagedCallersOnly]` entry points
- [x] ADR `0008-hostfxr-dotnet-coreclr-scripting.md` is recorded in `docs/adr/` capturing context, decision, and consequences
- [x] Run unified verification gate `.\scripts\verify.ps1` with 100% pass across linting, compilation, and the 5-second smoke test

## Answer

Synchronized GLOSSARY.md with canonical Scripting definition and deprecated terms, added Section 4.8 to AGENTS.md documenting Scripting subsystem lifecycle and hosting model, and recorded ADR 0008.
