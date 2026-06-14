#pragma once

#include <vector>
#include <string_view>
#include <lang/syntax/ast/ast.h>

namespace lang::syntax
{
    class LiteralExpr : public ExprNode
    {
    protected:
        explicit LiteralExpr(
					std::string_view _literal
        , common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   literal(_literal)
        {}

    public:
        std::string_view literal;
    };

    class NumberLiteral : public LiteralExpr
    {
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
				
        explicit NumberLiteral(
					std::string_view _literal
        , common::SourceLocation _full_range_loc = default_pos()
        ):  LiteralExpr(
							_literal
            , std::move(_full_range_loc)
            )
        {}
    };

    class StringLiteral : public LiteralExpr
    {
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        explicit StringLiteral(
					std::string_view _literal
        , common::SourceLocation _full_range_loc = default_pos()
        ):  LiteralExpr(
							_literal
            , std::move(_full_range_loc)
            )
        {}
    };

    class BoolLiteral : public LiteralExpr
    {
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        explicit BoolLiteral(
						std::string_view _literal
        ,   common::SourceLocation _full_range_loc = default_pos()
        ):  LiteralExpr(
							_literal
            , std::move(_full_range_loc)
            )
        {}
    };

    class IdentifierExpr : public ExprNode
    {
    public:
        std::string name;
        explicit IdentifierExpr(
					std::string_view _name
        , common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   name(_name)
        {}
    };

    class VariableExpr : public IdentifierExpr
    {
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;

        explicit VariableExpr(
					std::string_view _name
        , common::SourceLocation _full_range_loc = default_pos()
        ):  IdentifierExpr(
							_name
            , std::move(_full_range_loc)
            )
        {}
    };

    class SymbolPathExpr : public IdentifierExpr
    {
    public:
        ExprPtr identifier;
        common::SourceLocation name_loc;

        SymbolPathExpr(
					std::string_view _name
        , ExprPtr _identifier
        , common::SourceLocation _name_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  IdentifierExpr(
							_name
            , std::move(_full_range_loc)
            )
        ,   identifier(std::move(_identifier))
        ,   name_loc(_name_loc)
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class FunctionExpr : public ExprNode
    {
    public:
        std::string_view callee;
        std::vector<ExprPtr> args;
        common::SourceLocation name_loc;

        FunctionExpr(
					std::string_view _callee
        , std::vector<ExprPtr> _args
        , common::SourceLocation _name_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   callee(_callee)
        ,   args(std::move(_args))
        ,   name_loc(_name_loc)
        {}

        FunctionExpr(
					std::string_view _callee
        , std::vector<ExprPtr> _args
        , common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   callee(_callee)
        ,   args(std::move(_args))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
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
    protected:
        explicit OperatorExpr(
					OperatorKind _op
        , common::SourceLocation _op_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(std::move(_full_range_loc))
        ,   op(_op)
        ,   op_loc(_op_loc)
        {}

    public:
        OperatorKind op;
        common::SourceLocation op_loc;
    };

    class BinOpExpr : public OperatorExpr
    {
    public:
        ExprPtr left;
        ExprPtr right;

        BinOpExpr(
					OperatorKind _op
        , ExprPtr _left
        , ExprPtr _right
        , common::SourceLocation _op_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  OperatorExpr(
							_op
            , std::move(_full_range_loc)
            , std::move(_op_loc)
            )
        ,   left(std::move(_left))
        ,   right(std::move(_right))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class UnaryOpExpr : public OperatorExpr
    {
    protected:
        UnaryOpExpr(
					OperatorKind _op
        , ExprPtr _operand
        , common::SourceLocation _op_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  OperatorExpr(
							_op
            , std::move(_full_range_loc)
            , std::move(_op_loc)
            )
        ,   operand(std::move(_operand))
        {}
		public:
        ExprPtr operand;
    };

    class PrefixUnaryOpExpr : public UnaryOpExpr
    {
    public:
        PrefixUnaryOpExpr(
					OperatorKind _op
        , ExprPtr _operand
        , common::SourceLocation _op_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  UnaryOpExpr(
							_op
            , std::move(_operand)
            , std::move(_full_range_loc)
            , std::move(_op_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

    class PostfixUnaryOpExpr : public UnaryOpExpr
    {
    public:
        PostfixUnaryOpExpr(
					OperatorKind _op
        , ExprPtr _operand
        , common::SourceLocation _op_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  UnaryOpExpr(
							_op
            , std::move(_operand)
            , std::move(_full_range_loc)
            , std::move(_op_loc)
            )
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };

		// TODO: use std::pmr::vector so memory don't leak
    class StackAllocExpr : public ExprNode {
    public:
        std::vector<size_t> dimensions;
        std::vector<common::SourceLocation> locs;

        explicit StackAllocExpr(
					std::vector<size_t> _dimensions
        , std::vector<common::SourceLocation> _locs = std::vector<common::SourceLocation>()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  ExprNode(_full_range_loc)
        ,   dimensions(std::move(_dimensions))
        ,   locs(std::move(_locs))
        {}

        virtual void accept(ConstASTVisitor&) const noexcept override;
        virtual void accept(ASTVisitor&) noexcept override;
    };
}
