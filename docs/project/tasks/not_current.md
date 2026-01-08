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
    
- [ ] little improvement how code placed in files
- [ ] use warn, not only errors (wars - not fall or fail, error - usually fall and fail)

## Code

### other
- [ ] use FileLocation in streams instead of SourceLocation
- [ ] save line_start index(in symbols) in FileLocation to easier code in diagnostic builder
- [ ] update and improve PrintAST.

### Diagnostic
- [ ] lazy generating error (not needed yet, will need in future)
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
    - [ ] fix not passed very complex operators sequences in test.lang
    - [ ] update error generating 
        - [ ] pass location to generating func
        - [ ] on error like expected X - location should be not next token, but after last token

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

semantic is now current stage

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
