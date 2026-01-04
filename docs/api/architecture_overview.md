# Architecture overview

This section describes how the project is organized on disk and in code, not the internal logic.

## Navigation

- [Short overview](#short-overview)
- [File structure](#file-structure)
    - [overview](#overview)
    - [common](#common)
    - [lang](#lang)
    - [compiler app](#compiler-app)
- [Code architecture](#code-architecture)
    - [overview](#overview-1)
    - [common](#common-2)
    - [lang](#lang-1)
    - [compiler app](#compiler-app-1)

## Short overview

There are **3** modules
- common        - some tools, that can be used in different project's (e.g. logger, diagnostics)
- lang          - all about language itself, compilation, all defined, etc. most of code here, can be used as library
- compiler_app  - cli interface of compiler, just layer between language and user (**main function** right here)

In every module, usually there is at list one submodule (if not, it will be in future).
most modules have some sort of common stuff, or are highly likely to have it.

- common
    - diagnostic
    - streams
    - utils
- lang
    - ast
    - syntax
    - semantic
    - pipeline
    - utils
- compiler app
    - cliparser
    - main function

## File Structure

### Overview

File structure kinda **following** the code organization:

- if there is namespace - all files of that namespace placed in dir with namespace_name (in both `include` and `src`)
- trying to fill code most efficient way:
    - in headers: try to combine stuff that composed, or using often together.
      trying to not create to much files, but still divide mostly and organize
    
    - in sources: but here the other way - as less files as better:
      bcs of headers, we don't want to have much .cpp files, 
        but still organize them almost like headers, 
        and don't make to big

There are **3** modules
- [common](#common)
- [lang](#lang)
- [compiler_app](#compiler_app)

in every module's root dir there is the `CMakeLists.txt` file, that contains 
- module as target
- list of all .cpp files
- link to dependency libraries (e.g. lang use common, OR lang will use LLVM)

that mean, we can borrow module to other projects without problems (useful for common), 
and also make structure of cmake files cleaner.

every module have `include` and `src` dirs,
- `include` - all header. **using inner dir** to more detailed includes (and kind safety)
- `src`     - all implementations (that can be in .cpp files, e.g. `constexpr`, `inline`, etc must be in header)

### Common

**include inner directory: `common`**

in common dir placed very common stuff, that is not tied to project, can be reused in 
other projects.

so, lets see on files trees

headers:
```
src/common/include/
└── common
    ├── common.h
    ├── diagnostic
    │   ├── diagnostic_builder.h
    │   └── diagnostic.h
    ├── streams
    │   ├── istream.h
    │   ├── ostream.h
    │   └── stream.h
    └── utils
        └── logger.h
```

and compare to code structure (not full)

```cpp
namespace common
{
    inline void debug_break();
    struct FileLocation;
    struct SourceLocation

    namespace diagnostic {
        class DiagnosticBuilder;

        class Diagnostic : public std::exception;
        // ...
    }

    namespace streams {
        class AbstractStream;

        class InputStream : public AbstractStream;
        class OutputStream : public AbstractStream;
        // ...
    }

    namespace utils {
        class Logger;
    }
}
```

now compare to .cpp files
```
src/common/src/
├── diagnostic
│   └── diagnostic_builder.cpp
├── streams
│   ├── istream.cpp
│   ├── ostream.cpp
│   └── stream.cpp
└── utils
    └── logger.cpp
```

As we can see, headers follow code structure,
every namespace have inner - dir (with similar name).

but .cpp files, still similar, but make easier by merge some files.

more info about every piece see in [code architecture](#code-architecture)

### lang

Lang is main part of project, it's contains the entire language:
- compilation (pipeline)
- syntax (Lexer, Parser)
- ast (Abstract Syntax Tree)
- main language logic (semantic)

and i **recommend** read [how compilation work](not-written-yet) in that project, parallel with structure.

And now we can move to file's structure, (i don't wanna show one big tree).

as i sad before, lang have some namespaces(and inner dirs):
- [AST](#ast)       - Abstract Syntax Tree kinda IR
- [syntax](#syntax)    - first step on compilation, text -> AST
- [semantic](#semantic)  - analyze tree, checks for most errors
- codegen  - not created yet, only in plans translate AST -> LLVM IR
- [pipeline](pPipeline)  - control process of compilation
- [utils](#utils)     - some utilities and tools

and, as project grow, there are also [common](#common-1) part, 
that used in project, but not utility or piece of other parts.

### Common

Right now that is appears very recently, and not contains much, but i sure it will grow/

```
src/lang/include/lang/common/
└── symbol_path.h
```

- `symbol_path.h` path to symbol, symbol such as Function, Variable, Modules, etc

#### AST

AST have 4 main files:
```
src/lang/include/lang/ast/
├── ast.h
├── expr.h
├── stmt.h
└── visitor.h
```

- `ast.h`   (and `ast.cpp`)  just main defines, e.g. `BaseNode`, `StmtNode`, `ExprNode`
- `stmt.h`  (and `stmt.cpp`) all statements, e.g. `ImportStmt`, `ContinueStmt`, `BreakStmt`
- `expr.h`  (and `expr.cpp`) all expressions e.g. `NumberLiteral`, `UnaryOpExpr`, `BinOpExpr`
- `visitor.h` - declaration of `ASTVisitor` and `ConstASTVisitor` (visitor pattern)

#### Syntax

*syntax* is slightly bigger than *ast* bcs have `Parser` and `Lexer`:
```
src/lang/include/lang/syntax/
├── keywords.h
├── lexer.h
├── parser.h
├── syntax_container.h
└── token.h
```

Common part (used in lexer and parser)
- `token.h`

Lexer part
- `lexer.h`
- `keywords.h`

Parser part
- `parser.h`
- `syntax_container.h`

#### Semantic

*Semantic* is a lot more bigger, bcs have bunch of semantic types and semantic analyzers.

so structure shows it:
```
src/lang/include/lang/semantic/
├── analyzers
│   ├── import_analyzer.h
│   ├── name_checker.h
│   └── symbols_collector.h
└── types
    ├── identifier.h
    ├── module.h
    ├── program.h
    ├── scope.h
    ├── semantic_types.h
    └── typesystem.h
```

#### Pipeline

*Pipeline* have just drivers, that control process of compilation (and using other classes)
```
src/lang/include/lang/pipeline/
├── compiledriver.h
├── compile_options.h
├── compile_state.h
├── modules_loader.h
├── semantic_driver.h
└── syntax_driver.h
```

there are also some defines in `compile_options.h` and `compile_state.h`,
everything other is just drivers.

#### Utils

*Utils* is commonly used in project, bcs contains some very useful utils/tools
```
src/lang/include/lang/utils/
├── ast_utils.h
├── diagnostic.h
├── printast.h
├── semantic_utils.h
└── syntax_utils.h
```

- `diagnostic.h` there inherit warns and errors from common module.
- `ast_utils.h`, `syntax_utils.h` and `semantic_utils.h` just bunch of functions, that help write code.
- `printast.h` tool, that prints ast, e.g. for debug

### Compiler App

Compiler_app is just cli part, compiler itself as app, 
depends on language module.
right now it's not really big, in future that will grow very much.
here also main function.
(so small i can show whole tree)
```
src/compiler_app/
├── CMakeLists.txt
├── include
│   └── compiler
│       └── cliparser.h
└── src
    ├── cliparser.cpp
    └── main.cpp
```

- `cliparser.h` parse input from user to *compile_option*(from lang)
- main.cpp contains main functions.

## Code Architecture

code architecture divided to 3 modules
- [common](#common-2)
- [lang](#lang-1)
- [compiler app](#compiler-app-1)

see more about that [here](#short-overview).

### Common

in common module there are
- [diagnostic](#diagnostic)
- [streams](#streams)
- [utils](#utils-1)

and also there are some [common](#common-3) stuff for common module. (to much common words)

#### Common

```cpp
namespace common {
    inline void debug_break();
    struct FileLocation;
    struct SourceLocation;
}
```

- `debug_break()`   it's function for creating breakpoints, use hardware instructions
- `FileLocation`    it's structure to store position in file, contain just point
- `SourceLocation`  it's structure to store Source location, contains file name, start and end points

#### Diagnostic

```cpp
namespace common::diagnostic {
    class DiagnosticBuilder;

    class Diagnostic : public std::exception;
    class Error : public Diagnostic;
    class InterError : public Error;
    class Warn : public Diagnostic;
}
```

- `DiagnosticBuilder` it's class to build diagnostic message (message about error, or warn)
- `Diagnostic`        it's abstract class for Error and Warn, root of diagnostic
- `Error` and `Warn`  it's abstract classes inheritors of `Diagnostic`
- `InterError`        it's class inheritor of `Error`, so we can use it's objects

#### Streams

```cpp
namespace common::streams {
    class AbstractStream;

    class InputStream : public AbstractStream;
    class FileIStream : public InputStream;
    class StringIStream : public InputStream;
    class ConsoleIStream : public InputStream;

    class OutputStream : public AbstractStream;
    class FileOStream : public OutputStream;
    class StringOStream : public OutputStream;
    class ConsoleOStream : public OutputStream;
    class ConsoleErrOStream : public OutputStream;
}
```

- `AbstractStream` abstract class, root of streams
- `InputStream`    abstract class, contain implementation with `std::istream`
    - `FileIStream`       class inheritor to use as FileStream
    - `StringIStream`     class inheritor to use as StringStream
    - `ConsoleIStream`    class inheritor to use as ConsoleStream
- `OutputStream`    abstract class, contain implementation with `std::ostream`
    - `FileOStream`       class inheritor to use as FileStream
    - `StringOStream`     class inheritor to use as StringStream
    - `ConsoleOStream`    class inheritor to use as ConsoleStream
    - `ConsoleErrOStream` class inheritor to use as ConsoleErrStream

#### Utils

```cpp
namespace common::utils {
    class Logger;
}
```

- Logger - tool to log, use streams. have levels.

### Lang


in lang module there are
- [ast](#ast-1)
- [syntax](#syntax-1)
- [semantic](#semantic-1)
- [pipeline](#pipeline-1)
- [diagnostic](#diagnostic-1)
- [utils](#utils-2)

and also we have some [common](#common-4) stuff here

#### Common

```cpp 
namespace lang {
    struct SymbolPath;
}
```

- SymbolPath is path to code-symbol (e.g. module, function, variable, etc)

#### AST

here defined an ast-nodes, to create ast (used in parser).
also here defined ast visitors.

```cpp
namespace ast
{
    class BaseNode;
    class StmtNode : public BaseNode;
    class ExprNode : public StmtNode;

    class ASTVisitor;
    class ConstASTVisitor;

// control flow
    class BlockStmt : public StmtNode;
    class StructureStmt : public StmtNode;
    class StructureStmtWithCond : public StructureStmt;
    class IfStmt : public StructureStmtWithCond;
    class ElseStmt : public StructureStmt;
    class ForStmt : public StructureStmtWithCond;
    class WhileStmt : public StructureStmtWithCond;

// decls
    class DeclStmt : public StmtNode;
    class DeclName : DeclStmt;
    class DeclVariable : public DeclName;
    class DeclFunction : public DeclName;
    class DeclModule : public DeclName; // deprecated
    class DeclNamespace : public DeclName; // deprecated in future

// other stmts
    class ImportStmt : StmtNode;
    class ReturnStmt : StmtNode;
    class BreakStmt : StmtNode;
    class ContinueStmt : StmtNode;
}
```

#### Syntax

in syntax we have some types, and 2 main part - Lexer and Parser

```cpp
namespace syntax {
    enum class TokenType;
    struct Token;
    namespace lexer {
        class lexer;
        inline std::unordered_map<std::string_view, TokenType> keywords;
    }
    namespace parser {
        struct SyntaxContainer;
        class Parser;
    }
}
```

- `TokenType` used in `Token`       - enum of all tokens
- `Token` used in lexer             - structure of token, contains `TokenType`, symbol(string), and position
- `keywords` used in lexer          - dictionary of all keywords (and also all punctuation)
- `SyntaxContainer` used in parser  - struct with AST and context (e.g. extern_list, export_list)

#### Semantic

here semantic types, and semantic analyzers

some of types right now in root module namespace, i thin i will move them to semantic.
```cpp
namespace lang
{
    class AbstractType;
    class WrapperType : public AbstractType;
    class BaseType : public AbstractType;

    struct TypeInfo;
    class BuiltinType : public BaseType;
    class UnsolvedType : public BaseType;
    class FunctionType : public BaseType;
}
```

- `AbstractType` - abstract class need for polymorphism
- `BaseType`     - abstract class for types, not wrappers
- `TypeInfo`     - struct with basic info about type, used in built in (e.g. is_numeric, is_signed, etc)
- `BuiltinType`  - class of builtin type
- `FunctionType` - class of function type
- `UnsolvedType` - class of unsolved type, used in on stage, before semantic create "solved" types
- `WrapperType`  - class stands for wrap types (e.g. const int, its )

```cpp
namespace lang::Semantic {
    // types
    struct  Identifier;
    class   Scope;
    struct  Module;
    struct  Program;
    class   SemanticContext;
    class   SemanticState;
    class   SemanticInitializer;

    // analyzers
    class ImportAnalyzer : public ConstASTVisitor;
    class SymbolsCollector : public ASTVisitor; // none const => needs non-const ptr in Identifier (to edit in future)
}
```

- `Module` and `Identifier` - structs to organize semantic data
- `Scope`                   - class, also control state, with semantic data
- `Program`                 - main struct of semantic data
- `SemanticContext`         - struct organize other semantic data
- `SemanticState`           - combine all other semantic structs

- `ImportAnalyzer`      - collect all imports, and return vector of them
- `SymbolsCollector`    - collect all defined imports (not in use yet)

#### Pipeline

controllers of compilation process 

```cpp
namespace pipeline {
    struct  CompileOptions;
    class   CompileDriver;
    class   SyntaxDriver;
    class   ModulesLoader;
    class   SemanticDriver;
}
```

- `CompileOptions`  - struct with compile options (like input files, flags, etc)
- `SyntaxDriver`    - loads given file, and return `SyntaxContainer`
- `ModulesLoader`   - loads all imported modules from given files (use `ImportAnalyzer`)
- `SemanticDriver`  - use and start all semantic analyzers
- `CompileDriver`   - use all other drivers, to drive compile process

### Diagnostic

Diagnostic also create structure with OOP, so we can sort what we catch

```cpp
namespace lang::diagnostic {
    class CompileError : public common::diagnostic::Error;
    class CompileWarn : public common::diagnostic::Warn;

    class SyntaxError : public CompileError;
        class LexerError : public SyntaxError;
        class ParserError : public SyntaxError;

    class SyntaxWarn : public CompileWarn;
        class LexerError : public SyntaxError;
        class ParserWarn : public SyntaxError;

    // in future
    // class SemanticError : public CompileError; // commented, still not in use
    // class SemanticWarn : public CompileWarn;
}
```

- `CompileError` and `CompileWarn`      - abstract classes, inherit base `Error` and `Warn` classes
- `SyntaxError` and `SyntaxWarn`        - abstract class, to sort error
    - `LexerError` and `LexerWarn`      - diagnostics of `Lexer`
    - `ParserError` and `ParserWarn`    - diagnostics of `Parser`

diagnostic message builded by `DiagnosticBuilder` from common module.

#### utils

just bunch of utils, used in project.

```cpp
namespace lang::utils
{
    // enum types to string
    constexpr const char* stringify(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr const char* stringify(syntax::TokenType) noexcept;

    // print token or vector of tokens
    void print(const std::vector<syntax::Token>&) noexcept;
    void print(const syntax::Token&) noexcept;

    // TokenType to OperatorExpr::OperatorKind
    constexpr ast::OperatorExpr::OperatorKind token_to_op(syntax::TokenType);

    // print ast or vector of ast's
    class ASTPrinter : public ast::ConstASTVisitor;
    void print(const std::vector<std::unique_ptr<ast::BaseNode>>&) noexcept;
    void print(const ast::BaseNode*) noexcept;

    // used in parser
    constexpr bool is_declarator(syntax::TokenType) noexcept;
    constexpr bool is_operator(syntax::TokenType) noexcept;
    constexpr bool is_literal(syntax::TokenType) noexcept;

    // used in parser
    constexpr bool is_unary_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_prefix_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_postfix_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_mul_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_add_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_compare_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_logical_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_assign_op(ast::OperatorExpr::OperatorKind) noexcept;
}
```

- `is_` functions       - used in Parser to check easily
- `stringify` functions - used by `PrintAST`
- `print` functions     - use `PrintAST` and `stringify` to print given
- `PrintAST`            - class inherit of `ConstASTVisitor`, print AST


### Compiler App

compiler app is not really growth yet, but it will in future.

also ere defined **main function**.

```cpp
namespace compiler_app {
    struct ParsedFlag;
    class CLIParser;
}
```

- `CLIParser`   - class, parsing input from user, and return `ParsedFlag`
- `ParsedFlag`  - structure that reflects user input in practical form
