#pragma once

#include <vector>
#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <common/common.h>
#include <common/memory/pool_alloc.h>
#include <common/utils/basic_id.h>
#include <common/utils/strings_storage.h>

/**
 * @todo move to lang::semantic::typesystem namespace or something like that
 */
namespace lang {
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

	using CoreTypeID = common::utils::BasicID<CoreType>;
	using TypeID = common::utils::BasicID<AbstractType>;

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

  /**
   * @brief temporary type for parser, just name of type, 
            will not be unique: if N "int" vars -> N temporary types with name "int"
            live until semantic
   * 
   */
  class UnresolvedType : public CoreType {
  public:
  	/**
  	 * @brief name of type, save here to process in semantic
		 * @note	string is allocated in SourceFile
  	 */
  	std::string_view name;

  	UnresolvedType() = default;
  	UnresolvedType(
			std::string_view _name
  	):  name(_name)
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

  constexpr WrapperKind operator|(WrapperKind, WrapperKind) noexcept;
  constexpr WrapperKind& operator|=(WrapperKind&, WrapperKind) noexcept;
  constexpr bool operator&(WrapperKind, WrapperKind) noexcept;

}

namespace std {
	template<>
	struct hash<std::pair<lang::WrapperKind, common::utils::BasicID<lang::AbstractType>>> {
		size_t operator()(const std::pair<lang::WrapperKind, common::utils::BasicID<lang::AbstractType>>& p) const noexcept {
			size_t h1 = std::hash<uint8_t>{}(static_cast<uint8_t>(p.first));
			size_t h2 = std::hash<common::utils::BasicID<lang::AbstractType>>{}(p.second);
			
			// (boost hash_combine style)
			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}
	};
	template<>
	struct hash<std::pair<lang::WrapperKind, common::utils::BasicID<lang::CoreType>>> {
		size_t operator()(const std::pair<lang::WrapperKind, common::utils::BasicID<lang::AbstractType>>& p) const noexcept {
			size_t h1 = std::hash<uint8_t>{}(static_cast<uint8_t>(p.first));
			size_t h2 = std::hash<common::utils::BasicID<lang::AbstractType>>{}(p.second);
			
			// (boost hash_combine style)
			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}
	};
}

namespace lang {
class TypesTable {
	private:
		size_t next_core_id{0};
		size_t next_wrapper_id{0};
		common::memory::IPoolAlloc* pool;
	
		// CoreTypes
	  std::pmr::unordered_map<StringID, CoreTypeID> core_table;
		std::pmr::unordered_map<CoreTypeID, CoreType*> core_context;
	
		// WrapperTypes
		std::pmr::unordered_map<
			std::pair<WrapperKind, TypeID>
		,	TypeID
		>	wrapper_table;
		std::pmr::unordered_map<
			std::pair<WrapperKind, CoreTypeID>
		,	TypeID
		>	core_wrapper_table;
		std::pmr::unordered_map<TypeID, WrapperType*> wrapper_context;
	
	public:
		TypesTable(
			common::memory::IPoolAlloc* _pool
		):	pool(_pool)
		,		core_table(pool->get_resource())
		,		core_context(pool->get_resource())
		,		wrapper_table(pool->get_resource())
		,		wrapper_context(pool->get_resource())
		{}
	
		// CoreTypes
		CoreTypeID add_core(StringID);
	  CoreTypeID add_builtin(StringID, TypeInfo = TypeInfo());
	  bool contains(StringID) const noexcept;
	  CoreTypeID get_core(StringID) const noexcept;
		const CoreType* get_core(CoreTypeID) const noexcept;
	
		// WrapperTypes
		TypeID wrap(WrapperKind, CoreTypeID);
		TypeID wrap(WrapperKind, TypeID);
	
		WrapperType* get_wrapper(TypeID);
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
