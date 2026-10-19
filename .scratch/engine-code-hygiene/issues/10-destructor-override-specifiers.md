# Derived Class Destructor Override Specifiers

Status: resolved
Type: task

## Description
Ensure derived classes overriding base class virtual destructors explicitly include the `override` specifier.

### Target Files & Locations
- [OrcEngine/include/Orc/Graphics/Texturable.hpp:18](file:///home/node1/dev/projects/OrcEngine/OrcEngine/include/Orc/Graphics/Texturable.hpp#L18): Change `virtual ~Texturable() = default;` to `~Texturable() override = default;`.

### Completion Criteria
1. `~Texturable()` explicitly uses `override`.
2. Compilation succeeds without warnings.

## Answer

Added explicit override specifier to Texturable destructor in Texturable.hpp
