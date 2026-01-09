#pragma once

#include <vector>
#include <string>
#include <lang/ast/ast.h>

namespace lang
{
    struct UnitID
    {
        /**
         * @brief absolute file path, to identify CompieUnit
         */
        std::string filepath;

        bool operator==(const UnitID& other) const noexcept {
            return filepath == other.filepath;
        }
    };

    struct CompileUnit
    {
        /**
         * @brief Abstract Syntax tree
         */
        ast::AST ast;

        /**
         * @brief ID of itself
         */
        UnitID id;

        /**
         * @brief list of dependencies
         * @todo  sort and use binary search
         */
        std::vector<UnitID> dependencies;

        /**
         * @brief list of submodules
         * @todo  sort and use binary search
         */
        std::vector<UnitID> submodules;
    };
}

namespace std {
    /**
     * @todo: optimize by cache of hash
     */
    template<>
    struct hash<lang::UnitID> {
        size_t operator()(const lang::UnitID& id) const {
            return hash<std::string>{}(id.filepath);
        }
    };
}
