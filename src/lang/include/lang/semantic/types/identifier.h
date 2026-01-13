#pragma once

#include <climits>
#include <cstddef>
#include <lang/ast/ast.h>
#include <lang/common/symbol_path.h>

namespace lang::semantic
{
    struct SymbolID
    {
        size_t id{ULLONG_MAX};

        bool operator==(const SymbolID& ohter) const noexcept {
            return id == ohter.id;
        }
    };
    struct Symbol
    {
        /**
         * @brief   for error checking
         */
        bool is_valid{true};

        /**
         * @brief unique id of symbol
         */
        SymbolID id;

        /**
         * @brief   kind of symbol
         */
        enum class Kind {
            VARIABLE,
            FUNCTOIN,
            MODULE,
            TYPE,
            NOT_VALID 
        };

        /**
         * @brief   kind of symbol: var, func, module or types
         */
        Kind kind{Symbol::Kind::NOT_VALID};

        /**
         * @brief   pointer to node where it were declared
         * @note    for modules - nullptr
         */
        ast::DeclStmt* decl{nullptr};

        /**
         * @brief   SymbolPath to identifier
         */
        SymbolPath sympath;

        bool operator==(const Symbol& other) {
            if(kind != other.kind) return false;
            if(kind != Kind::MODULE && decl == other.decl) return true;
            return sympath == other.sympath;
        }
    };
}

namespace std {
    /**
     * @todo:   optimize by cache of hash
     */
    template<>
    struct hash<lang::semantic::SymbolID> {
        size_t operator()(const lang::semantic::SymbolID& id) const {
            return hash<size_t>{}(id.id);
        }
    };

    // /**
    //  * @todo:   optimize by cache of hash
    //  */
    // template<>
    // struct hash<lang::semantic::Symbol> {
    //     size_t operator()(const lang::semantic::Symbol& symbol) const {
    //         return hash<lang::semantic::SymbolID>{}(symbol.id);
    //     }
    // };
}

/**
 * @brief   saved old code down
 * @todo    remove old code
 */
namespace lang::semantic
{
    // class Scope;
    // struct Identifier {
    // public: 
    //     enum class Kind {
    //         VARIABLE,
    //         FUNCTOIN,
    //         MODULE,
    //         TYPE
    //     };
    // public:

    //     /**
    //      * @brief kind of identifier: var, func, module or types
    //      */
    //     Kind kind;
        
    //     /**
    //      * @brief pointer to node where it were declared, for modules - nullptr
    //      */
    //     ast::DeclStmt* decl{nullptr};

    //     /**
    //      * @brief absolute path to identifier (from global scope)
    //      */
    //     SymbolPath name;

    //     Identifier(SymbolPath _name
    //     ,          Kind _kind
    //     ,          ast::DeclStmt* _decl = nullptr
    //     ):  name(std::move(_name))
    //     ,   kind(_kind)
    //     ,   decl(_decl)
    //     {}
    // };
}
