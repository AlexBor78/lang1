#include <lang/ast/ast.h>

namespace lang::ast
{
    common::SourceLocation BaseNode::get_source_pos() const noexcept
    {
        return source_pos;
    }
    void BaseNode::set_source_pos(const common::SourceLocation& _pos) noexcept
    {
        source_pos = _pos;
    }

}