#pragma once

#include <unordered_map>
#include <unordered_set>
#include <lang/ast/ast.h>
#include <lang/ast/stmt.h>
#include <lang/semantic/types/typesystem.h>

namespace lang::syntax {
    struct SyntaxContainer {
        ast::AST ast;

        /**
         * @brief DeclNode of something -> It's type
         */
        std::unordered_map<ast::BaseNode*, std::unique_ptr<AbstractType>> types_context;

        /**
         * @brief DeclNode of something -> is it exported
         */
        std::unordered_set<ast::BaseNode*> export_list;

        /**
         * @brief DeclNode of something -> is it extern
         */
        std::unordered_set<ast::BaseNode*> extern_list;

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
