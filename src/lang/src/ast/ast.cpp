#include "defs.h"
#include <ast/ast.h>

namespace lang::ast
{
    Position BaseNode::get_source_pos() const noexcept
    {
        return source_pos;
    }
    void BaseNode::set_source_pos(const Position& _pos) noexcept
    {
        source_pos = _pos;
    }

    const Type* ExprNode::get_type() const noexcept
    {
        return type;
    }
    void ExprNode::set_type(const Type* _type) noexcept
    {
        type = _type;
    }
}