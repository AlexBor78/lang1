#pragma once

#include "lang/common/symbol_path.h"
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
            MODULE,
            TYPE
        };
    public:

        /**
         * @brief kind of identifier: var, func, module or types
         */
        Kind kind;
        
        /**
         * @brief pointer to node where it were declared, for modules - nullptr
         */
        ast::DeclStmt* decl{nullptr};

        /**
         * @brief absolute path to identifier (from global scope)
         */
        SymbolPath name;

        Identifier(SymbolPath _name
        ,          Kind _kind
        ,          ast::DeclStmt* _decl = nullptr
        ):  name(std::move(_name))
        ,   kind(_kind)
        ,   decl(_decl)
        {}
    };
}
