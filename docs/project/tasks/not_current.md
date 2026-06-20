# Not current tasks

just list of all task, just to not forget :)

### Tasks for future refactoring
- [ ] updates api (private and public)
    - [ ] rename `Lexer::is_success()` -> `Lexer::had_errors()`
    - [ ] rename `is_end()` -> `can_read()`
    - [ ] chose only one of `offset` and `n` args
- [ ] make naming more consistence
    - [ ] `ImportAnalyzer` -> `ImportsCollector`
    - [ ] `FileLocation` -> `FilePosition`
    - [ ] everywhere position to location where it is location (mainly in ast)
    - [ ] `DiagnosticBuilder` -> `DiagnosticGenerator`
    - [X] `TypeTable` -> `TypesTable`
- [ ] rewrite shitcode
    - [ ] rewrite `ModulesLoader` implementation
    
- [ ] little improvement how code placed in files
- [ ] use warn, not only errors (wars - not fall or fail, error - usually fall and fail)

## Code

### other
- [ ] use FileLocation in streams instead of SourceLocation
- [ ] save line_start index(in symbols) in FileLocation to easier code in diagnostic builder
- [ ] update and improve PrintAST.

### Diagnostic
- [ ] don't use exceptions, instead use rust Result like structure as return type
- [ ] lazy generating error (not needed yet, will need in future)
- [ ] add colors to logger output
- [ ] add colors to builded errors
- [ ] add syntax highlight

### Syntax
- [ ] Parser
    - [X] save import_list
    - [ ] fix not passed very complex operators sequences in test.lang
    - [ ] update error generating 
        - [ ] pass location to generating func
        - [ ] on error like expected X - location should be not next token, but after last token

### pipeline - drivers which control process of compilation
- [X] CompileDriver - process all imported modules
- [X] ModulesLoader - loads all imported modules before semantic analyze (by SyntaxDriver)
- [X] SyntaxDriver - Lexer + parser
- [ ] SemanticDrivers
    - [ ] HIRBuilder (collects `export`ed symbols; builder HIR out of AST)
    - [ ] NamesChecker
    - [ ] some types checkers
    - [ ] some logic checkers
    - [ ] some optimizers
- [ ] Codegen 

### semantic
1. clean up existed code
2. implement base semantic stage
- [ ] NameChecker check if identifier exists (var and functions)
- [ ] TypeBuilder - fill TypeTable with types (not supported for now, in future: aliases, structs, enum)
- [ ] TypeInitializer - create real types from UnsolvedTypes (BTW check if type exists)
- [ ] TypeResolver - resolve types (e.g. bool -> int, uint -> int, etc)

- [ ] optinal
    - [ ] Optimizers (e.g. increment, blockstmt->one-stmt(if possible), etc)
    - [ ] clean up files a little bit and reorganize code in them
- [ ] write SemanticInitializers (rewrite)
- [ ] update SymbolCollector (there are some code already, and it is outdated a little bit)

semantic is now current stage

## Documentation
- [^] write doc in code
- [^] wite down all other code and architecture docss

## Other

### upgrade build.sh script
- [X] make symlink to compiler in project-root dir
- [X] check exit code after build
- [ ] add --ninja build flag
- [X] rename flag `--generate-docs` to command `gen-docs`
- [ ] add docker build option

### add user-api to compiler cli app (POSIX like, or even use POSIX standard)
- [X] make POSIX-like cli compiler with support of:
    - [X] import paths
    - [X] different compile flags
    - [X] etc
