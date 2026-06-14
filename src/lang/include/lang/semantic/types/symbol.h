#pragma once

#include <climits>
#include <cstddef>
#include <variant>

#include <lang/common/symbol_path.h>
#include <lang/syntax/ast/stmt.h>

#include <lang/semantic/types/typesystem.h>

namespace lang::semantic {
	template<class T>
	class BasicID {
	private:
		bool global{false};
		union {
			size_t id{ULLONG_MAX};
			T* ptr;
		};

	public:
		inline bool operator==(const BasicID& ohter) const noexcept {
			return id == ohter.id;
		}
		inline bool is_global() const noexcept {return global;}
		inline size_t get_global_id() const noexcept {return id;}
		inline T* get_local_ptr() noexcept {return ptr;}
		inline const T* get_local_ptr() const noexcept {return ptr;}
	};
}

namespace std {
	template<class T>
	struct hash<lang::semantic::BasicID<T>> {
		inline size_t operator()(const lang::semantic::BasicID<T>& id) const {
			return hash<size_t>{}(id.get_global_id());
		}
	};
}

namespace lang::semantic {
	class Symbol;
	using SymbolID = BasicID<Symbol>;
	class Scope;
	using ScopeID = BasicID<Scope>;

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

	struct ModuleSymbol {
		ScopeID scope;

	};

	class Symbol {
	private:
		std::variant<
			TypeSymbol
		,	VarSymbol
		,	FuncSymbol
		,	ModuleSymbol
		> symbol;

	public:
//		SymbolID id;
//		SymbolPath sympath;

	public:
		explicit Symbol(VarSymbol v) : symbol(std::move(v)) {}
		explicit Symbol(TypeSymbol t) : symbol(std::move(t)) {}
		explicit Symbol(FuncSymbol f) : symbol(std::move(f)) {}
		explicit Symbol(ModuleSymbol m) : symbol(std::move(m)) {}

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
