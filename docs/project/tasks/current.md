# Current working topic: Finalizing. Redesigning architecture

## [All tasks list](./not_current.md)

<!-- DOCUMENTATION DOWN -->
<!-- TODO: move to documentation -->
- Semantic phases:
    1. Modules - build ast for every module in project, and then collect all symbols
    2. Names - check if name exist
    3. types - fill TypesTable with types, check if types exists, fill types context
    4. Optimize
<!-- DOCUMENTATION UP -->

## Idea
do not divide data and ast, but have different asts.

## Current stage - redesign architecture

### improve id system (in common module)
[ ] rename common `BasicID` to `TaggedID`
[ ] add BasicID: template class that accepts size
[ ] add aliases such as ID32 ID64
[ ] add IDGenerator to common (use std::atomic)

### done preparations before semantic 
[ ] pass IDGenerator to all ScopeCoreTypes by ptr
[ ] dived TypesTable into ScopeCoreTypes and WrapperTypesStorage
[ ] update docs

### continue developing
[ ] HIRBUilder for SyntaxAST -> SemanticAST


