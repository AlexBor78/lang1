#include <lang/utils/diagnostic.h>
#include <lang/semantic/types/typesystem.h>

namespace lang
{
// TypeFlagsAPI

    WrapperType::WrapperKind WrapperType::get_kind() const noexcept {
        return kind;
    }


// TypeTable

    const BaseType* TypeTable::add_builtin_type(std::string_view name) {
        if(contains(name)) common::diagnostic::InterError("Re-addition of type to typetable");
        
        auto type = std::make_unique<BuiltInType>(name);
        table[type->get_name()] = std::move(type);
        return  table[name].get();
    }

    const BaseType* TypeTable::add_builtin_type(std::string_view name, TypeInfo info) {
        if(contains(name)) throw common::diagnostic::InterError("Re-addition of type to typetable");

        auto type = std::make_unique<BuiltInType>(name, info);
        table[type->get_name()] = std::move(type);
        return  table[name].get();
    }

    const BaseType* TypeTable::get_type(std::string_view name) const noexcept {
        if(table.contains(name)) return table.at(name).get();
        else return nullptr;
    }

    bool TypeTable::contains(std::string_view name) const noexcept {
        return table.contains(name);
    }
}
