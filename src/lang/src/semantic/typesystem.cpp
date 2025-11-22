#include <lang/utils/error.h>
#include <lang/semantic/typesystem.h>

namespace lang
{
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
    bool QualType::is_reference() const noexcept {
        return flags & Flags::REFERENCE;
    }

    bool QualType::is_function() const noexcept {
        return flags & Flags::FUNCTOIN;
    }

    bool QualType::is_pointer() const noexcept {
        return flags & Flags::POINTER;
    }

    bool QualType::is_const() const noexcept {
        return flags & Flags::CONST;
    }

    std::string_view QualType::get_name() const noexcept {
        return type->name;
    }

// TypeTable

    const Type* TypeTable::add_type(std::string_view name) {
        if(contains(name)) Error("Re-addition of type to typetable");
        
        auto type = std::make_unique<Type>(name);
        table[type->name] = std::move(type);
        return  table[name].get();
    }

    const Type* TypeTable::add_type(std::string_view name, TypeInfo info) {
        if(contains(name)) throw Error("Re-addition of type to typetable");

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
