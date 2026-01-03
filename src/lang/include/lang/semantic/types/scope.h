#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include <lang/semantic/types/typesystem.h>

// bcf of that include clang going crazy, but code still WORK
// #include <lang/semantic/types/identifier.h>

namespace lang::semantic 
{
    class Identifier;
    class Scope
    {
    private:
        Scope* parent{nullptr};
        std::unordered_map<std::string, std::unique_ptr<Identifier>> identifiers;
        std::vector<std::unique_ptr<Scope>> child_scopes;
        TypeTable typetable;

    public:
        explicit Scope(Scope* _parent = nullptr):
            parent(_parent)
        {}

        bool is_global() const noexcept;
        bool contains(const std::string&) const noexcept;
        bool contains_local(const std::string&) const noexcept;
        bool is_identifier(const std::string&) const noexcept;
        bool is_type(const std::string&) const noexcept;

        const Identifier* get_identifier(const std::string&) const noexcept;
        Identifier* get_identifier(const std::string&) noexcept;
        const AbstractType* get_type(const std::string&) const noexcept;

        const Scope* get_parent() const noexcept;
        Scope* get_parent() noexcept;

        Scope* add_child();

        const TypeTable& get_typetable() const noexcept;
        TypeTable& get_typetable() noexcept;

        void add_identifier(std::unique_ptr<Identifier>);
    };
}
