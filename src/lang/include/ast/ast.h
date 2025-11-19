#pragma once

#include <memory>
#include <defs.h>

namespace lang::ast
{
    class ConstASTVisitor;
    class ASTVisitor;
    class StmtNode;
    class ExprNode;
    class Type;

    using StmtPtr = std::unique_ptr<StmtNode>;
    using ExprPtr = std::unique_ptr<ExprNode>;

    class BaseNode
    {
    private:
        Position source_pos = {
            .path = "null",
            .line = 0,
            .column = 0
        };
    protected:
        static constexpr Position default_pos() noexcept 
        {
            return {
                .path = "null",
                .line = 0,
                .column = 0
            };
        }
        explicit BaseNode(Position _pos = default_pos()): 
            source_pos(std::move(_pos))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept = 0;
        virtual void accept(ASTVisitor&) noexcept = 0;
        virtual ~BaseNode() = default;
        Position get_source_pos() const noexcept;
        void set_source_pos(const Position& pos) noexcept;
    };

    class StmtNode : public BaseNode
    {
    protected:
        explicit StmtNode(Position _pos = default_pos()):
            BaseNode(std::move(_pos))
        {}
    
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;
    };

    class ExprNode : public StmtNode
    {
    private:
        const Type* type{nullptr};
        
    protected:
        explicit ExprNode(const Type* _type): 
            type(_type)
        {}

        explicit ExprNode(Position _pos = default_pos()): 
            StmtNode(std::move(_pos))
        {}

        explicit ExprNode(const Type* _type, Position _pos = default_pos())
        :   StmtNode(std::move(_pos))
        ,   type(_type)
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;
        const Type* get_type() const noexcept;
        void set_type(const Type*) noexcept;
    };
}
