# Current working topic: Finalizing. Preparation before semantic.

## [All tasks list](./not_current.md)

<!-- DOCUMENTATION DOWN -->
<!-- TODO: move to documentation -->
- Semantic phases:
    1. Modules - build ast for every module in project, and then collect all symbols
    2. Names - check if name exist
    3. types - fill TypeTable with types, check if types exists, fill types context
    4. Optimize
<!-- DOCUMENTATION UP -->

## Current stage - semantic stage
1. clean up existed code
2. implement base semantic stage
- [ ] SymbolsCollector - collect all symbol declarations
- [ ] NameChecker check if identifier exists (var and functions)
- [ ] TypeBuilder - fill TypeTable with types (not supported for now, in future: aliases, structs, enum)
- [ ] TypeInitializer - create real types from UnsolvedTypes (BTW check if type exists)
- [ ] TypeResolver - resolve types (e.g. bool -> int, uint -> int, etc)

- [ ] optinal
    - [ ] Optimizers (e.g. increment, blockstmt->one-stmt(if possible), etc)

## Tasks:
- [X] remove noexcept from ast visitors
- [X] may be change semantic types a little bit if needed.
- [X] save export-import to context IN parser
- [X] remove ImportAnalyzer (basically - ImportsCollector)
- [X] clean up files organisation:
    - [X] move compile_options to common
    - [X] move compile state to common
    - [X] move Program to common
    - [X] create CompileUnit in common
- [X] update SymbolPath Structure
    (or create one more, that have absolute and relative SymbolPath, and bool flag)
    and use instead of SymbolPath (like FileLocation -> SourceLocation in past)
- [^] update semantic types architecture
    - [X] divide `semantic::Module` into 1 separated structure and SymbolID:
        - [X] CompileUnit - compile unit. not symbol 
                use it's own UnitID
                stored in Program data structure
        - [X] ModuleSym   - semantic symbol of module (like namespace)
                not needed as dedicated structure - SymbolID + context save all data
    - [X] remove deprecated shit from `Program`, save semantic context in it.
    - [X] make `semantic::Identifier` into universal SymbolID in semantic stage (instead of SymbolPath, ModuleID, pointer to node)
    - [X] integrate update SymbolPath into existing codebase
    - [X] make SymbolsContext that will be used to store all symbols (using scope, context, TypesTable, etc, and have api)
        note: SymbolsCon
    - [ ] clean up files a little bit and reorganize code in them

- [ ] write SemanticInitializers (rewrite)
- [ ] update SymbolCollector (there are some code already, and it is outdated a little bit)
