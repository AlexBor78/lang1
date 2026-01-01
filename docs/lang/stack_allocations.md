# Main Idea
**I made stack allocation syntactically different from declaration for arrays**

## Syntax
we use keyword `stack` to reserve memory on stack (will allocated on start)
```C

*char array <- stack[10]; // not initialized
*int buff <- stack[1024] = {}; // null initialization
*int numbers <- stack[] = {1, 2, 3, 4}; // initialized array with size 4
```
