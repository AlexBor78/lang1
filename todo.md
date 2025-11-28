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
    - [x] implement logger
    - [ ] add colors to logger output

- [ ] project code refactor
    - [X] rename src/compiler folder to src/compiler_app
    - [X] use shared_ptr instead unique_ptr in logger for streams
    - [ ] standardization api (private and public)
        - [ ] rename Lexer::is_success() -> Lexer::had_errors()

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
        - [ ] add warns
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
    - [ ] add END token
    - [ ] save node pos
    - [ ] add if-else for while
    - [ ] allow stmt without {} for if-else, for, while, stmt

- [ ] add user-api to compiler app
    - [ ] multi-file handling (import module, than use module.lang file)
    - [ ] pass insert files in args
    - [ ] pass paths to find modules

- [ ] semantic...
    - [X] create semantic types
        - [X] identifier
        - [X] scope
        - [X] module
        - [X] program

    - [ ] create semantic analysts
        - [ ] types creator
        - [ ] identifier checker
        - [ ] type checker
        - [ ] optimizer
