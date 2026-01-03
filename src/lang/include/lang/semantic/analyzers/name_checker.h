#pragma once

#include "lang/ast/expr.h"
#include "lang/semantic/types/semantic_types.h"
#include <lang/ast/visitor.h>

namespace lang::semantic
{
    class NameChecker : public ast::ConstASTVisitor
    {
    public: // api
        NameChecker(SemanticState* _state):
            state(_state)
        {}

        void analyze();
        void analyze(SemanticState*);

    private: // var
        SemanticState* state{nullptr};

    public: // pattern stuff
        virtual void visit_block_stmt(const ast::BlockStmt&) noexcept override;
        virtual void visit_if_stmt(const ast::IfStmt&) noexcept override;
        virtual void visit_else_stmt(const ast::ElseStmt&) noexcept override;
        virtual void visit_for_stmt(const ast::ForStmt&) noexcept override;
        virtual void visit_while_stmt(const ast::WhileStmt&) noexcept override;
        virtual void visit_decl_var(const ast::DeclVariable&) noexcept override;
        virtual void visit_decl_func(const ast::DeclFunction&) noexcept override;
        virtual void visit_return_stmt(const ast::ReturnStmt&) noexcept override;
        virtual void visit_import_stmt(const ast::ImportStmt&) noexcept override {}
        virtual void visit_break_stmt(const ast::BreakStmt&) noexcept override {}
        virtual void visit_continue_stmt(const ast::ContinueStmt&) noexcept override {}
        virtual void visit_number_literal(const ast::NumberLiteral&) noexcept override {}
        virtual void visit_string_literal(const ast::StringLiteral&) noexcept override {}
        virtual void visit_bool_literal(const ast::BoolLiteral&) noexcept override {}
        virtual void visit_variable_expr(const ast::VariableExpr&) noexcept override;
        virtual void visit_symbol_path(const ast::SymbolPathExpr&) noexcept override;
        virtual void visit_call_expr(const ast::FunctionExpr&) noexcept override;
        virtual void visit_binop_expr(const ast::BinOpExpr&) noexcept override;
        virtual void visit_prefix_unary_op_expr(const ast::PrefixUnaryOpExpr&) noexcept override;
        virtual void visit_postfix_unary_op_expr(const ast::PostfixUnaryOpExpr&) noexcept override;
    };
}
