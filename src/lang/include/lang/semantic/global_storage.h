#pragma once

#include <unordered_map>
#include <lang/semantic/scope.h>

namespace lang::semantic {
	class GlobalStorage {
	private:
		int64_t next_scope_id{0};
		int64_t next_symbol_id{0};
		common::memory::IPoolAlloc* pool;
		std::pmr::unordered_map<ScopeID, Scope*> scopes;
		std::pmr::unordered_map<SymbolID, Symbol*> symbols;
		
	public:
		GlobalStorage(
			common::memory::IPoolAlloc* _pool
		):	pool(_pool)
		,		scopes(pool->get_resource())
		,		symbols(pool->get_resource())
		{}

		inline ScopeID add(Scope* ptr) {
			ScopeID id(next_scope_id++);
			scopes[id] = ptr;
			return id;
		}
		SymbolID add(Symbol* ptr) {
			SymbolID id(next_symbol_id++);
			symbols[id] = ptr;
			return id;
		}

		inline Scope* get(ScopeID id) {
			return scopes.at(id);
		}
		inline Symbol* get(SymbolID id) {
			return symbols.at(id);
		}
	};
}

