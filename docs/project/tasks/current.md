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
- [ ] remove noexcept from ast visitors
- [ ] reorganize codes in files
- [ ] may be change semantic types a little bit if needed.
- [ ] update SymbolCollector (there are some code already, and it is outdated a little bit)
