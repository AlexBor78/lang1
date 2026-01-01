# Issus

## Unknown
- [ ] **Maybe in some pipeline stuff:** segfault if add too much cycle imports

## Lexer
- [ ] fails if **one-line-comment** in end of file

## Parser
- [ ] don't process last symbols right if code is wrong (e.g. missed ';')
- [ ] peek() out of range if something like "IDENTIFIER ;" in end of file
- [ ] not working not binary operations (`a + b + c` is ont working, but `a + (b + c)` is working)
