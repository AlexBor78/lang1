#pragma once

#include <lang/ast/ast.h>
#include <lang/ast/expr.h>
#include <lang/frontend/token.h>

namespace lang::utils
{
    // enum types to string
    constexpr const char* stringify(ast::OperatorExpr::OperatorKind) noexcept;

    // TokenType to OperatorExpr::OperatorKind
    constexpr ast::OperatorExpr::OperatorKind token_to_op(frontend::TokenType);

    // print ast or vector of ast's
    void print(const std::vector<std::unique_ptr<ast::BaseNode>>&) noexcept;
    void print(const ast::BaseNode*) noexcept;

    constexpr bool is_unary(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_prefix_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_postfix_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_mul_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_add_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_compare_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_logical_op(ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_assign_op(ast::OperatorExpr::OperatorKind) noexcept;


// constexpr implementations ---------------------------------------------------

    constexpr const char* stringify(ast::OperatorExpr::OperatorKind op) noexcept{
        switch (op) {
            case ast::OperatorExpr::OperatorKind::ASSIGN:     return "ASSIGN";
            case ast::OperatorExpr::OperatorKind::PLUS:       return "PLUS";
            case ast::OperatorExpr::OperatorKind::MINUS:      return "MINUS";
            case ast::OperatorExpr::OperatorKind::STAR:       return "STAR";
            case ast::OperatorExpr::OperatorKind::SLASH:      return "SLASH";
            case ast::OperatorExpr::OperatorKind::PERCENT:    return "PERCENT";
            case ast::OperatorExpr::OperatorKind::AMPERSAND:  return "AMPERSAND";
            case ast::OperatorExpr::OperatorKind::EQ:         return "EQ";
            case ast::OperatorExpr::OperatorKind::NEQ:        return "NEQ";
            case ast::OperatorExpr::OperatorKind::LT:         return "LT";
            case ast::OperatorExpr::OperatorKind::LE:         return "LE";
            case ast::OperatorExpr::OperatorKind::GT:         return "GT";
            case ast::OperatorExpr::OperatorKind::GE:         return "GE";
            case ast::OperatorExpr::OperatorKind::BANG:       return "BANG";
            case ast::OperatorExpr::OperatorKind::AND:        return "AND";
            case ast::OperatorExpr::OperatorKind::OR:         return "OR";
            case ast::OperatorExpr::OperatorKind::INCREMENT:  return "INCREMENT";
            case ast::OperatorExpr::OperatorKind::DECREMENT:  return "DECREMENT";
            case ast::OperatorExpr::OperatorKind::TILDE:      return "TILDE";
            case ast::OperatorExpr::OperatorKind::BIT_OR:     return "BIT_OR";
            case ast::OperatorExpr::OperatorKind::BIT_XOR:    return "BIT_XOR";
            case ast::OperatorExpr::OperatorKind::SHIFT_LEFT: return "SHIFT_LEFT";
            case ast::OperatorExpr::OperatorKind::SHIFT_RIGHT:return "SHIFT_RIGHT";

            case ast::OperatorExpr::OperatorKind::ADD_ASSIGN: return "ADD_ASSIGN";
            case ast::OperatorExpr::OperatorKind::SUB_ASSIGN: return "SUB_ASSIGN";
            case ast::OperatorExpr::OperatorKind::MUL_ASSIGN: return "MUL_ASSIGN";
            case ast::OperatorExpr::OperatorKind::DIV_ASSIGN: return "DIV_ASSIGN";
            case ast::OperatorExpr::OperatorKind::MOD_ASSIGN: return "MOD_ASSIGN";

            case ast::OperatorExpr::OperatorKind::AND_ASSIGN: return "AND_ASSIGN";
            case ast::OperatorExpr::OperatorKind::OR_ASSIGN:  return "OR_ASSIGN";
            case ast::OperatorExpr::OperatorKind::XOR_ASSIGN: return "XOR_ASSIGN";
            case ast::OperatorExpr::OperatorKind::SHL_ASSIGN: return "SHL_ASSIGN";
            case ast::OperatorExpr::OperatorKind::SHR_ASSIGN: return "SHR_ASSIGN";
        } return "UNKNOWN";
    }

    constexpr ast::OperatorExpr::OperatorKind token_to_op(frontend::TokenType tok) {
        switch (tok) {
            case frontend::TokenType::ASSIGN:       return ast::OperatorExpr::OperatorKind::ASSIGN;
            case frontend::TokenType::PLUS:         return ast::OperatorExpr::OperatorKind::PLUS;
            case frontend::TokenType::MINUS:        return ast::OperatorExpr::OperatorKind::MINUS;
            case frontend::TokenType::STAR:         return ast::OperatorExpr::OperatorKind::STAR;
            case frontend::TokenType::SLASH:        return ast::OperatorExpr::OperatorKind::SLASH;
            case frontend::TokenType::PERCENT:      return ast::OperatorExpr::OperatorKind::PERCENT;
            case frontend::TokenType::AMPERSAND:    return ast::OperatorExpr::OperatorKind::AMPERSAND;
            case frontend::TokenType::EQ:           return ast::OperatorExpr::OperatorKind::EQ;
            case frontend::TokenType::NEQ:          return ast::OperatorExpr::OperatorKind::NEQ;
            case frontend::TokenType::LT:           return ast::OperatorExpr::OperatorKind::LT;
            case frontend::TokenType::LE:           return ast::OperatorExpr::OperatorKind::LE;
            case frontend::TokenType::GT:           return ast::OperatorExpr::OperatorKind::GT;
            case frontend::TokenType::GE:           return ast::OperatorExpr::OperatorKind::GE;
            case frontend::TokenType::BANG:         return ast::OperatorExpr::OperatorKind::BANG;
            case frontend::TokenType::AND:          return ast::OperatorExpr::OperatorKind::AND;
            case frontend::TokenType::OR:           return ast::OperatorExpr::OperatorKind::OR;
            case frontend::TokenType::INCREMENT:    return ast::OperatorExpr::OperatorKind::INCREMENT;
            case frontend::TokenType::DECREMENT:    return ast::OperatorExpr::OperatorKind::DECREMENT;
            case frontend::TokenType::TILDE:        return ast::OperatorExpr::OperatorKind::TILDE;
            case frontend::TokenType::BIT_OR:       return ast::OperatorExpr::OperatorKind::BIT_OR;
            case frontend::TokenType::BIT_XOR:      return ast::OperatorExpr::OperatorKind::BIT_XOR;
            case frontend::TokenType::SHIFT_LEFT:   return ast::OperatorExpr::OperatorKind::SHIFT_LEFT;
            case frontend::TokenType::SHIFT_RIGHT:  return ast::OperatorExpr::OperatorKind::SHIFT_RIGHT;

            case frontend::TokenType::ADD_ASSIGN:   return ast::OperatorExpr::OperatorKind::ADD_ASSIGN;
            case frontend::TokenType::SUB_ASSIGN:   return ast::OperatorExpr::OperatorKind::SUB_ASSIGN;
            case frontend::TokenType::MUL_ASSIGN:   return ast::OperatorExpr::OperatorKind::MUL_ASSIGN;
            case frontend::TokenType::DIV_ASSIGN:   return ast::OperatorExpr::OperatorKind::DIV_ASSIGN;
            case frontend::TokenType::MOD_ASSIGN:   return ast::OperatorExpr::OperatorKind::MOD_ASSIGN;

            case frontend::TokenType::AND_ASSIGN:   return ast::OperatorExpr::OperatorKind::AND_ASSIGN;
            case frontend::TokenType::OR_ASSIGN:    return ast::OperatorExpr::OperatorKind::OR_ASSIGN;
            case frontend::TokenType::XOR_ASSIGN:   return ast::OperatorExpr::OperatorKind::XOR_ASSIGN;
            case frontend::TokenType::SHL_ASSIGN:   return ast::OperatorExpr::OperatorKind::SHL_ASSIGN;
            case frontend::TokenType::SHR_ASSIGN:   return ast::OperatorExpr::OperatorKind::SHR_ASSIGN;

            default: throw std::runtime_error("wrong TokenType to OperatorKind");
        }
    }

    constexpr bool is_unary(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case ast::OperatorExpr::OperatorKind::BANG:
        case ast::OperatorExpr::OperatorKind::TILDE:
        case ast::OperatorExpr::OperatorKind::AMPERSAND: // address-of
        case ast::OperatorExpr::OperatorKind::STAR:      // deref
        case ast::OperatorExpr::OperatorKind::PLUS:      // unary +
        case ast::OperatorExpr::OperatorKind::MINUS:     // unary -
        case ast::OperatorExpr::OperatorKind::INCREMENT:
        case ast::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_prefix_op(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case ast::OperatorExpr::OperatorKind::BANG:
        case ast::OperatorExpr::OperatorKind::TILDE:
        case ast::OperatorExpr::OperatorKind::AMPERSAND:
        case ast::OperatorExpr::OperatorKind::STAR:
        case ast::OperatorExpr::OperatorKind::PLUS:
        case ast::OperatorExpr::OperatorKind::MINUS:
        case ast::OperatorExpr::OperatorKind::INCREMENT:
        case ast::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_postfix_op(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case ast::OperatorExpr::OperatorKind::INCREMENT:
        case ast::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_mul_op(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        // arithmetic
        case ast::OperatorExpr::OperatorKind::STAR:
        case ast::OperatorExpr::OperatorKind::SLASH:
        case ast::OperatorExpr::OperatorKind::PERCENT:

        // bitwise
        case ast::OperatorExpr::OperatorKind::AMPERSAND:
        case ast::OperatorExpr::OperatorKind::BIT_XOR:
        case ast::OperatorExpr::OperatorKind::BIT_OR:
        case ast::OperatorExpr::OperatorKind::SHIFT_LEFT:
        case ast::OperatorExpr::OperatorKind::SHIFT_RIGHT:
            return true;

        default: return false;
    }
}

constexpr bool is_add_op(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case ast::OperatorExpr::OperatorKind::PLUS:
        case ast::OperatorExpr::OperatorKind::MINUS:
            return true;
        default: return false;
    }
}

constexpr bool is_compare_op(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case ast::OperatorExpr::OperatorKind::EQ:
        case ast::OperatorExpr::OperatorKind::NEQ:
        case ast::OperatorExpr::OperatorKind::LT:
        case ast::OperatorExpr::OperatorKind::LE:
        case ast::OperatorExpr::OperatorKind::GT:
        case ast::OperatorExpr::OperatorKind::GE:
            return true;
        default: return false;
    }
}

constexpr bool is_logical_op(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case ast::OperatorExpr::OperatorKind::AND:
        case ast::OperatorExpr::OperatorKind::OR:
            return true;
        default: return false;
    }
}

constexpr bool is_assign_op(ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case ast::OperatorExpr::OperatorKind::ASSIGN:
        case ast::OperatorExpr::OperatorKind::ADD_ASSIGN:
        case ast::OperatorExpr::OperatorKind::SUB_ASSIGN:
        case ast::OperatorExpr::OperatorKind::MUL_ASSIGN:
        case ast::OperatorExpr::OperatorKind::DIV_ASSIGN:
        case ast::OperatorExpr::OperatorKind::MOD_ASSIGN:
        case ast::OperatorExpr::OperatorKind::AND_ASSIGN:
        case ast::OperatorExpr::OperatorKind::OR_ASSIGN:
        case ast::OperatorExpr::OperatorKind::XOR_ASSIGN:
        case ast::OperatorExpr::OperatorKind::SHL_ASSIGN:
        case ast::OperatorExpr::OperatorKind::SHR_ASSIGN:
            return true;
        default: return false;
    }
}

}
