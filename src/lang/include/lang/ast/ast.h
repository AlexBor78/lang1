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

    class BaseNode {
    protected:
        static constexpr inline common::SourceLocation default_pos() noexcept {
            return common::SourceLocation{};
        }
        explicit BaseNode(common::SourceLocation _full_range_loc = default_pos()): 
            source_pos(std::move(_full_range_loc))
        {}

    public:
        virtual void accept(ConstASTVisitor&) const noexcept = 0;
        virtual void accept(ASTVisitor&) noexcept = 0;
        virtual ~BaseNode() = default;
        common::SourceLocation source_pos = {};
    };

    class StmtNode : public BaseNode
    {
    protected:
			using BaseNode::BaseNode;
    };

    class ExprNode : public StmtNode
    {
    protected:
			using StmtNode::StmtNode;
    };
}
