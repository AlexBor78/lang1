#pragma once

#include <memory>
#include <vector>
#include <common/common.h>

namespace lang::ast
{
    class ConstASTVisitor;
    class ASTVisitor;
    class StmtNode;
    class ExprNode;
    class DeclStmt;
    class BaseNode;

    using StmtPtr = std::unique_ptr<StmtNode>;
    using ExprPtr = std::unique_ptr<ExprNode>;
    using AST = std::vector<std::unique_ptr<BaseNode>>;

    class BaseNode
    {
    private:
        common::SourceLocation source_pos = {};
    protected:
        static constexpr inline common::SourceLocation default_pos() noexcept {
            return common::SourceLocation{};
        }
        explicit BaseNode(common::SourceLocation _pos = default_pos()): 
            source_pos(std::move(_pos))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept = 0;
        virtual void accept(ASTVisitor&) noexcept = 0;
        virtual ~BaseNode() = default;
        common::SourceLocation get_source_pos() const noexcept;
        void set_source_pos(const common::SourceLocation& pos) noexcept;
    };

    class StmtNode : public BaseNode
    {
    protected:
        explicit StmtNode(common::SourceLocation _pos = default_pos()):
            BaseNode(std::move(_pos))
        {}
    
    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;
    };

    class ExprNode : public StmtNode
    {
    protected:
        explicit ExprNode(common::SourceLocation _pos = default_pos()):
            StmtNode(std::move(_pos))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept override = 0;
        virtual void accept(ASTVisitor&) noexcept override = 0;
    };
}
