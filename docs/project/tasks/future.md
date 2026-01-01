# Future tasks (plans)

moved all stuff here that is not soon

- [ ] design module (files and logic)
    - [ ] check if file was edited (by hash)
        - [ ] crypto library
        - [ ] maybe some data structure's

    - [ ] file format (some JSON stuff)
        - [ ] needs JSON lib
        - [ ] serialization
            <!-- - [ ] AST serializer (ConstVisitor BTW) no needs, but still wants to dump :)-->
            - [ ] Module's(data structure) serializer (TypeTable, Scope, Identifier, etc)
            - [ ] SerializationDriver class
        - [ ] deserialization (and deserializer)
            - [ ] AST deserializer (ConstVisitor BTW)
            - [ ] Module(data structure) deserializer (TypeTable, Scope, Identifier, etc)
            - [ ] main DeSerializer class

- [ ] todo some day(after 0:0 version), wrote just to not forget
    - [ ] change DeclNode api -> std::string_view get_name() -> const std::string& get_name()
    - [ ] remove ModuleDecl node (file_name is module name)
    - [ ] replace namespaces with modules (remove namespaces)
    - [ ] use precompiled headers
    - [ ] design module (for increment compilation)
    - [ ] multi-threading

- [ ] language features after 0:0 version
    - [ ] structs
    - [ ] alias types
    - [ ] base std
