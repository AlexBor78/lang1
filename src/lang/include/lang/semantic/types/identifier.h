#pragma once

#include "lang/common/symbol_path.h"
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
            MODULE
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

        /**
         * @brief path to identifier from global scope
         */
        SymbolPath full_name;

        /**
         * @brief for module or function (and structs in future i think)
         */
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
