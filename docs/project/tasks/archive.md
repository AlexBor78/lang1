# Tasks archive

## Really early stuff

### Streams
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

### Logger
- [X] add .log to gitignore
- [X] implement logger

### Parser
- [X] extend AST(BreakStmt, ContinueStmt)
- [X] add END token 
- [X] add if-else for while
- [X] allow stmt without {} for if-else, for, while, stmt

### Lexer
- [X] rename class Tokenizer to Lexer 
- [X] extend lexer for 3 char op's and comments
- [X] use lexer api instead of stream api (use steam api IN imp of lexer api)

### AST
- [X] remove module decl node

### semantic
- [X] create semantic types
    - [X] identifier
    - [X] scope
    - [X] module
    - [X] program
    - [X] ProgramInitializer (Program name, global_scope, etc)
- [X] remove noexcept from ast visitors
- [X] may be change semantic types a little bit if needed.
- [X] save export-import to context IN parser
- [X] remove ImportAnalyzer (basically - ImportsCollector)
- [X] clean up files organisation:
    - [X] move compile_options to common
    - [X] move compile state to common
    - [X] move Program to common
    - [X] create CompileUnit in common
- [X] update SymbolPath Structure
    (or create one more, that have absolute and relative SymbolPath, and bool flag)
    and use instead of SymbolPath (like FileLocation -> SourceLocation in past)
- [^] update semantic types architecture
    - [X] divide `semantic::Module` into 1 separated structure and SymbolID:
        - [X] CompileUnit - compile unit. not symbol 
                use it's own UnitID
                stored in Program data structure
        - [X] ModuleSym   - semantic symbol of module (like namespace)
                not needed as dedicated structure - SymbolID + context save all data
    - [X] remove deprecated shit from `Program`, save semantic context in it.
    - [X] make `semantic::Identifier` into universal SymbolID in semantic stage (instead of SymbolPath, ModuleID, pointer to node)
    - [X] integrate update SymbolPath into existing codebase
    - [X] make SymbolsContext that will be used to store all symbols (using scope, context, TypesTable, etc, and have api)
        note: SymbolsCon

### Once of refactoring: redesign memory use (using allocators now); change semantic types; etc
1. [X] redesign memory usage
    [X] introduce ArenaAloc
    [X] Lexer part
      [X] introduce SourceFile (structure that store WHOLE file)
      [X] store all files in one arena (till end of compilation)
      [X] do not store strings in tokens, only string views
      [X] adapt lexer to use SourceFile
    [X] Parser part
      [X] change all smart ptrs in ast to stuped ones
      [X] allocate all ast in separated arena


2. [X] redesign AST 
    [X] add whole bunch off shit that been moved to contexts
    [X] move to ArenaAloc for AST
    [X] adapt parser to use AST
3. [X] HIR (Basicaly second AST with different data)
    [X] redesign semantic types (scope, symbol)
    [X] design HIR
      [X] introduce StringsPool to store strings and use string_views


## Documentation stage
- [X] add doxygen
- [X] reorganize and clean up docs
    - [X] tasks
    - [X] roadmap
- [X] move and clean up all docs we already had
- [X] add doxygen to project
- [X] clean-up roadmap, dived notes and roadmap, sort all notes
- [X] remove clean.sh and run.sh and write one good script (not ai generated)
- [X] decide gitignore generated doc or not - IGNORE
- [X] mv doc -> docs
- [X] write architecture overviews
- [X] are we even need architecture_plan.txt ? - NO

## pre-semantic syntax updates (enough for now, but still not supported a lot)
- [X] update keywords
- [X] update type's syntax, and type's Types (better wrappers with OOP)
- [X] impl base modules and remove namespace's
- [X] impl stack allocations (as in doc)
- [X] new type's syntax AND ne type's Types architecture (to have infinity wrappers)
- [X] remove namespaces at all
- [X] update keywords
- [X] add keywords, nodes, etc for new features
- [X] pointer to function
- [X] implement new modules (at list everything without increment build)
- [X] `export import` support
- [X] stack allocations syntax support

## pre-semantic finalization
- [X] fix fail of lexer if one-line comment in end of file
- [X] fix parser error: peak out of range
- [X] save node pos in parser
- [X] fix `a + b + c` like exprs
- [X] fix falling while generating diagnostic message

## Other

### upgrade build.sh script
- [X] add build arg (debug)
- [X] use multiple cores
- [X] pass `DEBUGING` directive through cmake flags
