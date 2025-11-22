#include <lang/semantic/identifier.h>
#include <lang/utils/error.h>
#include <lang/semantic/scope.h>

namespace lang::semantic
{
    bool Scope::is_global() const noexcept {
        return parent == nullptr;
    }

    bool Scope::contains_local(const std::string& name) const noexcept {
        return  identifiers.contains(name) || typetable.contains(name);
    }

    bool Scope::contains(const std::string& name) const noexcept {
        if(!parent) return contains_local(name);
        if(contains_local(name)) return true;
        return parent->contains(name);
    }

    bool Scope::is_identifier(const std::string& name) const noexcept {
        if(!parent) return identifiers.contains(name);
        return identifiers.contains(name) || parent->is_identifier(name);
    }

    bool Scope::is_type(const std::string& name) const noexcept {
        if(!parent) return typetable.contains(name);
        return typetable.contains(name) || parent->is_type(name);
    }

    const Identifier* Scope::get_identifier(const std::string& name) const noexcept {
        if(identifiers.contains(name)) return identifiers.at(name).get();
        if(parent) return parent->get_identifier(name);
        else return nullptr;
    }

    Identifier* Scope::get_identifier(const std::string& name) noexcept {
        // don't return parents identifier for edit
        if(identifiers.contains(name)) return identifiers.at(name).get();
        else return nullptr;
    }

    const Type* Scope::get_type(const std::string& name) const noexcept {
        if(!parent || typetable.contains(name)) return typetable.get_type(name);
        return parent->get_type(name);
    }

    const Scope* Scope::get_parent() const noexcept {
        return  parent;
    }
    Scope* Scope::get_parent() noexcept {
        return  parent;
    }

    const TypeTable& Scope::get_typetable() const noexcept {
        return typetable;
    }

    TypeTable& Scope::get_typetable() noexcept {
        return typetable;
    }

    void Scope::add_identifier(std::unique_ptr<Identifier> identifier) {
        if(contains_local(identifier->name)) throw Error("Re-addition of identifier to scope");
        identifiers[identifier->name] = std::move(identifier);
    }
}
