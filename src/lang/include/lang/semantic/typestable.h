#pragma once

#include <unordered_map>
#include <lang/semantic/types.h>

namespace lang::semantic {
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
}

