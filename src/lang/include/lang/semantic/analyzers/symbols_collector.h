#pragma once

#include <lang/ast/visitor.h>
#include <lang/semantic/types/semantic_types.h>

namespace lang::semantic
{
    class SymbolsCollector : public ast::ASTVisitor
    {
    public: // api
        SymbolsCollector(SemanticState* _state):
            state(_state)
        {}

        void collect();
        void collect(SemanticState*);

    private: // var
        SemanticState* state{nullptr};

    public: // valuable nodes
        virtual void visit_block_stmt(ast::BlockStmt&) noexcept override;
        virtual void visit_decl_var(ast::DeclVariable&) noexcept override;
        virtual void visit_decl_func(ast::DeclFunction&) noexcept override;

    public: // useless nodes
        virtual void visit_if_stmt(ast::IfStmt&) noexcept override {}
        virtual void visit_else_stmt(ast::ElseStmt&) noexcept override {}
        virtual void visit_for_stmt(ast::ForStmt&) noexcept override {}
        virtual void visit_while_stmt(ast::WhileStmt&) noexcept override {}
        virtual void visit_return_stmt(ast::ReturnStmt&) noexcept override {}
        virtual void visit_import_stmt(ast::ImportStmt&) noexcept override {}
        virtual void visit_break_stmt(ast::BreakStmt&) noexcept override {}
        virtual void visit_continue_stmt(ast::ContinueStmt&) noexcept override {}
        virtual void visit_number_literal(ast::NumberLiteral&) noexcept override {}
        virtual void visit_string_literal(ast::StringLiteral&) noexcept override {}
        virtual void visit_bool_literal(ast::BoolLiteral&) noexcept override {}
        virtual void visit_variable_expr(ast::VariableExpr&) noexcept override {}
        virtual void visit_symbol_path(ast::SymbolPath&) noexcept override {}
        virtual void visit_call_expr(ast::FunctionExpr&) noexcept override {}
        virtual void visit_binop_expr(ast::BinOpExpr&) noexcept override {}
        virtual void visit_prefix_unary_op_expr(ast::PrefixUnaryOpExpr&) noexcept override {}
        virtual void visit_postfix_unary_op_expr(ast::PostfixUnaryOpExpr&) noexcept override {}
    };
}
