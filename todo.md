- [ ] Lexer
    - [X] rename class Tokenizer to Lexer 
    - [X] extend lexer for 3 char op's and comments
    - [ ] extend literals (0xFF, unicode, \xFF, etc)
    - [X] use lexer api instead of stream api (use steam api IN imp of lexer api)

- [X] done streams
    - [X] move stream.h to stream.h istream.h ostream.h
    - [X] done InputStream classes
        - [X] implement InputStream
        - [X] add StringIStream
        - [X] add FileIStream
        - [x] add ConsoleIStream
    - [X] done OutputStream classes
        - [X] implement OutputStream
        - [X] create ConsoleOSteam
        - [X] create ConsoleErrOSteam
        - [X] create FileOStream
        - [X] create StringOStream

- [ ] done Logger
    - [X] add .log to gitignore
    - [X] implement logger
    - [ ] add colors to logger output

- [ ] project code refactor
    - [ ] rename .../lang/frontend -> .../lang/syntax (and utils btw)
    - [ ] make src/common dir and move logger and streams, maybe some utils to this
    <!-- NO - [ ] maybe change file structure (mv .../lang/(semantic, syntax) -> .../lang/frontend/..., etc) -->
    - [X] rename src/compiler folder to src/compiler_app
    - [X] use shared_ptr instead unique_ptr in logger for streams
    - [X] refactor PrintAST
        - [X] rename some nodes (e.g. CallExpr -> FunctionExpr)
        - [ ] check if // WTF?

    - [ ] make naming(classes, args, api, etc) more consistence
        - [ ] use AST alias type instead of `std::vector<ast::BaseNode>` 
        - [ ] maybe rename ASTVisitor -> Visitor (and with const variation)
        - [ ] standardization api (private and public)
            - [ ] rename Lexer::is_success() -> Lexer::had_errors()

    - [ ] refactor Parser
        - [X] first iteration
            - [X] adapt to new api
            - [X] clean up code a little bit
        - [ ] second iteration
            - [ ] clean up code
            - [ ] add pos to errors and nodes
            - [ ] maybe(would be perfect) fix issue with last's tokens (peek(): out of range)

    - [ ] use warns
        - [ ] Lexer
        - [ ] Parser

    - [X] use Logger
        - [X] CompileError: while building error message
        - [X] Lexer
        - [X] Parser

    - [X] refactor Position
        - [X] rename Position to SourceLocation 
        - [X] add end pos (and remove length)
        - [X] set pos.line and pos.column in stream

    - [ ] refactor and extend Error
        - [X] add warns
        - [ ] add colors
        - [X] upgrade output
        - [X] handle multi-line tokens
        - [X] add FrontendError's <- CompileError
            - [X] add StreamError   <- FrontendError
            - [X] add LexerError    <- FrontendError
            - [X] add ParserError   <- FrontendError
        - [ ] add MiddlendError's <- CompileError
            - [ ] add SemanticError's <- MiddlendError

    - [X] update api
        - [X] add and use match() function
            - Stream - doesn't need
            - Lexer - doesn't need
            - [X] Parser
        - [X] change peak -> peek
            - [X] in Stream
            - [X] in Lexer
            - [X] in Parser
        - [X] change next -> advance
            - [X] in Stream
            - [X] in Lexer
            - [X] in Parser
        - [X] add skip
            - [X] in Stream
            - [X] in Lexer
            - [X] in Parser

    - [X] upgrade build.sh script
        - [X] add build arg (debug)
        - [X] use multiple cores
        - [X] pass DEBUGING through flags

- [ ] extend parser
    - [X] extend AST(BreakStmt, ContinueStmt)
    - [X] add END token 
    - [ ] save node pos
    - [X] add if-else for while
    - [X] allow stmt without {} for if-else, for, while, stmt

- [ ] add user-api to compiler app
    - [X] CLIParser
    - [X] CompileDriver - control process of compilation
    - [ ] update build.sh to make symlink to compiler in project-root dir (and update clean.sh to remove link)
    - [ ] remove run.sh

- [X] preparation for semantic
    - [X] add state type
    - [X] remove types from AST and add type context
    - [X] add SyntaxType and SyntaxTypeContext (use in parser)


- [ ] semantic...
    - [X] create semantic types
        - [X] identifier
        - [X] scope
        - [X] module
        - [X] program

    - Semantic phases:
        1. Modules - process imports, collect all names from modules, (without body)
        2. Names - check if name exist
        3. types - fill TypeTable with types, check if types exists, fill types context
        4. Optimize

    - [ ] create semantic analysts
        - [ ] ProgramInitializer (Program name, global_scope, etc)
        - [ ] Modules phase
            - [ ] ModuleProcessor - check if file starts from ModuleDecl node
            - [ ] ImportProcessor - process modules import

        - [ ] Names phase
            - [ ] NameChecker check if identifier exists (var and functions)

        - [ ] Types phase
            - [ ] TypeBuilder - fill TypeTable with types (not supported for now, in future: aliases, structs, enum)
            - [ ] TypeInitializer - create QualType from SyntaxType (BTW check if  type exists)
            - [ ] TypeResolver - resolve types (e.g. bool -> int, uint -> int, etc)

        - [ ] optimize phase
            - [ ] Optimizers (e.g. increment, blockstmt->one-stmt(if possible), etc)

- [ ] todo some day(after v0 version), wrote just to not forget
    - [ ] use precompiled headers
    - [ ] design module (for increment compilation)
    - [ ] multi-threading
    
- [ ] design module (files and logic)
    - [ ] check if file was edited (by hash)
        - [ ] crypto library
        - [ ] maybe some data structure's

    - [ ] file format (some JSON stuff)
        - [ ] needs JSON lib
        - [ ] serialization (and serializer)
            - [ ] AST serializer (ConstVisitor BTW)
            - [ ] Module(data structure) serializer (TypeTable, Scope, Identifier, etc)
            - [ ] main Serializer class
        - [ ] deserialization (and deserializer)
            - [ ] AST deserializer (ConstVisitor BTW)
            - [ ] Module(data structure) deserializer (TypeTable, Scope, Identifier, etc)
            - [ ] main DeSerializer class
