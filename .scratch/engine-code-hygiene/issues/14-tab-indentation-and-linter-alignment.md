# Universal Tab Indentation and Convention Linter Alignment

Status: resolved
Type: task

## Description
Convert space-based indentation to tabs across all engine headers and sources that fail `./scripts/lint-conventions.sh`, strip trailing whitespace/tabs, and update convention linting script to avoid false-positive namespace checks on application entry point files.

### Target Files & Locations
- [OrcEngine/include/Orc/Audio/Audio.hpp:55](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Audio/Audio.hpp#L55): Convert spaces to tabs.
- [OrcEngine/include/Orc/Engine/Clock.hpp:11-41](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Clock.hpp#L11-L41): Convert spaces to tabs.
- [OrcEngine/include/Orc/Engine/ResourceHolder.hpp:31](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/ResourceHolder.hpp#L31): Convert spaces to tabs.
- [OrcEngine/include/Orc/Engine/Utility.hpp:17-23](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Utility.hpp#L17-L23): Convert spaces to tabs.
- [OrcEngine/source/Orc/Audio/Bus.cpp:38, 58, 68](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Audio/Bus.cpp#L38): Convert spaces to tabs.
- [OrcEngine/source/Orc/Audio/Audio.cpp:99](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Audio/Audio.cpp#L99): Convert spaces to tabs.
- [OrcEngine/source/Orc/Engine/EntryPoint.cpp:6-12](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/EntryPoint.cpp#L6-L12): Convert spaces to tabs.
- [OrcEngine/source/Orc/Engine/Utility.cpp:19-35](file:///home/node1/dev/projects/OrcEngine/OrcEngine/source/Orc/Engine/Utility.cpp#L19-L35): Convert spaces to tabs.
- [OrcEngine/include/Orc/Engine/Config.hpp:31](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/Config.hpp#L31): Remove trailing tab after namespace brace.
- [OrcEngine/include/Orc/Engine/EntryPoint.hpp:6-8](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Engine/EntryPoint.hpp#L6-L8): Enclose declarations in `namespace orc { ... }`.
- [scripts/lint-conventions.sh:86-91](file:///home/node1/dev/projects/OrcEngine/scripts/lint-conventions.sh#L86-L91): Update linter to exempt `EntryPoint.cpp` and `StbImage.cpp` from the strict `namespace orc` requirement.

### Completion Criteria
1. `./scripts/lint-conventions.sh` runs across all files and exits with 0 errors and 0 warnings.

## Answer

Converted space-based indentation to tabs across Audio.hpp, Clock.hpp, ResourceHolder.hpp, Utility.hpp, Bus.cpp, Audio.cpp, EntryPoint.cpp, Utility.cpp; removed trailing tab in Config.hpp; enclosed callbacks in namespace orc in EntryPoint.hpp; and updated convention linter to exempt non-symbol units
