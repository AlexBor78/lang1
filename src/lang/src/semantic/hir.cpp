#include <lang/semantic/hir/hir.h>
#include <lang/semantic/hir/visitor.h>

namespace lang::semantic::hir {

// BlockStmt

	void BlockStmt::accept(ConstVisitor& visitor) const noexcept {
		visitor.visit_block_stmt(*this);
	}
	void BlockStmt::accept(Visitor& visitor) noexcept {
		visitor.visit_block_stmt(*this);
	}

// IfStmt

    void IfStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_if_stmt(*this);
    }
    void IfStmt::accept(Visitor& visitor) noexcept
    {
        visitor.visit_if_stmt(*this);
    }

// ElseStmt

    void ElseStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_else_stmt(*this);
    }
    void ElseStmt::accept(Visitor& visitor) noexcept
    {
        visitor.visit_else_stmt(*this);
    }

// WhileStmt

    void WhileStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_while_stmt(*this);
    }
    void WhileStmt::accept(Visitor& visitor) noexcept
    {
        visitor.visit_while_stmt(*this);
    }

// DeclVar

    void DeclVar::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_decl_var(*this);
    }
    void DeclVar::accept(Visitor& visitor) noexcept
    {
        visitor.visit_decl_var(*this);
    }

// DeclFunction

    void DeclFunc::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_decl_func(*this);
    }
    void DeclFunc::accept(Visitor& visitor) noexcept
    {
        visitor.visit_decl_func(*this);
    }

// ImportStmt

    void ImportStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_import_stmt(*this);
    }
    void ImportStmt::accept(Visitor& visitor) noexcept
    {
        visitor.visit_import_stmt(*this);
    }

// ReturnStmt

    void ReturnStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_return_stmt(*this);
    }
    void ReturnStmt::accept(Visitor& visitor) noexcept
    {
        visitor.visit_return_stmt(*this);
    }

// BreakStmt

    void BreakStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_break_stmt(*this);
    }
    void BreakStmt::accept(Visitor& visitor) noexcept
    {
        visitor.visit_break_stmt(*this);
    }

// ContinueStmt

    void ContinueStmt::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_continue_stmt(*this);
    }
    void ContinueStmt::accept(Visitor& visitor) noexcept
    {
        visitor.visit_continue_stmt(*this);
    }
    
// Literal
    
    void IntegerLiteral::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_integer_literal(*this);
    }
    void IntegerLiteral::accept(Visitor& visitor) noexcept
    {
        visitor.visit_integer_literal(*this);
    }

// Literal
    
    void FloatLiteral::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_float_literal(*this);
    }
    void FloatLiteral::accept(Visitor& visitor) noexcept
    {
        visitor.visit_float_literal(*this);
    }

// StringLiteral
    
    void StringLiteral::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_string_literal(*this);
    }
    void StringLiteral::accept(Visitor& visitor) noexcept
    {
        visitor.visit_string_literal(*this);
    }

// BoolLiteral
    
    void BoolLiteral::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_bool_literal(*this);
    }
    void BoolLiteral::accept(Visitor& visitor) noexcept
    {
        visitor.visit_bool_literal(*this);
    }

// VariableExpr

    void VariableExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_variable_expr(*this);
    }
    void VariableExpr::accept(Visitor& visitor) noexcept
    {
        visitor.visit_variable_expr(*this);
    }


// CallExpr

    void CallExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_call_expr(*this);
    }
    void CallExpr::accept(Visitor& visitor) noexcept
    {
        visitor.visit_call_expr(*this);
    }

// BinOpExpr

    void BinOpExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_binop_expr(*this);
    }
    void BinOpExpr::accept(Visitor& visitor) noexcept
    {
        visitor.visit_binop_expr(*this);
    }

// PrefixOpExpr

    void PrefixOpExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_prefix_unary_op_expr(*this);
    }
    void PrefixOpExpr::accept(Visitor& visitor) noexcept
    {
        visitor.visit_prefix_unary_op_expr(*this);
    }

// PostfixOpExpr

    void PostfixOpExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_postfix_unary_op_expr(*this);
    }
    void PostfixOpExpr::accept(Visitor& visitor) noexcept
    {
        visitor.visit_postfix_unary_op_expr(*this);
    }

// StackAllocExpr

    void StackAllocExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_stackalloc_expr(*this);
    }
    void StackAllocExpr::accept(Visitor& visitor) noexcept
    {
        visitor.visit_stackalloc_expr(*this);
    }
}
