# Ideas list
Ideas in work, not sure will they implemented, 
not sure about they final look, but want to save and think about it.


## Multiple imports

**Reason:** too much imports

**Description:** write `import` keyword once, and then all modules u want to import

example:
```
import {
    module_a, // absolute path
    module_b, // absolute path
    .module_c // relative path
};
```

also i think will works with `export import`, that will make code a lot better.
example:
```
export import {
    .submodule_a,
    .submodule_b,
    .submodule_c
};
```

## The `as` keyword

**Reason:** better code organization, less conflicts, easier solving

**Description:** specify imported module's name

examples:
```
import module as x;
import {
    module_a as a,
    module_b as b
};
```

not sure if it good to use with `export import`
```
export import .submodule_a as a;
export import {
    .submodule_b as b,
    .submodule_c as c
};
```

example of using in code renamed module:
```
import module_a as a;

int main() {
    a::foo();
    // module_a::foo() // will not work
    return 0;
}
```

## Parted imports

**Reason:** if u don't want to import whole module but some symbol

not sure how it will work with `as`

example:
```
import module_a::x;
import module_b::{y, x};
import {
    module_c,
    module_d::foo,
    module_e::{
        bar, baz
    }
};
```

use example:
```
import math::sin;
import std::io::println;

int main() {
    println(sin(52));
    return 0;
}
```

## Include C-headers

**Reason:** make FFI easier - u don't have to define all symbols from C again

**Description:** you pass path to header, than it processed and all symbols defined in it scanned 
    (don't sure about marcos, i think only more like vars, functions, structure, not sure),
    **not** Implementations, u need to link compiled C code with code in my language
    i think it shouldn't work with `parted import`

examples (not sure about syntax yet):
```
import C"path/to/header_a.h";
import {
    C"path/to/header_a.h",
    C"path/to/header_b.h"
};
```

example in combination with `as` keyword (not sure)
```
import {
    C"path/to/header_a.h" as a,
    C"path/to/header_b.h" as b
};
```

## Parser assert

**Reason:** test parser

**Description:** to test parser, you write Keyword, and stmt **WITH**, it will check if exception was thrown

**Keyword:** `Parser_Assert:` (yeah with `:`)
example:
```
Parser_Assert:
import .module::submodule::-::name;
```
**About implementation:** after that keyword parser will test it, then continue parsing if test passed
testing stmt will not added to file's AST
