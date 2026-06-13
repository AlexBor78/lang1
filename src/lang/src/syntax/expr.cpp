#include <lang/syntax/visitor.h>
#include <lang/syntax/expr.h>

namespace lang::syntax
{
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
