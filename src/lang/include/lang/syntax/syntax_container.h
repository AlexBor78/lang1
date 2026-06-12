#pragma once

#include <unordered_map>
#include <unordered_set>
#include <lang/syntax/ast.h>
#include <lang/syntax/stmt.h>
#include <lang/semantic/types/typesystem.h>

namespace lang::syntax {
	/**
	 * @todo move most of that data TO AST (exclude imports and submodules lists)
	 */
	struct SyntaxContainer {
		AST ast;

		/**
		 * @brief DeclNode of something -> It's type
		 */
		std::unordered_map<BaseNode*, std::unique_ptr<AbstractType>> types_context;

		/**
		 * @brief DeclNode of something -> is it exported
		 */
		std::unordered_set<BaseNode*> export_list;

		/**
		 * @brief DeclNode of something -> is it extern
		 */
		std::unordered_set<BaseNode*> extern_list;

		/**
		 * @brief list of all imported modules (without submodule)
		 */
		std::unordered_set<ImportStmt*> imports_list;

		/**
		 * @brief list of submodules, `export import` nodes will be here
		 */
		std::unordered_set<ImportStmt*> submodules_list;
	};
}
