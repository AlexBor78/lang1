# Not current tasks

just list of all task, just to not forget :)

### Tasks for future refactoring
- [ ] updates api (private and public)
    - [ ] rename Lexer::is_success() -> Lexer::had_errors()
    - [ ] rename `is_end()` -> `can_read()`
    - [ ] chose only one of `offset` and `n` args
- [ ] make naming more consistence
    - [ ] `ImportAnalyzer` -> `ImportsCollector`
    
- [ ] little improvement how code placed in files
- [ ] use warn, not only errors (wars - not fall or fail, error - usually fall and fail)

## Code

### Diagnostic
- [ ] add colors to logger output
- [ ] add colors to builded errors
- [ ] add syntax highlight

### AST
improve structure with MI (multiple inherit)    
- [ ] make more base node
    - [ ] KeywordStmt
    - [X] StructureStmt
    - [ ] StmtWithCond
- [ ] use them to create concrete nodes
    - [ ] IfStmt with StructureStmt, StmtWithCond, KeywordStmt
    - [ ] ElseStmt with StructureStmt, KeywordStmt 
    - [ ] etc

### Syntax
- [ ] Lexer
    - [ ] extend literals (0xFF, unicode, \xFF, etc)
- [ ] Parser
    - [ ] save import_list

### pipeline - drivers which control process of compilation
- [X] SyntaxDriver - Lexer + parser
- [X] ModulesLoader - loads all imported modules before semantic analyze (by SyntaxDriver)
- [ ] CompileDriver - process all imported modules
    - [ ] SemanticDrivers
        - [ ] SymbolsCollector
        - [ ] NamesChecker
        - [ ] some types checkers
        - [ ] some logic checkers
        - [ ] some optimizers
    - [ ] Codegen 

### semantic
<!-- DOCUMENTATION DOWN -->
<!-- TODO: move to documentation -->
- Semantic phases:
    1. Modules - build ast for every module in project, and then collect all symbols
    2. Names - check if name exist
    3. types - fill TypeTable with types, check if types exists, fill types context
    4. Optimize
<!-- DOCUMENTATION UP -->

- [ ] fix problems founded
    - [ ] remove noexcept from ast visitors

- [ ] create semantic analyzers
    - [X] ProgramInitializer (Program name, global_scope, etc)

    - [ ] Semantic analyze
        - [X] Modules phase
        <!-- 
            ImportsCollector(better name for ImportAnalyzer) will be removed,
            bcs easier to save imports_list in parser.
         -->
            - [X] ImportAnalyzers - process ast return all imported modules
            - [ ] SymbolsCollector - collect all symbol declarations
            
        - [ ] Names phase
            - [ ] NameChecker check if identifier exists (var and functions)

        - [ ] Types phase
            - [ ] TypeBuilder - fill TypeTable with types (not supported for now, in future: aliases, structs, enum)
            - [ ] TypeInitializer - create QualType from SyntaxType (BTW check if  type exists)
            - [ ] TypeResolver - resolve types (e.g. bool -> int, uint -> int, etc)

        - [ ] optimize phase
            - [ ] Optimizers (e.g. increment, blockstmt->one-stmt(if possible), etc)

## Documentation
- [^] write doc in code
- [^] wite down all other code and architecture docss

## Other

### upgrade build.sh script
- [ ] make symlink to compiler in project-root dir
- [ ] pass arguments to compiler in `run` command

### add user-api to compiler cli app (POSIX like, or even use POSIX standard)
- [ ] make POSIX-like cli compiler with support of:
    - [ ] import paths
    - [ ] different compile flags
    - [ ] etc
