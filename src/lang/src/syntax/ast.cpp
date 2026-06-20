#include <lang/syntax/ast/visitor.h>
#include <lang/syntax/ast/stmt.h>
#include <lang/syntax/ast/expr.h>

namespace lang::syntax::ast
{
// BlockStmt

    void BlockStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_block_stmt(*this);
    }
    void BlockStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_block_stmt(*this);
    }
		
// IfStmt

    void IfStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_if_stmt(*this);
    }
    void IfStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_if_stmt(*this);
    }

// ElseStmt

    void ElseStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_else_stmt(*this);
    }
    void ElseStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_else_stmt(*this);
    }

// ForStmt

    void ForStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_for_stmt(*this);
    }
    void ForStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_for_stmt(*this);
    }

// WhileStmt

    void WhileStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_while_stmt(*this);
    }
    void WhileStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_while_stmt(*this);
    }

// DeclVar

    void DeclVariable::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_var(*this);
    }
    void DeclVariable::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_var(*this);
    }

// DeclFunction

    void DeclFunction::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_decl_func(*this);
    }
    void DeclFunction::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_decl_func(*this);
    }

// ImportStmt

    void ImportStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_import_stmt(*this);
    }
    void ImportStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_import_stmt(*this);
    }

// ReturnStmt

    void ReturnStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_return_stmt(*this);
    }
    void ReturnStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_return_stmt(*this);
    }

// BreakStmt

    void BreakStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_break_stmt(*this);
    }
    void BreakStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_break_stmt(*this);
    }

// ContinueStmt

    void ContinueStmt::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_continue_stmt(*this);
    }
    void ContinueStmt::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_continue_stmt(*this);
    }
    
// NumberLiteral
    
    void NumberLiteral::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_number_literal(*this);
    }
    void NumberLiteral::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_number_literal(*this);
    }

// StringLiteral
    
    void StringLiteral::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_string_literal(*this);
    }
    void StringLiteral::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_string_literal(*this);
    }

// BoolLiteral
    
    void BoolLiteral::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_bool_literal(*this);
    }
    void BoolLiteral::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_bool_literal(*this);
    }

// VariableExpr

    void VariableExpr::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_variable_expr(*this);
    }
    void VariableExpr::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_variable_expr(*this);
    }

// NamespaceExpr

    void SymbolPathExpr::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_symbol_path(*this);
    }
    void SymbolPathExpr::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_symbol_path(*this);
    }

// CallExpr

    void FunctionExpr::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_call_expr(*this);
    }
    void FunctionExpr::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_call_expr(*this);
    }

// BinOpExpr

    void BinOpExpr::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_binop_expr(*this);
    }
    void BinOpExpr::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_binop_expr(*this);
    }

// PrefixUnaryOpExpr

    void PrefixUnaryOpExpr::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_prefix_unary_op_expr(*this);
    }
    void PrefixUnaryOpExpr::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_prefix_unary_op_expr(*this);
    }

// PostfixUnaryOpExpr

    void PostfixUnaryOpExpr::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_postfix_unary_op_expr(*this);
    }
    void PostfixUnaryOpExpr::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_postfix_unary_op_expr(*this);
    }

// StackAllocExpr

    void StackAllocExpr::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_stackalloc_expr(*this);
    }
    void StackAllocExpr::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_stackalloc_expr(*this);
    }
}
