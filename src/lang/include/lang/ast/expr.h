#pragma once
#include <lang/ast/ast.h>
#include <string_view>
#include <vector>

namespace lang::ast
{
    class LiteralExpr : public ExprNode
    {
    private:
        std::string literal;

    protected:
        explicit LiteralExpr(std::string_view _literal
        ,                    common::SourceLocation _pos = default_pos()
        ):  ExprNode(std::move(_pos))
        ,   literal(_literal)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        std::string_view get_literal() const noexcept;
        const char* get_c_literal() const noexcept;
    };

    class NumberLiteral : public LiteralExpr
    {
    public:
        explicit NumberLiteral(std::string_view _literal
        ,                      common::SourceLocation _pos = default_pos()
        ):  LiteralExpr(_literal
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class StringLiteral : public LiteralExpr
    {
    public:
        explicit StringLiteral(std::string_view _literal
        ,                      common::SourceLocation _pos = default_pos()
        ):  LiteralExpr(_literal
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class BoolLiteral : public LiteralExpr
    {
    public:
        explicit BoolLiteral(std::string_view _literal
        ,                    common::SourceLocation _pos = default_pos()
        ):  LiteralExpr(_literal
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class IdentifierExpr : public ExprNode
    {
    private:
        std::string name;

    public:
        explicit IdentifierExpr(std::string_view _name
        ,                       common::SourceLocation _pos = default_pos()
        ):  ExprNode(std::move(_pos))
        ,   name(_name)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        std::string_view get_name() const noexcept;
    };

    class VariableExpr : public IdentifierExpr
    {
    public:
        explicit VariableExpr(std::string_view _name
        ,                     common::SourceLocation _pos = default_pos()
        ):  IdentifierExpr(_name
            ,              std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class SymbolPath : public IdentifierExpr
    {
    private:
        ExprPtr identifier;
    public:
        SymbolPath(std::string_view _name
        ,             ExprPtr _identifier
        ,             common::SourceLocation _pos = default_pos()
        ):  IdentifierExpr(_name
            ,              std::move(_pos)
            )
        ,   identifier(std::move(_identifier))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_identifier() const noexcept;
        ExprNode* get_identifier() noexcept;
    };

    class FunctionExpr : public ExprNode
    {
    private:
        std::string callee;
        std::vector<ExprPtr> args;

    public:
        FunctionExpr(std::string_view _callee
        ,        std::vector<ExprPtr>& _args
        ,        common::SourceLocation _pos = default_pos()
        ):  ExprNode(std::move(_pos))
        ,   args(std::move(_args))
        ,   callee(_callee)
        {}

        FunctionExpr(std::string_view _callee
        ,        std::vector<ExprPtr> _args
        ,        common::SourceLocation _pos = default_pos()
        ):  ExprNode(std::move(_pos))
        ,   args(std::move(_args))
        ,   callee(_callee)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        std::string_view get_callee() const noexcept;
        const std::vector<ExprPtr>& get_args() const noexcept;
        std::vector<ExprPtr>& get_args() noexcept;
    };

    class OperatorExpr : public ExprNode
    {
    public: enum class OperatorKind
    {
        // Assignment
        ASSIGN,     // =
        ADD_ASSIGN,       // +=
        SUB_ASSIGN,       // -=
        MUL_ASSIGN,       // *=
        DIV_ASSIGN,       // /=
        MOD_ASSIGN,       // %=
        AND_ASSIGN,       // &=
        OR_ASSIGN,        // |=
        XOR_ASSIGN,       // ^=
        SHL_ASSIGN,       // <<=
        SHR_ASSIGN,        // >>=

        // Arithmetic
        PLUS,       // +
        MINUS,      // -
        STAR,       // *
        SLASH,      // /
        PERCENT,    // %

        // Unary / Address / Dereference
        AMPERSAND,  // &
        BANG,       // !
        TILDE,      // ~

        // Increment / Decrement
        INCREMENT,  // ++
        DECREMENT,  // --

        // Comparison
        EQ,         // ==
        NEQ,        // !=
        LT,         // <
        LE,         // <=
        GT,         // >
        GE,         // >=

        // Logical
        AND,        // &&
        OR,         // ||

        // Bitwise
        BIT_OR,         // |
        // BIT_AND = AMPERSAND, // &
        BIT_XOR,        // ^
        SHIFT_LEFT,     // <<
        SHIFT_RIGHT     // >>
    };

    private:
        OperatorKind op;
        
    protected:
        explicit OperatorExpr(OperatorKind _op
        ,                    common::SourceLocation _pos = default_pos()
        ):  ExprNode(std::move(_pos))
        ,   op(_op)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        OperatorKind get_op() const noexcept;
    };

    class BinOpExpr : public OperatorExpr
    {
    private:
        ExprPtr left;
        ExprPtr right;
    public:
        BinOpExpr(OperatorKind _op
        ,         ExprPtr _left
        ,         ExprPtr _right
        ,         common::SourceLocation _pos = default_pos()
        ):  OperatorExpr(_op
            ,           std::move(_pos)
            )
        ,   left(std::move(_left))
        ,   right(std::move(_right))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_left() const noexcept;
        ExprNode* get_left() noexcept;

        const ExprNode* get_right() const noexcept;
        ExprNode* get_right() noexcept;

    };

    class UnaryOpExpr : public OperatorExpr
    {
    private:
        ExprPtr operand;

    protected:
        UnaryOpExpr(OperatorKind _op
        ,           ExprPtr _operand
        ,           common::SourceLocation _pos = default_pos()
        ):  OperatorExpr(_op
            ,           std::move(_pos)
            )
        ,   operand(std::move(_operand))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        const ExprNode* get_operand() const noexcept;
        ExprNode* get_operand() noexcept;
    };

    class PrefixUnaryOpExpr : public UnaryOpExpr
    {
    public:
        PrefixUnaryOpExpr(OperatorKind _op
        ,                 ExprPtr _operand
        ,                 common::SourceLocation _pos = default_pos()
        ):  UnaryOpExpr(_op
            ,           std::move(_operand)
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class PostfixUnaryOpExpr : public UnaryOpExpr
    {
    public:
        PostfixUnaryOpExpr(OperatorKind _op
        ,                  ExprPtr _operand
        ,                  common::SourceLocation _pos = default_pos()
        ):  UnaryOpExpr(_op
            ,           std::move(_operand)
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };
}
