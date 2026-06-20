#pragma once

#include <lang/semantic/hir/hir.h>

namespace lang::semantic::hir {

	class Visitor {
	protected:
		Visitor() = default;

  public:
    virtual void visit_block_stmt(BlockStmt&) = 0;
    virtual void visit_if_stmt(IfStmt&) = 0;
    virtual void visit_else_stmt(ElseStmt&) = 0;
    virtual void visit_while_stmt(WhileStmt&) = 0;
    virtual void visit_decl_var(DeclVar&) = 0;
    virtual void visit_decl_func(DeclFunc&) = 0;
    virtual void visit_import_stmt(ImportStmt&) = 0;
    virtual void visit_return_stmt(ReturnStmt&) = 0;
    virtual void visit_break_stmt(BreakStmt&) = 0;
    virtual void visit_continue_stmt(ContinueStmt&) = 0;
    virtual void visit_integer_literal(IntegerLiteral&) = 0;
    virtual void visit_float_literal(FloatLiteral&) = 0;
    virtual void visit_string_literal(StringLiteral&) = 0;
    virtual void visit_bool_literal(BoolLiteral&) = 0;
    virtual void visit_variable_expr(VariableExpr&) = 0;
    virtual void visit_stackalloc_expr(StackAllocExpr&) = 0;
    virtual void visit_call_expr(CallExpr&) = 0;
    virtual void visit_binop_expr(BinOpExpr&) = 0;
    virtual void visit_prefix_unary_op_expr(PrefixOpExpr&) = 0;
    virtual void visit_postfix_unary_op_expr(PostfixOpExpr&) = 0;
      // virtual void visit_(&) = 0;
	};

	class ConstVisitor {
	protected:
		ConstVisitor() = default;

  public:
    virtual void visit_block_stmt(const BlockStmt&) = 0;
    virtual void visit_if_stmt(const IfStmt&) = 0;
    virtual void visit_else_stmt(const ElseStmt&) = 0;
    virtual void visit_while_stmt(const WhileStmt&) = 0;
    virtual void visit_decl_var(const DeclVar&) = 0;
    virtual void visit_decl_func(const DeclFunc&) = 0;
    virtual void visit_import_stmt(const ImportStmt&) = 0;
    virtual void visit_return_stmt(const ReturnStmt&) = 0;
    virtual void visit_break_stmt(const BreakStmt&) = 0;
    virtual void visit_continue_stmt(const ContinueStmt&) = 0;
    virtual void visit_integer_literal(const IntegerLiteral&) = 0;
    virtual void visit_float_literal(const FloatLiteral&) = 0;
    virtual void visit_string_literal(const StringLiteral&) = 0;
    virtual void visit_bool_literal(const BoolLiteral&) = 0;
    virtual void visit_variable_expr(const VariableExpr&) = 0;
    virtual void visit_stackalloc_expr(const StackAllocExpr&) = 0;
    virtual void visit_call_expr(const CallExpr&) = 0;
    virtual void visit_binop_expr(const BinOpExpr&) = 0;
    virtual void visit_prefix_unary_op_expr(const PrefixOpExpr&) = 0;
    virtual void visit_postfix_unary_op_expr(const PostfixOpExpr&) = 0;
    // virtual void visit_(const &) = 0;
	};

}
