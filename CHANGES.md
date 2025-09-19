# Changes

## [1.0.0] - 2025-01-18

### Added
- **Project Setup**: Basic compiler parts and file layout
- **Code Tree**: Tree structure to hold code parts
- **Types**: Basic types like int, float, string, bool
- **Errors**: System to show errors with line numbers
- **Lexer**: Reads code and makes tokens
- **Parser**: Turns tokens into code tree (basic setup)
- **Checker**: Checks if code makes sense (basic setup)
- **Generator**: Makes output code (basic setup)
- **CLI**: Command line tool with help and options
- **Utils**: Helper functions for files and debug

### Files Added
- `src/ast.hpp` & `src/ast.cpp` - Code tree
- `src/types.hpp` - Type system
- `src/error.hpp` & `src/error.cpp` - Error system
- `src/semantic.hpp` & `src/semantic.cpp` - Code checker
- `src/codegen.hpp` & `src/codegen.cpp` - Code maker
- `src/parser.hpp` & `src/parser.cpp` - Parser
- `src/utils.hpp` & `src/utils.cpp` - Helper tools

### Files Changed
- `src/lexar.hpp` - Better token system
- `src/main.cpp` - New CLI and build steps