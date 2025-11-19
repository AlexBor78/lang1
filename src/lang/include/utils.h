#pragma once

#include <vector>

#include <defs.h>
#include <ast/expr.h>

namespace lang::utils
{
    // TokenType TO string
    constexpr const char* stringify(TokenType);

    void print(const std::vector<Token>&);
    void print(const Token&);

    // OperatorExpr::OperatorKind TO string
    constexpr const char* stringify(ast::OperatorExpr::OperatorKind);
    
    // TokenType TO OperatorExpr::OperatorKind
    ast::OperatorExpr::OperatorKind token_to_op(TokenType);
}
