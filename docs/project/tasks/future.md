# Future tasks (plans)

moved all stuff here that is not soon

- [ ] design modules system (files and logic)
    - [ ] check if file was edited (by hash)
        - [ ] crypto library
        - [ ] maybe some data structure's

    - [ ] file format (some JSON stuff)
        - [ ] needs JSON lib
        - [ ] serialization
            <!-- - [ ] AST serializer no needs, but still wants to dump :)-->
            - [ ] Module's(data structure) serializer (TypeTable, Scope, Identifier, etc)
            - [ ] SerializationDriver class
        - [ ] deserialization (and deserializer)
            - [ ] AST deserializer (ConstVisitor BTW)
            - [ ] Module(data structure) deserializer (TypeTable, Scope, Identifier, etc)
            - [ ] main Deserializer class

- [ ] todo some day(after 0:0 version), wrote just to not forget
    - [ ] change DeclNode api -> std::string_view get_name() -> const std::string& get_name()
    - [X] remove ModuleDecl node (file_name is module name)
    - [X] replace namespaces with modules (remove namespaces)
    - [ ] use precompiled headers in project
    - [ ] design modules system (for increment compilation)
    - [ ] multi-threading

- [ ] language features after 0:0 version
    - [ ] structs
    - [ ] alias types
    - [ ] base std
