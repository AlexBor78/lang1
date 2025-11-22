- rename class Tokenizer to Lexer
- extend lexer for 3 char op's and comments

- refactor parser
    - save node pos
    - add logger to parser
        - move stream.h to stream.h istream.h ostream.h

- extend parser
    - add if-else for while
    - allow stmt without {} for if-else, for while stmt

- semantic...
