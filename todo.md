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
    - [ ] rename src/compiler folder to src/app or something similar
    - [ ] use Logger
        - [ ] maybe stream idk
        - [ ] Lexer
        - [ ] Parser

    - refactor Position
        - [ ] rename Position to SourceLocation 
        - [ ] add end pos (and remove length)
        - [X] set pos.line and pos.column in stream

    - [ ] refactor and extend Error
        - [ ] upgrade output
        - [ ] handle multi-line tokens
        - [ ] add FrontendError's <- CompileError
            - [ ] add StreamError   <- FrontendError
            - [ ] add LexerError    <- FrontendError
            - [ ] add ParserError   <- FrontendError
        - [ ] add MiddlendError's <- CompileError
            - [ ] add SemanticError's <- MiddlendError

    - [ ] update api
        - [ ] add and use match() function
            - Stream - doesn't need
            - Lexer - doesn't need
            - [ ] Parser
        - [ ] change peak -> peek
            - [X] in Stream
            - [X] in Lexer
            - [ ] in Parser
        - [ ] change next -> advance
            - [X] in Stream
            - [X] in Lexer
            - [ ] in Parser
        - [ ] add skip
            - [X] in Stream
            - [X] in Lexer
            - [ ] in Parser

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
