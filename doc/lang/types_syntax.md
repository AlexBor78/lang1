## Structure
**Everything is prefix**
`[wrapper mod][wrapper][type mod][type]`
- *optional* Wrapper mod - `const` or `mutable` (`mutable` by default)
- *optional* Wrapper - `*`, `&` pointer or link
- *optional* Type mod - `const` or `mutable` (`mutable` by default)
- *required* Type - type name(e.g. `int`, `double` `MyStruct`) or other type(e. g. `cosnt char*`, `fn(int, int) -> int`, etc)
### `fn` keyword
**`fn` keyword is used ONLY for types of function. not to declare functions**
*BTW: you cannot create object of function, only ptr to fn. Function objects will be it's a part of default library, as a structure with fn ptr inside*
- `*fn() foo;` pointer to function without args that returns `void`
- `*fn(type, type) -> type foo;` pointer to function with 2 args that returns `type`

## Examples
### With `()` to get it
- `(int) x;` - just integer variable
- `(*(int)) x;`- pointer to integer var (or array)
- `(const(*(int))) x;` const pointer to mutable int (u cannot edit pointer itself, but can change integers  in memory)
- `(*(const(int))) x;` mutable pointer to const int (u can edit pointer itself, but cannot change integers in memory)
- `(*(*(const(int)))) x;` mutable pointer to mutable pointer to const int
- `(*(fn(int, int))) x;` pointer to function `fn(int, int) -> void`
- `(*(fn(int, int) -> int)) x;` pointer to function `fn(int, int) -> int`

### Without `()` to see in real use
- `int x;` - just integer variable
- `*int x;`- pointer to integer var (or array)
- `const* int x;` const pointer to mutable int 
- `* const int x;` mutable pointer to const int 
- `** const int x;` mutable pointer to mutable pointer to const int
- `*fn(int, int) x;` poi[[Types syntax]]nter to function `fn(int, int) -> void`
- `*fn(int, int) -> int x;` pointer to function `fn(int, int) -> int`
