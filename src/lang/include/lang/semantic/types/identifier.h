#pragma once

#include <climits>
#include <cstddef>
#include <variant>
#include <lang/common/symbol_path.h>
#include <lang/syntax/stmt.h>

namespace lang::semantic {
	 	class Symbol;
  	class SymbolID {
		private:
				bool global{false};
        union {
				  size_t id{ULLONG_MAX};
				  Symbol* symptr;
				};
 		public:
        inline bool operator==(const SymbolID& ohter) const noexcept {
            return id == ohter.id;
        }
				inline bool is_global() const noexcept {return global;}
				inline size_t get_global_id() const noexcept {return id;}
				inline Symbol* get_local_ptr() noexcept {return symptr;}
				inline const Symbol* get_local_ptr() const noexcept {return symptr;}
    };
}

namespace std {
    template<>
    struct hash<lang::semantic::SymbolID> {
        inline size_t operator()(const lang::semantic::SymbolID& id) const {
            return hash<size_t>{}(id.get_global_id());
        }
    };
}

namespace lang::semantic {

		struct TypeSymbol {
		};

		struct VarSymbol {
				syntax::DeclVariable* decl;
				SymbolID type;
		};

		struct FuncSymbol {
				syntax::DeclFunction* decl;
				SymbolID return_type;
				SymbolID type;
		};

		struct ModuleSymbol {

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
        SymbolID id;
        SymbolPath sympath;

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


/**
 * @brief   saved old code down
 * @todo    remove old code
 */
namespace lang::semantic
{
    // class Scope;
    // struct Identifier {
    // public: 
    //     enum class Kind {
    //         VARIABLE,
    //         FUNCTOIN,
    //         MODULE,
    //         TYPE
    //     };
    // public:

    //     /**
    //      * @brief kind of identifier: var, func, module or types
    //      */
    //     Kind kind;
        
    //     /**
    //      * @brief pointer to node where it were declared, for modules - nullptr
    //      */
    //     ast::DeclStmt* decl{nullptr};

    //     /**
    //      * @brief absolute path to identifier (from global scope)
    //      */
    //     SymbolPath name;

    //     Identifier(SymbolPath _name
    //     ,          Kind _kind
    //     ,          ast::DeclStmt* _decl = nullptr
    //     ):  name(std::move(_name))
    //     ,   kind(_kind)
    //     ,   decl(_decl)
    //     {}
    // };
}
