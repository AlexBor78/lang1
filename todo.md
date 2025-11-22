- [ ] Lexer
    - [X] rename class Tokenizer to Lexer 
    - [X] extend lexer for 3 char op's and comments
    - [ ] add more literals (0xFF, etc)

- [ ] done Logger
    - [ ] add .log to gitignore
    - [ ] done streams
        - [ ] move stream.h to stream.h istream.h ostream.h
        - [ ] create ConsoleOSteam
        - [ ] create FileOStream

- [ ] project code refactor
    - refactor Position
        - [ ] rename Position to SourceLocation and add end pos (and remove length)
        - [ ] set Position.line and Positions.column in stream
        
    - refactor error
        - [ ] upgrade output
        - [ ] handle multi-line tokens
        - [ ] add ParserError
        - [ ] add SemanticError
        - [ ] add LexerError

    - [ ] refactor parser
        - [ ] save node pos
        - [ ] add logger to parser

- [ ] extend parser
    - [ ] add if-else for while
    - [ ] allow stmt without {} for if-else, for while stmt

- [ ] add user-api to compiler 
    - [ ] multi-file handling (import module, than use module file)
    - [ ] pass insert files in args
    - [ ] pass paths to find modules

- [ ] semantic...
    - [ ] create semantic analysts
        - [ ] types creator
        - [ ] identifier checker
        - [ ] type checker
        - [ ] optimizer

