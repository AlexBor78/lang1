#include <lang/utils/error.h>
#include <lang/semantic/typesystem.h>

namespace lang
{
// TypeFlagsAPI

    TypeFlagsAPI::Flags TypeFlagsAPI::get_flags() const noexcept {
        return flags;
    }

    bool TypeFlagsAPI::is_reference() const noexcept {
        return flags & Flags::REFERENCE;
    }

    bool TypeFlagsAPI::is_function() const noexcept {
        return flags & Flags::FUNCTOIN;
    }

    bool TypeFlagsAPI::is_pointer() const noexcept {
        return flags & Flags::POINTER;
    }

    bool TypeFlagsAPI::is_const() const noexcept {
        return flags & Flags::CONST;
    }

    std::string_view QualType::get_name() const noexcept {
        return type->name;
    }

// QualType

    const Type* QualType::get_raw_type() const noexcept {
        return type;
    }
    bool QualType::is_equal(const Type* other) const noexcept {
        return type == other;
    }
    bool QualType::is_equal(QualType other) const noexcept {
        return type == other.type && flags == other.flags;
    }

// TypeTable

    const Type* TypeTable::add_type(std::string_view name) {
        if(contains(name)) errors::InterError("Re-addition of type to typetable");
        
        auto type = std::make_unique<Type>(name);
        table[type->name] = std::move(type);
        return  table[name].get();
    }

    const Type* TypeTable::add_type(std::string_view name, TypeInfo info) {
        if(contains(name)) throw errors::InterError("Re-addition of type to typetable");

        auto type = std::make_unique<Type>(name, info);
        table[type->name] = std::move(type);
        return  table[name].get();
    }

    const Type* TypeTable::get_type(std::string_view name) const noexcept {
        if(table.contains(name)) return table.at(name).get();
        else return nullptr;
    }

    bool TypeTable::contains(std::string_view name) const noexcept {
        return table.contains(name);
    }
}
