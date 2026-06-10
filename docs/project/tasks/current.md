# Current working topic: Finalizing. Redesigning architecture

## [All tasks list](./not_current.md)

<!-- DOCUMENTATION DOWN -->
<!-- TODO: move to documentation -->
- Semantic phases:
    1. Modules - build ast for every module in project, and then collect all symbols
    2. Names - check if name exist
    3. types - fill TypeTable with types, check if types exists, fill types context
    4. Optimize
<!-- DOCUMENTATION UP -->

## Idea
do not devide data and ast, but have diffrent asts.

## Current stage - redesign architecture

1. [ ] redesign memory usage
    [X] introduce ArenaAloc
    [ ] Lexer part
      [ ] introduce SourceFile (structure that store WHOLE file)
      [ ] store all files in one arena (till end of compilation)
      [ ] do not store strings in tokens, only string views
      [ ] adapt lexer to use SourceFile
    [ ] Parser part
      [ ] change all smart ptrs in ast to stuped ones
      [ ] allocate all ast in separated arena


2. [ ] redesign AST 
    [ ] add whole bunch off shit that been moved to contexts
    [ ] move to ArenaAloc for AST
    [ ] adapt parser to use AST
3. [ ] HIR (Basicaly second AST with different data)
    [ ] redesign semantic types (scope, symbol)
    [ ] design HIR
4. [ ] "translaor" for SyntaxAST -> SemanticAST

