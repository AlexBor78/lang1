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
        virtual void visit_block_stmt(const BlockStmt&) = 0;
        virtual void visit_if_stmt(const IfStmt&) = 0;
        virtual void visit_else_stmt(const ElseStmt&) = 0;
        virtual void visit_for_stmt(const ForStmt&) = 0;
        virtual void visit_while_stmt(const WhileStmt&) = 0;
        virtual void visit_decl_var(const DeclVariable&) = 0;
        virtual void visit_decl_func(const DeclFunction&) = 0;
        virtual void visit_import_stmt(const ImportStmt&) = 0;
        virtual void visit_return_stmt(const ReturnStmt&) = 0;
        virtual void visit_break_stmt(const BreakStmt&) = 0;
        virtual void visit_continue_stmt(const ContinueStmt&) = 0;
        virtual void visit_number_literal(const NumberLiteral&) = 0;
        virtual void visit_string_literal(const StringLiteral&) = 0;
        virtual void visit_bool_literal(const BoolLiteral&) = 0;
        virtual void visit_variable_expr(const VariableExpr&) = 0;
        virtual void visit_stackalloc_expr(const StackAllocExpr&) = 0;
        virtual void visit_symbol_path(const SymbolPathExpr&) = 0;
        virtual void visit_call_expr(const FunctionExpr&) = 0;
        virtual void visit_binop_expr(const BinOpExpr&) = 0;
        virtual void visit_prefix_unary_op_expr(const PrefixUnaryOpExpr&) = 0;
        virtual void visit_postfix_unary_op_expr(const PostfixUnaryOpExpr&) = 0;
        // virtual void visit_(const &) = 0;
    };

    class ASTVisitor
    {
    protected:
        ASTVisitor() = default;
        
    public:
        virtual void visit_block_stmt(BlockStmt&) = 0;
        virtual void visit_if_stmt(IfStmt&) = 0;
        virtual void visit_else_stmt(ElseStmt&) = 0;
        virtual void visit_for_stmt(ForStmt&) = 0;
        virtual void visit_while_stmt(WhileStmt&) = 0;
        virtual void visit_decl_var(DeclVariable&) = 0;
        virtual void visit_decl_func(DeclFunction&) = 0;
        virtual void visit_import_stmt(ImportStmt&) = 0;
        virtual void visit_return_stmt(ReturnStmt&) = 0;
        virtual void visit_break_stmt(BreakStmt&) = 0;
        virtual void visit_continue_stmt(ContinueStmt&) = 0;
        virtual void visit_number_literal(NumberLiteral&) = 0;
        virtual void visit_string_literal(StringLiteral&) = 0;
        virtual void visit_bool_literal(BoolLiteral&) = 0;
        virtual void visit_variable_expr(VariableExpr&) = 0;
        virtual void visit_stackalloc_expr(StackAllocExpr&) = 0;
        virtual void visit_symbol_path(SymbolPathExpr&) = 0;
        virtual void visit_call_expr(FunctionExpr&) = 0;
        virtual void visit_binop_expr(BinOpExpr&) = 0;
        virtual void visit_prefix_unary_op_expr(PrefixUnaryOpExpr&) = 0;
        virtual void visit_postfix_unary_op_expr(PostfixUnaryOpExpr&) = 0;
        // virtual void visit_(&) = 0;
    };
}
