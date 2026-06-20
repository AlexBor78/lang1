#pragma once

#include <variant>

#include <common/memory/arena_alloc.h>
//#include <lang/common/symbol_path.h>

#include <lang/semantic/typesystem.h>

namespace lang::semantic {
	class Scope;
	class Symbol;
	using ScopeID 	= common::utils::BasicID<Scope>;
	using SymbolID 	= common::utils::BasicID<Symbol>;

	namespace hir {
		class DeclVar;
		class DeclFunc;
	}

	struct TypeSymbol {
	};

	struct VarSymbol {
		hir::DeclVar* decl;
		TypeID type;
	};

	struct FuncSymbol {
		hir::DeclFunc* decl;
		TypeID return_type;
		TypeID type;
		ScopeID scope;
	};

	struct Module {
		ScopeID public_scope; 	// allocated in "global storage"
		ScopeID private_scope;	// allocated in thread-local memory
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
		inline bool is() const noexcept {
	    return std::holds_alternative<T>(symbol);
		}

		template <class T>
		inline T& as() {
			return std::get<T>(symbol);
		}

		template <class T>
		inline const T& as() const {
			return std::get<T>(symbol);
		}
	};
}
