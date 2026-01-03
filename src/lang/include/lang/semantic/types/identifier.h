#pragma once

#include <memory>
#include <lang/ast/ast.h>
#include <lang/semantic/types/scope.h>

namespace lang::semantic
{
    class Scope;
    struct Identifier {
    public: 
        enum class Kind {
            VARIABLE,
            FUNCTOIN,
            TYPE,
            NAMESPACE
        };
    public:
        static std::unique_ptr<Identifier> create(
            std::string_view name,
            Kind kind,
            ast::DeclStmt* decl,
            Scope* parent_scope = nullptr
        );

        std::string name;
        Kind kind;
        ast::DeclStmt* decl{nullptr};

        // for namespace or function
        std::unique_ptr<Scope> inner{nullptr};

        Identifier(std::string_view _name
        ,          Kind _kind
        ,          ast::DeclStmt* _decl
        ,          std::unique_ptr<Scope> _inner = nullptr
        ):  name(_name)
        ,   kind(_kind)
        ,   decl(_decl)
        ,   inner(std::move(_inner))
        {}
    };
}
