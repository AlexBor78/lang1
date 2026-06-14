#pragma once

#include <memory>
#include <vector>
#include <common/common.h>

namespace lang::syntax
{
    class ConstASTVisitor;
    class ASTVisitor;
    class StmtNode;
    class ExprNode;
    class DeclStmt;
    class BaseNode;

    using StmtPtr = StmtNode*;
    using ExprPtr = ExprNode*;
    using AST = std::vector<BaseNode*>;

    class BaseNode {
    protected:
        static constexpr inline common::SourceLocation default_pos() noexcept {
            return common::SourceLocation{};
        }
        explicit BaseNode(
					common::SourceLocation _full_range_loc = default_pos()
				):	source_pos(std::move(_full_range_loc))
        {}

    public:
        virtual ~BaseNode() = default;
        common::SourceLocation source_pos = {};
        virtual void accept(ASTVisitor&) noexcept = 0;
        virtual void accept(ConstASTVisitor&) const noexcept = 0;
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
