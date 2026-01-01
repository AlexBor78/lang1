#include <lang/ast/visitor.h>
#include <lang/ast/expr.h>

namespace lang::ast
{
// LiteralExpr

    std::string_view LiteralExpr::get_literal() const noexcept
    {
        return literal;
    }
    const char* LiteralExpr::get_c_literal() const noexcept
    {
        return literal.c_str();
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

// IdentifierExpr

    std::string_view IdentifierExpr::get_name() const noexcept
    {
        return name;
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

    void SymbolPath::accept(ConstASTVisitor& visitor) const noexcept
    {
        visitor.visit_symbol_path(*this);
    }
    void SymbolPath::accept(ASTVisitor& visitor) noexcept
    {
        visitor.visit_symbol_path(*this);
    }

    const ExprNode* SymbolPath::get_identifier() const noexcept
    {
        return identifier.get();
    }
    ExprNode* SymbolPath::get_identifier() noexcept
    {
        return identifier.get();
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

    std::string_view FunctionExpr::get_callee() const noexcept
    {
        return callee;
    }
    const std::vector<ExprPtr>& FunctionExpr::get_args() const noexcept
    {
        return args;
    }
    std::vector<ExprPtr>& FunctionExpr::get_args() noexcept
    {
        return args;
    }

// OperatorExpr

    OperatorExpr::OperatorKind OperatorExpr::get_op() const noexcept
    {
        return op;
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

    const ExprNode* BinOpExpr::get_left() const noexcept
    {
        return left.get();
    }
    ExprNode* BinOpExpr::get_left() noexcept
    {
        return left.get();
    }
    const ExprNode* BinOpExpr::get_right() const noexcept
    {
        return right.get();
    }
    ExprNode* BinOpExpr::get_right() noexcept
    {
        return right.get();
    }

// UnaryOpExpr

    const ExprNode* UnaryOpExpr::get_operand() const noexcept
    {
        return operand.get();
    }
    ExprNode* UnaryOpExpr::get_operand() noexcept
    {
        return operand.get();
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
}
