#pragma once

#include <unordered_map>

#include <common/utils/tagged_id.h>
#include <common/utils/strings_storage.h>

#include <lang/common/symbol_path.h>
#include <lang/semantic/symbol.h>
#include <lang/semantic/types.h>
#include <lang/semantic/typestable.h>

namespace lang::semantic {
	class Scope;
	using ScopeID = common::utils::TaggedID<Scope>;

	class Scope {
	private:
		/**
		 * @brief pointer to parent scope; nullptr - iether it is a global, OR hiest local scope
		 */
		Scope* parent{nullptr};

		int64_t next_id{0};

		common::memory::IPoolAlloc* pool;

		std::pmr::unordered_map<StringID, SymbolID> symbols;
		std::pmr::unordered_map<SymbolID, Symbol*> symbols_context;

		TypesTable _types_table;

	public:
		explicit Scope(
			common::memory::IPoolAlloc* _pool
		,	Scope* _parent = nullptr
		):	parent(_parent)
		,		symbols(_pool->get_resource())
		,		symbols_context(_pool->get_resource())
		,		_types_table(_pool)
		{}

		// returns true if it is root scope (e.g. root private scope, or root global scope)
		inline bool is_final() const noexcept {
			return parent == nullptr;
		}

		//  const Scope* get_parent() const noexcept;
		//  Scope* get_parent() noexcept;

		SymbolID add_symbol(StringID, Symbol*);
		inline TypesTable& types_table() noexcept {
			return _types_table;
		}

		// return false in case of cannot find (symbol/type can be in global scope)
		inline bool contains(StringID id) const noexcept {
			if(contains_local(id)) return true;
			if(parent) return parent->contains(id);
			return false;
		}

		inline bool contains_local(StringID id) const noexcept {
			return 	symbols.contains(id)
			&&			_types_table.contains(id)
			;
		}

		SymbolID get_symbol_by_name(StringID name) {
			if(!contains(name)) throw common::diagnostic::InterError("undefind name");
			if(contains_local(name)) return symbols.at(name);
			if(parent) return parent->get_symbol_by_name(name);
			throw common::diagnostic::InterError("unreacheble: cannot get symbol, that contains");
		}
	};
}

