#pragma once

#include <lang/syntax/ast/ast.h>

namespace lang::syntax {
	struct TranslationBundle {
		ast::AST ast;
		
		TranslationBundle(
			ast::AST _ast
		):	ast(std::move(_ast))
		{}
	};
}
