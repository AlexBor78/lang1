#pragma once

#include <lang/ast/ast.h>
#include <lang/ast/stmt.h>
#include <lang/ast/expr.h>

namespace lang::ast
{
    class ConstASTVisitor
    {
    protected:
        ConstASTVisitor() = default;
        
    public:
        virtual void visit_block_stmt(const BlockStmt&) noexcept = 0;
        virtual void visit_if_stmt(const IfStmt&) noexcept = 0;
        virtual void visit_else_stmt(const ElseStmt&) noexcept = 0;
        virtual void visit_for_stmt(const ForStmt&) noexcept = 0;
        virtual void visit_while_stmt(const WhileStmt&) noexcept = 0;
        virtual void visit_decl_var(const DeclVariable&) noexcept = 0;
        virtual void visit_decl_func(const DeclFunction&) noexcept = 0;
        virtual void visit_decl_module(const DeclModule&) noexcept = 0;
        virtual void visit_decl_namespace(const DeclNamespace&) noexcept = 0;
        virtual void visit_import_stmt(const ImportStmt&) noexcept = 0;
        virtual void visit_return_stmt(const ReturnStmt&) noexcept = 0;
        virtual void visit_break_stmt(const BreakStmt&) noexcept = 0;
        virtual void visit_continue_stmt(const ContinueStmt&) noexcept = 0;
        virtual void visit_number_literal(const NumberLiteral&) noexcept = 0;
        virtual void visit_string_literal(const StringLiteral&) noexcept = 0;
        virtual void visit_bool_literal(const BoolLiteral&) noexcept = 0;
        virtual void visit_variable_expr(const VariableExpr&) noexcept = 0;
        virtual void visit_namespace_expr(const NamespaceExpr&) noexcept = 0;
        virtual void visit_call_expr(const FunctionExpr&) noexcept = 0;
        virtual void visit_binop_expr(const BinOpExpr&) noexcept = 0;
        virtual void visit_prefix_unary_op_expr(const PrefixUnaryOpExpr&) noexcept = 0;
        virtual void visit_postfix_unary_op_expr(const PostfixUnaryOpExpr&) noexcept = 0;
        // virtual void visit_(const &) noexcept = 0;
    };

    class ASTVisitor
    {
    protected:
        ASTVisitor() = default;
        
    public:
        virtual void visit_block_stmt(BlockStmt&) noexcept = 0;
        virtual void visit_if_stmt(IfStmt&) noexcept = 0;
        virtual void visit_else_stmt(ElseStmt&) noexcept = 0;
        virtual void visit_for_stmt(ForStmt&) noexcept = 0;
        virtual void visit_while_stmt(WhileStmt&) noexcept = 0;
        virtual void visit_decl_var(DeclVariable&) noexcept = 0;
        virtual void visit_decl_func(DeclFunction&) noexcept = 0;
        virtual void visit_decl_module(DeclModule&) noexcept = 0;
        virtual void visit_decl_namespace(DeclNamespace&) noexcept = 0;
        virtual void visit_import_stmt(ImportStmt&) noexcept = 0;
        virtual void visit_return_stmt(ReturnStmt&) noexcept = 0;
        virtual void visit_break_stmt(BreakStmt&) noexcept = 0;
        virtual void visit_continue_stmt(ContinueStmt&) noexcept = 0;
        virtual void visit_number_literal(NumberLiteral&) noexcept = 0;
        virtual void visit_string_literal(StringLiteral&) noexcept = 0;
        virtual void visit_bool_literal(BoolLiteral&) noexcept = 0;
        virtual void visit_variable_expr(VariableExpr&) noexcept = 0;
        virtual void visit_namespace_expr(NamespaceExpr&) noexcept = 0;
        virtual void visit_call_expr(FunctionExpr&) noexcept = 0;
        virtual void visit_binop_expr(BinOpExpr&) noexcept = 0;
        virtual void visit_prefix_unary_op_expr(PrefixUnaryOpExpr&) noexcept = 0;
        virtual void visit_postfix_unary_op_expr(PostfixUnaryOpExpr&) noexcept = 0;
        // virtual void visit_(&) noexcept = 0;
    };
}
