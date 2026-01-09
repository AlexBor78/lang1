#pragma once

#include "lang/common/symbol_path.h"
#include <lang/ast/ast.h>

namespace lang::semantic
{
    struct SymbolID
    {
        /**
         * @brief kind of symbol
         * 
         */
        enum class Kind {
            VARIABLE,
            FUNCTOIN,
            MODULE,
            TYPE
        };

        /**
         * @brief kind of symbol: var, func, module or types
         */
        Kind kind;

        /**
         * @brief pointer to node where it were declared
         * @note for modules - nullptr
         */
        ast::DeclStmt* decl{nullptr};

        /**
         * @brief absolute path to identifier
         * @note using to generate hash
         */
        SymbolPath path;

        bool operator==(const SymbolID& other) {
            if(kind != other.kind) return false;
            if(kind != Kind::MODULE && decl == other.decl) return true;
            return path == other.path;
        }
    };
}

namespace std {
    /**
     * @todo: optimize by cache of hash
     */
    template<>
    struct hash<lang::semantic::SymbolID> {
        size_t operator()(const lang::semantic::SymbolID& id) const {
            return hash<lang::SymbolPath>{}(id.path);
        }
    };
}

/**
 * @brief saved old code down
 * @todo remove old code
 */
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
