#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <string_view>
#include <unordered_map>

/**
 * @todo move to lang::semantic namespace
 * 
 */
namespace lang
{

    /**
     * @brief class needed to have polymorphism with BuiltInType and wrappers types
     * 
     */
    class AbstractType {
    protected:
        AbstractType() = default;

    public:
        virtual ~AbstractType() = default;
        virtual bool is_builtin() {return false;}
        // virtual AbstractType& operator=(AbstractType& other) = default;
    };

    /**
     * @brief Abstraction used in TypesTable
              BaseType - Type without Wrappers, 
              it can be BuiltinType or CustomType (e.g. strcut, alias)
     * 
     */
    class BaseType : public AbstractType {
    protected:
        BaseType() = default;
    };

    /**
     * @brief struct to easier configure builtin types
     * 
     */
    struct TypeInfo {
        bool is_numeric{false};
        bool is_integer{false};
        bool is_signed{false};
    };
    
    class BuiltInType : public BaseType {
    private:
        std::string name;
        TypeInfo info;
        
    public:
        inline std::string_view get_name() const noexcept {
            return name;
        }
        inline TypeInfo get_type_info() const noexcept {
            return info;
        }

        virtual bool is_builtin() override {return true;}
        explicit BuiltInType
        (             std::string_view _name
        ,             TypeInfo _info = TypeInfo{.is_numeric = false
                      ,                         .is_integer = false
                      ,                         .is_signed = false
                      }
        ):  name(_name)
        ,   info(_info)
        {}
    };

    class FunctionType : public BaseType {
    private:
        std::vector<std::unique_ptr<AbstractType>> args_types;
        std::unique_ptr<AbstractType> return_type;

    public:
        FunctionType() = default;
        FunctionType(
            std::vector<std::unique_ptr<AbstractType>> _args_types,
            std::unique_ptr<AbstractType> _return_type
        ): args_types(std::move(_args_types))
        ,  return_type(std::move(_return_type))
        {}
    };

     // alternative name - DeclNotes
    /**
     * @brief temporary type for parser, just name of type, 
              will not be unique: if N "int" vars -> N temporary types with name "int"
              live until semantic
     * 
     * 
     */
    class UnresolvedType : public BaseType {
    public:
        /**
         * @brief name of type, save here to process in semantic
         * 
         */
        std::string name;

        UnresolvedType() = default;
        UnresolvedType(std::string_view _name):  
            name(_name)
        {}
    };

    class WrapperType : public AbstractType
    {
    public: enum class WrapperKind : uint8_t {
            CONST,
            MUTABLE,
            POINTER,
            REFERENCE,
        };
    protected: // var
        WrapperKind kind{WrapperKind::MUTABLE};
        std::unique_ptr<AbstractType> inner;
        const BaseType* base_type{nullptr};

        bool is_final() const noexcept; // return base_type; if (base_type) -> it's final wrapper

    public: // api
        WrapperType() = default;
        WrapperType(WrapperKind _kind
        ,           std::unique_ptr<AbstractType> _inner
        ):  kind(_kind)
        ,   inner(std::move(_inner))
        {}
        WrapperType(WrapperKind _kind
        ,           const BaseType* _base
        ):  kind(_kind)
        ,   base_type(_base)
        {}
        
        WrapperKind get_kind() const noexcept;
        AbstractType* get_into() const noexcept;
        BaseType* get_base_type() const noexcept;
        // virtual WrapperType& operator=(WrapperType& other) {
        //     this->inner = std::move(other.inner);
        //     this->base_type = other.base_type;
        //     this->kind = other.kind;
        // }
    };

    constexpr WrapperType::WrapperKind operator|(WrapperType::WrapperKind, WrapperType::WrapperKind) noexcept;
    constexpr WrapperType::WrapperKind& operator|=(WrapperType::WrapperKind&, WrapperType::WrapperKind) noexcept;
    constexpr bool operator&(WrapperType::WrapperKind, WrapperType::WrapperKind) noexcept;

    class TypeTable
    {
    private:
        std::unordered_map<std::string_view, std::unique_ptr<BaseType>> table;

    public:
        TypeTable() = default;
        explicit TypeTable(std::unordered_map<std::string_view, std::unique_ptr<BaseType>> _table):
            table(std::move(_table))
        {}
        explicit TypeTable(std::unordered_map<std::string_view, std::unique_ptr<BaseType>>& _table):
            table(std::move(_table))
        {}
        explicit TypeTable(const TypeTable& _table) = delete;
        explicit TypeTable(TypeTable& _table):
            table(std::move(_table.table))
        {}
        explicit TypeTable(TypeTable&& _table):
            table(std::move(_table.table))
        {}

        const BaseType* get_type(std::string_view) const noexcept;

        const BaseType* add_builtin_type(std::string_view);
        const BaseType* add_builtin_type(std::string_view, TypeInfo);

        // const BaseType* add_type(std::string_view);
        // const BaseType* add_type(std::string_view, TypeInfo);
        bool contains(std::string_view) const noexcept;
    };


    constexpr WrapperType::WrapperKind operator|(WrapperType::WrapperKind a, WrapperType::WrapperKind b) noexcept {
        return static_cast<WrapperType::WrapperKind>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
        );
    }

    constexpr WrapperType::WrapperKind& operator|=(WrapperType::WrapperKind& a, WrapperType::WrapperKind b) noexcept {
        return a = static_cast<WrapperType::WrapperKind>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
        );
    }

    constexpr bool operator&(WrapperType::WrapperKind a, WrapperType::WrapperKind b) noexcept {
        return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
    }
}
