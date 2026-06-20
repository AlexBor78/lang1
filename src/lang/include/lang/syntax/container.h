#pragma once

#include <unordered_set>
#include <lang/syntax/ast/ast.h>
#include <lang/syntax/ast/stmt.h>

namespace lang::syntax {
	/**
	 * @todo move most of that data TO AST (exclude imports and submodules lists)
	 */
	struct SyntaxContainer {
		ast::AST ast;

		/**
		 * @brief list of all imported modules (without submodule)
		 */
		std::unordered_set<ast::ImportStmt*> imports_list;

		/**
		 * @brief list of submodules, `export import` nodes will be here
		 */
		std::unordered_set<ast::ImportStmt*> submodules_list;
	};
}
