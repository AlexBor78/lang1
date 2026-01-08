# Issues

## Unknown
- [X] **Maybe in some pipeline stuff:** segfault if add too much cycle imports

## Lexer
- [X] fails if **one-line-comment** in end of file

## Parser
- [X] don't process last symbols right if code is wrong (e.g. missed ';')
- [X] peek() out of range if something like "IDENTIFIER ;" in end of file
- [X] not working not binary operations (`a + b + c` is ont working, but `a + (b + c)` is working)

## Diagnostic
- [X] usual fail to generate detailed error
