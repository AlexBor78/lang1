#pragma once

#include <lang/syntax/ast/ast.h>

namespace lang::syntax {
	struct TranslationBundle {
		syntax::AST ast;
		
		TranslationBundle(
			syntax::AST _ast
		):	ast(std::move(_ast))
		{}
	};
}
