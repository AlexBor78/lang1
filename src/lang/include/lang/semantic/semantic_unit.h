#pragma once

#include <lang/semantic/hir/hir.h>
#include <lang/semantic/symbol.h>

namespace lang::semantic {
	struct SemanticUnit {
		hir::BaseNode* hir;
		SymbolID module;

	};
}
