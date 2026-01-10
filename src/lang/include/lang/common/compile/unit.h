#pragma once

#include <cstddef>
#include <climits>

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include <lang/ast/ast.h>
#include <lang/common/symbol_path.h>

namespace lang
{
    struct UnitID
    {
        /**
         * @brief   for error checking as return result
         * @todo    give better name
         */
        bool is_valid{true};

        /**
         * @brief   unique ID of Unit, use to abstract from filepath (where we cannot specify it yet)
           @note    optional, if there is no id, BUT there is sympath or filepath -> Manager will try to use it
         * @note    initialized with maximum. that is not valid value BTW
         */
        size_t id{ULLONG_MAX};

        /**
         * @brief   temporary(or not) store symbol path of module
           @note    optional
         * @note    needs by ModulesLoader
         * @note    can be used to create SymbolID of module, and find in SymbolsContext
         */
        SymbolPath symbolpath;

        /**
         * @brief   absolute file path
         * @note    optional
         */
        std::string filepath;

        /**
         * @brief   compare this to other object
         * @note    try to compare in that way: id -> sympath -> filepath -> false
         * @param   other second object ot compare
         * @return  true 
         * @return  false 
         */
        bool operator==(const UnitID& other) const noexcept {
            if(!is_valid || !other.is_valid) return false;
            if(id != ULLONG_MAX && other.id != ULLONG_MAX) return id == other.id;
            if(!symbolpath.empty() && !other.symbolpath.empty()) return symbolpath == other.symbolpath;
            if(!filepath.empty() && !other.filepath.empty()) return filepath == other.filepath;
            return false;
        }

        /**
         * @brief   Construct a new UnitID object
         * @note    is_valid is true by default
         */
        UnitID() = default;
        
        /**
         * @brief   Construct a new UnitID object, need to create unvalid object faster
         * @param   _is_valid 
         */
        UnitID(bool _is_valid
        ): is_valid(_is_valid)
        {}
    };

    struct CompileUnit
    {
        /**
         * @brief   ID of itself
         */
        UnitID id;

        /**
         * @brief   Abstract Syntax tree
         */
        ast::AST ast;

        /**
         * @brief   list of dependencies
         * @todo    sort and use binary search
         */
        std::vector<UnitID> dependencies;

        /**
         * @brief   list of submodules
         * @todo    sort and use binary search
         */
        std::vector<UnitID> submodules;

        CompileUnit() = default;
        CompileUnit(UnitID _id
        ,           ast::AST _ast
        ,           std::vector<UnitID> _dependencies = {}
        ,           std::vector<UnitID> _submodules = {}
        ): id(std::move(_id))
        ,  ast(std::move(_ast))
        ,  dependencies(std::move(_dependencies))
        ,  submodules(std::move(_submodules))
        {}
    };

    /**
     * @brief   store and manage CompieUnits and their's IDs
     */
    class CompileUnitsManager
    {
    private:

        /**
         * @brief   counter of units, needs to create abstract id
         */
        size_t counter{0};

        /**
         * @brief   hashtable abstract id -> index in array
         * @note    need to use abstract from indexes, e.g. for sort+binary_search in future, or change store container
         */
        std::unordered_map<size_t, size_t>      indexes_context;

        /**
         * @brief   hashtable symbolpath -> abstract id
         */
        std::unordered_map<SymbolPath, size_t>  sympaths_context;

        /**
         * @brief   hashtable filepath -> abstract id
         */
        std::unordered_map<std::string, size_t> filepaths_context;

        /**
         * @brief   array of all units, 
         */
        std::vector<std::unique_ptr<CompileUnit>> units;

        /**
         * @brief   tries to complete id from contexts
         * @note    don't change not empty fields (even don't right)
         * @return  UnitID 
         */
        UnitID try_complete(UnitID) const;

    public:

        /**
         * @brief   allocate new CompileUnit in memory, and generate new ID for it
         * @note    it's also allocate new CompileUnit, and initialize `id` field
         * @return  new UnitID 
         */
        UnitID gen_new_id();

        /**
         * @brief   update stored info in contexts
         * @note    the only way to save filepath or sympath context
         * @note    it don't change not-empty info to empty (e.g. update only filepath if given sympath is empty)
         */
        void update_contexts(UnitID);

        /**
         * @brief   non-const getter by id
         * @note    will try to use sympath and filepath after id.id
         * @return  const CompileUnit* 
         */
        CompileUnit* get(UnitID);

        /**
         * @brief   const getter by id
         * @note    will try to use sympath and filepath after id.id
         * @return  const CompileUnit* 
         */
        const CompileUnit* get(UnitID) const;

        /**
         * @brief   check if contains unit with given id
         * 
         * @return  true 
         * @return  false 
         */
        bool contains(UnitID) const noexcept;

        /**
         * @brief   check if contains unit with given sympath
         * 
         * @return  true 
         * @return  false 
         */
        bool contains(SymbolPath) const noexcept;

        /**
         * @brief   check if contains unit with given filepath
         * 
         * @return  true 
         * @return  false 
         */
        bool contains(const std::string&) const noexcept;
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
