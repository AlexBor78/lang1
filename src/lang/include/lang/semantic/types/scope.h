#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <lang/common/symbol_path.h>
#include <lang/semantic/types/typesystem.h>

// bcf of that include clangd going crazy, but code still WORK
// #include <lang/semantic/types/identifier.h>

namespace lang::semantic 
{
    class Identifier;
    class Scope
    {
    private:
        Scope* parent{nullptr};
        std::unordered_map<SymbolPath, std::unique_ptr<Identifier>> identifiers;
        std::vector<std::unique_ptr<Scope>> child_scopes;
        TypeTable typetable;

    public:
        explicit Scope(Scope* _parent = nullptr):
            parent(_parent)
        {}

        /**
         * @brief check if this scope is global
         * 
         * @return true 
         * @return false 
         */
        bool is_global() const noexcept;

        /**
         * @brief check if given symbol contains from this scope to global
         * @return true 
         * @return false 
         */
        bool contains(const SymbolPath&) const noexcept;

        /**
         * @brief check if given symbol contains in TTHIS scope only
         * 
         * @return true 
         * @return false 
         */
        bool contains_local(const SymbolPath&) const noexcept;

        /**
         * @brief check if given symbol is identifier
         * 
         * @return true 
         * @return false 
         */
        bool is_identifier(const SymbolPath&) const noexcept;
        bool is_type(const SymbolPath&) const noexcept;

        const Identifier* get_identifier(const SymbolPath&) const noexcept;
        Identifier* get_identifier(const SymbolPath&) noexcept;
        const AbstractType* get_type(const SymbolPath&) const noexcept;

        const Scope* get_parent() const noexcept;
        Scope* get_parent() noexcept;

        Scope* add_child();

        const TypeTable& get_typetable() const noexcept;
        TypeTable& get_typetable() noexcept;

        void add_identifier(std::unique_ptr<Identifier>);
    };
}
