#pragma once

#include <vector>
#include <cstdint>
#include <common/common.h>
#include <common/utils/basic_id.h>
#include <common/utils/strings_storage.h>

namespace lang::semantic {
  /**
   * @brief class needed to have polymorphism with CoreType and wrappers types
   */
  class AbstractType {
  protected:
  	AbstractType() = default;

  public:
  	virtual ~AbstractType() = default;
  };

  /**
   * @brief Abstraction used in TypesTable
            CoreType - Type without Wrappers, 
            it can be BuiltinType, FunctionType or UserType (e.g. strcut, alias)
   * 
   */
  class CoreType : public AbstractType {
  protected:
		using AbstractType::AbstractType;

	public:
  	virtual bool is_builtin() {return false;}
  };

	using CoreTypeID = common::utils::TaggedID<CoreType>;
	using TypeID = common::utils::TaggedID<AbstractType>;

  /**
   * @brief struct to easier configure builtin types
   */
  struct TypeInfo {
  	bool is_numeric{false};
  	bool is_integer{false};
  	bool is_signed{false};
		int8_t size{0}; // size in bytes
  };
  
  class BuiltInType : public CoreType {
  public:
  	StringID name;
  	TypeInfo info;

  	virtual bool is_builtin() override {return true;}

  	explicit BuiltInType(
			StringID _name
  	, TypeInfo _info = TypeInfo()
  	):  name(_name)
  	,   info(_info)
  	{}
  };

  class FunctionType : public CoreType {
  public:
  	TypeID return_type;
  	std::pmr::vector<TypeID> args_types;

  	FunctionType() = default;
  	FunctionType(
			std::pmr::vector<TypeID> _args_types
  	, TypeID _return_type
  	):	return_type(std::move(_return_type))
		,		args_types(std::move(_args_types))
  	{}
  };


	enum class WrapperKind : uint8_t {
		CONST,
		MUTABLE,
		POINTER,
		REFERENCE,
	};

  class WrapperType : public AbstractType {
  protected: // var
  	WrapperKind kind{WrapperKind::MUTABLE};
		bool final{false};
  	CoreTypeID base_type;
  	TypeID inner;

  public: // api
		using WrapperKind = WrapperKind;
  	WrapperType() = default;
  	WrapperType(
			WrapperKind _kind
  	, TypeID _inner
  	):  kind(_kind)
  	,   inner(std::move(_inner))
  	{}

  	WrapperType(
			WrapperKind _kind
  	, CoreTypeID 	_base
  	):  kind(_kind)
		,		final(true)
  	,   base_type(_base)
  	{}
  	
  	bool is_final() const noexcept;
  	TypeID unwrap() const;
  	CoreTypeID unwrap_to_core() const noexcept;
  	WrapperKind get_kind() const noexcept;
  };

	inline constexpr WrapperKind operator|(WrapperKind a, WrapperKind b) noexcept {
		return static_cast<WrapperKind>(
			static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
		);
	}
	
	inline constexpr WrapperKind& operator|=(WrapperKind& a, WrapperKind b) noexcept {
		return a = static_cast<WrapperKind>(
			static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
		);
	}
	
	inline constexpr bool operator&(WrapperKind a, WrapperKind b) noexcept {
		return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
	}
}

namespace std {
	template<>
	struct hash<std::pair<lang::semantic::WrapperKind, common::utils::TaggedID<lang::semantic::AbstractType>>> {
		size_t operator()(const std::pair<lang::semantic::WrapperKind, common::utils::TaggedID<lang::semantic::AbstractType>>& p) const noexcept {
			size_t h1 = std::hash<uint8_t>{}(static_cast<uint8_t>(p.first));
			size_t h2 = std::hash<common::utils::TaggedID<lang::semantic::AbstractType>>{}(p.second);
			
			// (boost hash_combine style)
			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}
	};
	template<>
	struct hash<std::pair<lang::semantic::WrapperKind, common::utils::TaggedID<lang::semantic::CoreType>>> {
		size_t operator()(const std::pair<lang::semantic::WrapperKind, common::utils::TaggedID<lang::semantic::AbstractType>>& p) const noexcept {
			size_t h1 = std::hash<uint8_t>{}(static_cast<uint8_t>(p.first));
			size_t h2 = std::hash<common::utils::TaggedID<lang::semantic::AbstractType>>{}(p.second);
			
			// (boost hash_combine style)
			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}
	};
}
