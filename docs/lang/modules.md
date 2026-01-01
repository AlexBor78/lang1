# Main ideas

- **ONE file - ONE module**
- modules like namespaces

## Syntax
- **Absolute path `module::submodule`**
- **Relative path `.submodule` look from current module or it's parent module**

```C
import [module_name];
import [module_name]::[sub_module_name];

// all symbols "private" by default
// exporting symbol = making them public for module api
export [Symbol Declaration or Name];

// reexporting modules for create structure
export import [module_name];
```
### Examples
```C
// some module
// import std; // OR
import std::io;

// private api function
// will not acceseable out of module and it's submodules
* const char stringify(TypeB) {
	  // ...
}

// public api function
export void print_stuff(Type arg) {
	for(auto x : arg) {
		std::io::println(stringify(x));
	}
}
```

```C
// std module inner
export import .io; // reexporting module std::io
```

## File Structure
One file - one module
```
lang_core
├── lang_core.lang
├── pipeline
│   ├── codegen_driver.lang
│   ├── compile_driver.lang
│   ├── modules_loader.lang
│   ├── pipeline.lang
│   ├── semantic_driver.lang
│   └── syntax_driver.lang
├── syntax
│   ├── lexer.lang
│   ├── parser.lang
│   └── syntax.lang
├── ast
│   ├── ast.lang
│   ├── expr.lang
│   └── stmt.lang
├── semantic
│   ├── semantic.lang
│   └── types.lang
├── codegen
│   └── codegen.lang
```

and in lang_core.lang something like
```C
// module lang_core
export import .pipeline; // lang_core::pipline
export import .syntax;   // lang_core::syntax
export import .ast;      // lang_core::ast
export import .semantic; // lang_core::semantic
export import .codegen;  // lnag_core::codegen;
```

and in every inner `.lang` file something like this:
```C
// pipeline module
export import .compile_driver;   // lang_core::pipeline::compile_dirver
export import .syntax_driver;    // lang_core::pipeline::syntax_driver
export import .modules_loader;   // lang_core::pipeline::modules_loader
export import .semantic_driver;  // lang_core::pipeline::semantic_driver
export import .codgen_driver;    // lang_core::pipeline::codgen_driver
```
