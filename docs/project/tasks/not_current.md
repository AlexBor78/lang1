# Not current tasks

just list of all task, just to not forget :)

## Code

### Logger
- [X] add .log to gitignore
- [X] implement logger
- [ ] add colors to logger output

### Parser
- [ ] refactor
    - [X] first iteration
        - [X] adapt to new api
        - [X] clean up code a little bit
    - [ ] second iteration
        - [ ] use warns
        - [ ] clean up code
        - [ ] add pos to errors and nodes
        - [ ] maybe(would be perfect) fix issue with last's tokens (peek(): out of range, if token missed)
    - [X] extend AST(BreakStmt, ContinueStmt)
    - [X] add END token 
    - [ ] save node pos
    - [X] add if-else for while
    - [X] allow stmt without {} for if-else, for, while, stmt

### Lexer
- [X] rename class Tokenizer to Lexer 
- [X] extend lexer for 3 char op's and comments
- [ ] extend literals (0xFF, unicode, \xFF, etc)
- [X] use lexer api instead of stream api (use steam api IN imp of lexer api)
- [ ] use warn

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


## semantic
- [X] create semantic types
    - [X] identifier
    - [X] scope
    - [X] module
    - [X] program

<!-- DOCUMENTATION DOWN -->
- Semantic phases:
    1. Modules - build ast for every module in project, and then collect all symbols
    2. Names - check if name exist
    3. types - fill TypeTable with types, check if types exists, fill types context
    4. Optimize
<!-- DOCUMENTATION UP -->

- [ ] fix problems founded
    - [ ] remove noexcept from ast visitors
    - [ ] SAVE NODE SourceLocation IN PARSER!!!!!!
    - [ ] remove module decl node

- [ ] create semantic analyzers
    - [X] ProgramInitializer (Program name, global_scope, etc)

    - [ ] Semantic analyze
        - [X] Modules phase
            - [X] ImportAnalyzers process ast return all imported modules (names)
            <!-- maybe rename ImportAnalyzer -> ImportsCollector -->
            - [X] SymbolsCollector - collect all symbol (not body)
            
        - [ ] Names phase
            - [ ] NameChecker check if identifier exists (var and functions)

        - [ ] Types phase
            - [ ] TypeBuilder - fill TypeTable with types (not supported for now, in future: aliases, structs, enum)
            - [ ] TypeInitializer - create QualType from SyntaxType (BTW check if  type exists)
            - [ ] TypeResolver - resolve types (e.g. bool -> int, uint -> int, etc)

        - [ ] optimize phase
            - [ ] Optimizers (e.g. increment, blockstmt->one-stmt(if possible), etc)

### make naming(classes, args, api, etc) more consistence
<!-- - [ ] use AST alias type instead of `std::vector<ast::BaseNode>`  not only alias, structure with context--> 
<!-- - [ ] maybe rename ASTVisitor -> Visitor (and with const variation) no -->
- [ ] standardization api (private and public)
    - [ ] rename Lexer::is_success() -> Lexer::had_errors() <!-- maybe deprecated bcs of different code structure -->


### refactor and extend Error
- [X] add warns
- [ ] add colors
- [X] upgrade output
- [X] handle multi-line tokens
- [X] add SyntaxError's <- CompileError
    - [X] add StreamError   <- SyntaxError
    - [X] add LexerError    <- SyntaxError
    - [X] add ParserError   <- SyntaxError
- [ ] add SemanticError's <- CompileError
    - [ ] add TypeError <- SemanticError
    - [ ] add LogicError <- SemanticError
    - [ ] add etc <- SemanticError

## Change done stuff -> new and actual ideas
- [ ] new type's syntax AND ne type's Types design (to have infinity wrappers)
- [ ] remove namespaces at all
- [ ] add keywords, nodes, etc for new features
- [ ] implement new modules (at list everything without increment build)


## Documentation

now it is in current tasks

## Other

### upgrade build.sh script
- [X] add build arg (debug)
- [X] use multiple cores
- [X] pass DEBUGING through flags
- [ ] make symlink to compiler in project-root dir


### add user-api to compiler cli app (POSIX like, or even use POSIX standard)
- [X] CLIParser
- [X] CompileDriver - control process of compilation
- [ ] update build.sh to make symlink to compiler in project-root dir (and update clean.sh to remove link)
- [ ] remove run.sh
