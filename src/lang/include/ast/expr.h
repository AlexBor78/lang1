#pragma once
#include <ast/ast.h>
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
        ,                    const Type* _type = nullptr
        ,                    Position _pos = default_pos()
        ):  ExprNode(_type
            ,        std::move(_pos)
            )
        ,   literal(_literal)
        {}

    public:
        virtual void accept(ConstVisitor&) const noexcept override = 0;
        virtual void accept(NodeVisitor&) noexcept override = 0;

        std::string_view get_literal() const noexcept;
        const char* get_c_literal() const noexcept;
    };

    class NumberLiteral : public LiteralExpr
    {
    public:
        explicit NumberLiteral(std::string_view _literal
        ,                      const Type* _type = nullptr
        ,                      Position _pos = default_pos()
        ):  LiteralExpr(_literal
            ,           _type
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
    };

    class StringLiteral : public LiteralExpr
    {
    public:
        explicit StringLiteral(std::string_view _literal
        ,                      const Type* _type = nullptr
        ,                      Position _pos = default_pos()
        ):  LiteralExpr(_literal
            ,           _type
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
    };

    class BoolLiteral : public LiteralExpr
    {
    public:
        explicit BoolLiteral(std::string_view _literal
        ,                    const Type* _type = nullptr
        ,                    Position _pos = default_pos()
        ):  LiteralExpr(_literal
            ,           _type
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
    };

    class VariableExpr : public ExprNode
    {
    private:
        std::string name;

    public:
        explicit VariableExpr(std::string_view _name
        ,                     const Type* _type = nullptr
        ,                     Position _pos = default_pos()
        ):  ExprNode(std::move(_pos))
        ,   name(_name)
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

        std::string_view get_name() const noexcept;
    };

    class CallExpr : public ExprNode
    {
    private:
        std::string callee;
        std::vector<ExprPtr> args;

    public:
        explicit CallExpr(std::string_view _callee
        ,                 std::vector<ExprPtr>& _args
        ,                 const Type* _type = nullptr
        ,                 Position _pos = default_pos()
        ):  ExprNode(_type, std::move(_pos))
        ,   args(std::move(_args))
        ,   callee(_callee)
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

        std::string_view get_callname() const noexcept;
        const std::vector<ExprPtr>& get_args() const noexcept;
        std::vector<ExprPtr>& get_args() noexcept;
    };

    class OperatorExpr : public ExprNode
    {
    public: enum class OperatorKind
    {
        ASSIGN,     // =
        PLUS,       // +
        MINUS,      // -
        STAR,       // *
        SLASH,      // /
        PERCENT,    // %
        EQ,         // ==
        NEQ,        // !=
        LT,         // <
        LE,         // <=
        GT,         // >
        GE,         // >=
        BANG,       // !
        AND,        // &&
        OR,         // ||
        INCREMENT,  // ++
        DECREMENT   // --
    };
    private:
        OperatorKind op;
        
    protected:
        explicit OperatorExpr(OperatorKind _op
        ,                    const Type* _type = nullptr
        ,                    Position _pos = default_pos()
        ):  ExprNode(_type, std::move(_pos))
        ,   op(_op)
        {}

    public:
        virtual void accept(ConstVisitor&) const noexcept override = 0;
        virtual void accept(NodeVisitor&) noexcept override = 0;

        OperatorKind get_op() const noexcept;
    };

    class BinOpExpr : public OperatorExpr
    {
    private:
        ExprPtr left;
        ExprPtr right;
    public:
        BinOpExpr(ExprPtr _left
        ,         ExprPtr _right
        ,         OperatorKind _op
        ,         const Type* _type = nullptr
        ,         Position _pos = default_pos()
        ):  OperatorExpr(_op
            ,           _type
            ,           std::move(_pos)
            )
        ,   left(std::move(_left))
        ,   right(std::move(_right))
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;

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
        UnaryOpExpr(ExprPtr _operand
        ,           OperatorKind _op
        ,           const Type* _type = nullptr
        ,           Position _pos = default_pos()
        ):  OperatorExpr(_op
            ,           _type
            ,           std::move(_pos)
            )
        ,   operand(std::move(_operand))
        {}

    public:
        virtual void accept(ConstVisitor&) const noexcept override = 0;
        virtual void accept(NodeVisitor&) noexcept override = 0;

        const ExprNode* get_operand() const noexcept;
        ExprNode* get_operand() noexcept;
    };

    class PrefixUnaryOpExpr : public UnaryOpExpr
    {
    public:
        PrefixUnaryOpExpr(ExprPtr _operand
        ,                 OperatorKind _op
        ,                 const Type* _type = nullptr
        ,                 Position _pos = default_pos()
        ):  UnaryOpExpr(std::move(_operand)
            ,           _op
            ,           _type
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
    };

    class PostfixUnaryOpExpr : public UnaryOpExpr
    {
    public:
        PostfixUnaryOpExpr(ExprPtr _operand
        ,                  OperatorKind _op
        ,                  const Type* _type = nullptr
        ,                  Position _pos = default_pos()
        ):  UnaryOpExpr(std::move(_operand)
            ,           _op
            ,           _type
            ,           std::move(_pos)
            )
        {}

        virtual void accept(ConstVisitor&) const noexcept override;
        virtual void accept(NodeVisitor&) noexcept override;
    };
}
