#pragma once

#include <climits>
#include <cstddef>
#include <variant>

#include <common/memory/arena_alloc.h>
#include <common/utils/basic_id.h>
#include <lang/common/symbol_path.h>
#include <lang/syntax/ast/stmt.h>

#include <lang/semantic/types/typesystem.h>

namespace lang::semantic {
	class Symbol;
	class Scope;
	struct Module;
	using SymbolID 	= common::utils::BasicID<Symbol>;
	using ScopeID 	= common::utils::BasicID<Scope>;
	using TypeID		= common::utils::BasicID<AbstractType>;
	using ModuleID	= common::utils::BasicID<Module>;

	struct TypeSymbol {
	};

	struct VarSymbol {
		syntax::DeclVariable* decl;
		AbstractType* type;
	};

	struct FuncSymbol {
		syntax::DeclFunction* decl;
		AbstractType* return_type;
		AbstractType* type;
		ScopeID scope;
	};

	struct Module {
		ScopeID scope;
		common::memory::ArenaAlloc* arena{nullptr};

	};

	class Symbol {
	private:
		std::variant<
			TypeSymbol
		,	VarSymbol
		,	FuncSymbol
		,	Module
		> symbol;

	public:
//		SymbolID id;
//		SymbolPath sympath;

	public:
		explicit Symbol(VarSymbol v) : symbol(std::move(v)) {}
		explicit Symbol(TypeSymbol t) : symbol(std::move(t)) {}
		explicit Symbol(FuncSymbol f) : symbol(std::move(f)) {}
		explicit Symbol(Module m) : symbol(std::move(m)) {}

		template <class T>
		inline const T* as() const noexcept {
			return std::get_if<T>(&symbol);
		}

		template <class T>
		inline T* as() noexcept {
			return std::get_if<T>(&symbol);
		}
	};
}
