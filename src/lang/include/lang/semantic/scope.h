#pragma once

#include <unordered_map>

#include <common/utils/basic_id.h>
#include <common/utils/strings_storage.h>

#include <lang/common/symbol_path.h>
#include <lang/semantic/symbol.h>
#include <lang/semantic/types.h>
#include <lang/semantic/typestable.h>

namespace lang::semantic {
	class Scope;
	using ScopeID = common::utils::BasicID<Scope>;

	class Scope {
	private:
		/**
		 * @brief pointer to parent scope; nullptr - iether it is a global, OR hiest local scope
		 */
		Scope* parent{nullptr};

		common::memory::IPoolAlloc* pool;

		std::pmr::unordered_map<StringID, SymbolID> symbols;
		std::pmr::unordered_map<SymbolID, Symbol*> symbols_context;

		TypesTable types_table;

	public:
		explicit Scope(
			common::memory::IPoolAlloc* _pool
		,	Scope* _parent = nullptr
		):	parent(_parent)
		,		symbols(_pool->get_resource())
		,		symbols_context(_pool->get_resource())
		,		types_table(_pool)
		{}

		bool is_global() const noexcept;

		//  const Scope* get_parent() const noexcept;
		//  Scope* get_parent() noexcept;

		bool contains(StringID) const noexcept;
		bool contains(SymbolID) const noexcept;
		bool contains(CoreTypeID) const noexcept;

		bool contains_local(StringID) const noexcept;
		bool contains_local(SymbolID) const noexcept;
		bool contains_local(CoreTypeID) const noexcept;

		Symbol* add_symbol(StringID);
		CoreTypeID add_type(StringID);
		SymbolID get_symbol_by_name(StringID) noexcept;
		CoreTypeID get_type_by_name(StringID) noexcept;
	};
}

