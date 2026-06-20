#pragma once

#include <lang/semantic/types/hir/hir.h>
#include <lang/semantic/types/symbol.h>

namespace lang::semantic {
	struct SemanticUnit {
		hir::BaseNode* hir;
		SymbolID module;

	};
}
