#include <ast/visitor.h>

#include <ast/expr.h>


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
    
    void NumberLiteral::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_number_literal(*this);
    }
    void NumberLiteral::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_number_literal(*this);
    }

// StringLiteral
    
    void StringLiteral::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_string_literal(*this);
    }
    void StringLiteral::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_string_literal(*this);
    }

// BoolLiteral
    
    void BoolLiteral::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_bool_literal(*this);
    }
    void BoolLiteral::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_bool_literal(*this);
    }

// VariableExpr

    void VariableExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_variable_expr(*this);
    }
    void VariableExpr::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_variable_expr(*this);
    }

    std::string_view VariableExpr::get_name() const noexcept
    {
        return name;
    }

// CallExpr

    void CallExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_call_expr(*this);
    }
    void CallExpr::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_call_expr(*this);
    }

    std::string_view CallExpr::get_callname() const noexcept
    {
        return callee;
    }
    const std::vector<ExprPtr>& CallExpr::get_args() const noexcept
    {
        return args;
    }
    std::vector<ExprPtr>& CallExpr::get_args() noexcept
    {
        return args;
    }

// OperatorExpr

    OperatorExpr::OperatorKind OperatorExpr::get_op() const noexcept
    {
        return op;
    }

// BinOpExpr

    void BinOpExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_binop_expr(*this);
    }
    void BinOpExpr::accept(NodeVisitor& visitor) noexcept
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

    void PrefixUnaryOpExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_prefix_unary_op_expr(*this);
    }
    void PrefixUnaryOpExpr::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_prefix_unary_op_expr(*this);
    }

// PostfixUnaryOpExpr

    void PostfixUnaryOpExpr::accept(ConstVisitor& visitor) const noexcept
    {
        visitor.visit_postfix_unary_op_expr(*this);
    }
    void PostfixUnaryOpExpr::accept(NodeVisitor& visitor) noexcept
    {
        visitor.visit_postfix_unary_op_expr(*this);
    }
}
