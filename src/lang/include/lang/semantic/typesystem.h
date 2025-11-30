#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <string_view>
#include <unordered_map>

namespace lang
{    
    struct TypeInfo {
        bool is_numeric{false};
        bool is_integer{false};
        bool is_signed{false};
    };
    struct Type {
        std::string name;
        TypeInfo info;
        
        explicit Type(std::string_view _name
        ,             TypeInfo _info = TypeInfo{.is_numeric = false
                      ,                         .is_integer = false
                      ,                         .is_signed = false
                      }
        ):  name(_name)
        ,   info(_info)
        {}
    };

    class TypeFlagsAPI
    {
    public: enum class Flags : uint8_t {
            NONE        = 0,
            CONST       = 1 << 0,
            POINTER     = 1 << 1,
            REFERENCE   = 1 << 2,
            FUNCTOIN    = 1 << 3 // unused for now (not supported by syntax)
        };
    protected: // var
        Flags flags;

    public: // api
        TypeFlagsAPI(Flags _flags):
            flags(_flags)
        {}
        
        Flags get_flags() const noexcept;
        bool is_reference() const noexcept;
        bool is_function() const noexcept;
        bool is_pointer() const noexcept;
        bool is_const() const noexcept;
    };

    // alternative name - DeclNotes
    // temporary container with full semantic info of declaration
    struct SemanticBag : public TypeFlagsAPI{
        bool is_extern{false};
        std::string name;

        SemanticBag(): TypeFlagsAPI(TypeFlagsAPI::Flags::NONE) {}
        SemanticBag(std::string_view _name
        ,           Flags _flags
        ,           bool _is_extern = false
        ):  TypeFlagsAPI(_flags)
        ,   name(_name)
        ,   is_extern(_is_extern)
        {}
    };
    
    class QualType : public TypeFlagsAPI
    {
    private:
        const Type* type;

    public:
        explicit QualType(const Type* _type
        ,                 Flags _flags = Flags::NONE
        ):  TypeFlagsAPI(_flags)
        ,   type(_type)   
        {}

        const Type* get_raw_type() const noexcept;
        bool is_equal(const Type*) const noexcept;
        bool is_equal(QualType) const noexcept;

        std::string_view get_name() const noexcept;
    };
    
    constexpr TypeFlagsAPI::Flags operator|(TypeFlagsAPI::Flags, TypeFlagsAPI::Flags) noexcept;
    constexpr TypeFlagsAPI::Flags& operator|=(TypeFlagsAPI::Flags&, TypeFlagsAPI::Flags) noexcept;
    constexpr bool operator&(TypeFlagsAPI::Flags, TypeFlagsAPI::Flags) noexcept;

    class TypeTable
    {
    private:
        std::unordered_map<std::string_view, std::unique_ptr<Type>> table;

    public:
        TypeTable() = default;
        explicit TypeTable(std::unordered_map<std::string_view, std::unique_ptr<Type>> _table):
            table(std::move(_table))
        {}
        explicit TypeTable(std::unordered_map<std::string_view, std::unique_ptr<Type>>& _table):
            table(std::move(_table))
        {}
        explicit TypeTable(const TypeTable& _table) = delete;
        explicit TypeTable(TypeTable& _table):
            table(std::move(_table.table))
        {}
        explicit TypeTable(TypeTable&& _table):
            table(std::move(_table.table))
        {}

        const Type* get_type(std::string_view) const noexcept;
        const Type* add_type(std::string_view);
        const Type* add_type(std::string_view, TypeInfo);
        bool contains(std::string_view) const noexcept;
    };


    constexpr TypeFlagsAPI::Flags operator|(TypeFlagsAPI::Flags a, TypeFlagsAPI::Flags b) noexcept {
        return static_cast<TypeFlagsAPI::Flags>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
        );
    }

    constexpr TypeFlagsAPI::Flags& operator|=(TypeFlagsAPI::Flags& a, TypeFlagsAPI::Flags b) noexcept {
        return a = static_cast<TypeFlagsAPI::Flags>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
        );
    }

    constexpr bool operator&(TypeFlagsAPI::Flags a, TypeFlagsAPI::Flags b) noexcept {
        return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
    }
}
