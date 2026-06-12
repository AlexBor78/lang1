#pragma once

#include <vector>
#include <string_view>
#include <lang/syntax/ast.h>

namespace lang::syntax
{
    class LiteralExpr : public ExprNode
    {
    private:
        std::string literal;

    protected:
        explicit LiteralExpr(std::string_view _literal
        ,                    common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
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
        ,                      common::SourceLocation _full_range_loc = default_pos()
        ):  LiteralExpr(_literal
            ,           std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class StringLiteral : public LiteralExpr
    {
    public:
        explicit StringLiteral(std::string_view _literal
        ,                      common::SourceLocation _full_range_loc = default_pos()
        ):  LiteralExpr(_literal
            ,           std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class BoolLiteral : public LiteralExpr
    {
    public:
        explicit BoolLiteral(std::string_view _literal
        ,                    common::SourceLocation _full_range_loc = default_pos()
        ):  LiteralExpr(_literal
            ,           std::move(_full_range_loc)
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
        ,                       common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
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
        ,                     common::SourceLocation _full_range_loc = default_pos()
        ):  IdentifierExpr(_name
            ,              std::move(_full_range_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class SymbolPathExpr : public IdentifierExpr
    {
    private:
        ExprPtr identifier;

    public:
        SymbolPathExpr(std::string_view _name
        ,             ExprPtr _identifier
        ,             common::SourceLocation _name_loc = default_pos()
        ,             common::SourceLocation _full_range_loc = default_pos()
        ):  IdentifierExpr(_name
            ,              std::move(_full_range_loc)
            )
        ,   identifier(std::move(_identifier))
        ,   name_loc(_name_loc)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        const ExprNode* get_identifier() const noexcept;
        ExprNode* get_identifier() noexcept;

        common::SourceLocation name_loc;
    };

    class FunctionExpr : public ExprNode
    {
    private:
        std::string callee;
        std::vector<ExprPtr> args;

    public:
        FunctionExpr(std::string_view _callee
        ,        std::vector<ExprPtr> _args
        ,        common::SourceLocation _name_loc = default_pos()
        ,        common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   callee(_callee)
        ,   args(std::move(_args))
        ,   name_loc(_name_loc)
        {}

        FunctionExpr(std::string_view _callee
        ,        std::vector<ExprPtr> _args
        ,        common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   callee(_callee)
        ,   args(std::move(_args))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        std::string_view get_callee() const noexcept;
        const std::vector<ExprPtr>& get_args() const noexcept;
        std::vector<ExprPtr>& get_args() noexcept;
      
        common::SourceLocation name_loc;
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
        ,                     common::SourceLocation _op_loc = default_pos()
        ,                     common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   op(_op)
        ,   op_loc(_op_loc)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;

        OperatorKind get_op() const noexcept;
        common::SourceLocation op_loc;
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
        ,         common::SourceLocation _op_loc = default_pos()
        ,         common::SourceLocation _full_range_loc = default_pos()
        ):  OperatorExpr(_op
            ,           std::move(_full_range_loc)
            ,           std::move(_op_loc)
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
        ,           common::SourceLocation _op_loc = default_pos()
        ,           common::SourceLocation _full_range_loc = default_pos()
        ):  OperatorExpr(_op
            ,           std::move(_full_range_loc)
            ,           std::move(_op_loc)
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
        ,                 common::SourceLocation _op_loc = default_pos()
        ,                 common::SourceLocation _full_range_loc = default_pos()
        ):  UnaryOpExpr(_op
            ,           std::move(_operand)
            ,           std::move(_full_range_loc)
            ,           std::move(_op_loc)
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
        ,                  common::SourceLocation _op_loc = default_pos()
        ,                  common::SourceLocation _full_range_loc = default_pos()
        ):  UnaryOpExpr(_op
            ,           std::move(_operand)
            ,           std::move(_full_range_loc)
            ,           std::move(_op_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class StackAllocExpr : public ExprNode {
        std::vector<size_t> dimensions;

    public:

        explicit StackAllocExpr(std::vector<size_t> _dimensions
        ,                       std::vector<common::SourceLocation> _locs = std::vector<common::SourceLocation>()
        ,                       common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(_full_range_loc)
        ,   dimensions(std::move(_dimensions))
        ,   locs(std::move(_locs))
        {}

        const std::vector<size_t> get_dimensions() const noexcept;
        
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        std::vector<common::SourceLocation> locs;
    };
}
