#pragma once

#include <memory>
#include <unordered_map>

#include <lang/common/symbol_path.h>
#include <lang/semantic/types/symbol.h>
#include <lang/semantic/types/typesystem.h>

namespace lang::semantic 
{
    class Scope
    {
    private:
        /**
         * @brief pointer to parent scope; nullptr - it is global scope
         */
        Scope* parent{nullptr};

        /**
         * @brief list of symbol in scope
         */
        std::pmr::unordered_map<SymbolID, Symbol*> symbols;

    public:
        explicit Scope(Scope* _parent = nullptr):
            parent(_parent)
        {}

        bool is_global() const noexcept;

        const Scope* get_parent() const noexcept;
        Scope* get_parent() noexcept;

        /**
         * @brief   check if given symbol contains from this scope to global
         */
        bool contains(SymbolID) const noexcept;
        /**
         * @brief   check if given symbol contains from this scope to global
         */
        bool contains(TypeID) const noexcept;

        /**
         * @brief   check if given symbol contains in THIS scope only
         */
        bool contains_local(SymbolID) const noexcept;

        /**
         * @brief   check if given type contains in THIS scope only
         */
        bool contains_local(TypeID) const noexcept;


        void add(SymbolID, Symbol*);
        const Symbol* get(SymbolID) const noexcept;
        Symbol* get(SymbolID) noexcept;
        Symbol& at(SymbolID);

        void add(TypeID, Symbol*);
        const AbstractType* get(TypeID) const noexcept;
        AbstractType* get(TypeID) noexcept;
        AbstractType& at(TypeID);

    };
}
