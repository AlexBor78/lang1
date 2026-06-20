#include <lang/semantic/scope.h>

namespace lang::semantic {
		SymbolID Scope::add_symbol(StringID name, Symbol* ptr) {
			if(symbols.contains(name))
				throw common::diagnostic::InterError("symbol redefinition");
			SymbolID id(next_id++);
			symbols[name] = id;
			symbols_context[id] = ptr;
			return id;
		}
}
