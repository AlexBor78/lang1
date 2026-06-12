#pragma once

#include <lang/syntax/ast.h>
#include <lang/syntax/expr.h>
#include <lang/syntax/token.h>

namespace lang::utils
{
    // enum types to string
    constexpr const char* stringify(syntax::OperatorExpr::OperatorKind) noexcept;

    // TokenType to OperatorExpr::OperatorKind
    constexpr syntax::OperatorExpr::OperatorKind token_to_op(syntax::TokenType);

    // print ast or vector of ast's
    void print(const std::vector<std::unique_ptr<syntax::BaseNode>>&) noexcept;
    void print(const syntax::BaseNode*) noexcept;

    constexpr bool is_unary_op(syntax::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_prefix_op(syntax::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_postfix_op(syntax::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_mul_op(syntax::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_add_op(syntax::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_compare_op(syntax::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_logical_op(syntax::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_assign_op(syntax::OperatorExpr::OperatorKind) noexcept;


// constexpr implementations ---------------------------------------------------

    constexpr const char* stringify(syntax::OperatorExpr::OperatorKind op) noexcept {
        switch (op) {
            case syntax::OperatorExpr::OperatorKind::ASSIGN:     return "ASSIGN";
            case syntax::OperatorExpr::OperatorKind::PLUS:       return "PLUS";
            case syntax::OperatorExpr::OperatorKind::MINUS:      return "MINUS";
            case syntax::OperatorExpr::OperatorKind::STAR:       return "STAR";
            case syntax::OperatorExpr::OperatorKind::SLASH:      return "SLASH";
            case syntax::OperatorExpr::OperatorKind::PERCENT:    return "PERCENT";
            case syntax::OperatorExpr::OperatorKind::AMPERSAND:  return "AMPERSAND";
            case syntax::OperatorExpr::OperatorKind::EQ:         return "EQ";
            case syntax::OperatorExpr::OperatorKind::NEQ:        return "NEQ";
            case syntax::OperatorExpr::OperatorKind::LT:         return "LT";
            case syntax::OperatorExpr::OperatorKind::LE:         return "LE";
            case syntax::OperatorExpr::OperatorKind::GT:         return "GT";
            case syntax::OperatorExpr::OperatorKind::GE:         return "GE";
            case syntax::OperatorExpr::OperatorKind::BANG:       return "BANG";
            case syntax::OperatorExpr::OperatorKind::AND:        return "AND";
            case syntax::OperatorExpr::OperatorKind::OR:         return "OR";
            case syntax::OperatorExpr::OperatorKind::INCREMENT:  return "INCREMENT";
            case syntax::OperatorExpr::OperatorKind::DECREMENT:  return "DECREMENT";
            case syntax::OperatorExpr::OperatorKind::TILDE:      return "TILDE";
            case syntax::OperatorExpr::OperatorKind::BIT_OR:     return "BIT_OR";
            case syntax::OperatorExpr::OperatorKind::BIT_XOR:    return "BIT_XOR";
            case syntax::OperatorExpr::OperatorKind::SHIFT_LEFT: return "SHIFT_LEFT";
            case syntax::OperatorExpr::OperatorKind::SHIFT_RIGHT:return "SHIFT_RIGHT";

            case syntax::OperatorExpr::OperatorKind::ADD_ASSIGN: return "ADD_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::SUB_ASSIGN: return "SUB_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::MUL_ASSIGN: return "MUL_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::DIV_ASSIGN: return "DIV_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::MOD_ASSIGN: return "MOD_ASSIGN";

            case syntax::OperatorExpr::OperatorKind::AND_ASSIGN: return "AND_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::OR_ASSIGN:  return "OR_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::XOR_ASSIGN: return "XOR_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::SHL_ASSIGN: return "SHL_ASSIGN";
            case syntax::OperatorExpr::OperatorKind::SHR_ASSIGN: return "SHR_ASSIGN";
        } return "UNKNOWN";
    }

    constexpr syntax::OperatorExpr::OperatorKind token_to_op(syntax::TokenType tok) {
        switch (tok) {
            case syntax::TokenType::ASSIGN:       return syntax::OperatorExpr::OperatorKind::ASSIGN;
            case syntax::TokenType::PLUS:         return syntax::OperatorExpr::OperatorKind::PLUS;
            case syntax::TokenType::MINUS:        return syntax::OperatorExpr::OperatorKind::MINUS;
            case syntax::TokenType::STAR:         return syntax::OperatorExpr::OperatorKind::STAR;
            case syntax::TokenType::SLASH:        return syntax::OperatorExpr::OperatorKind::SLASH;
            case syntax::TokenType::PERCENT:      return syntax::OperatorExpr::OperatorKind::PERCENT;
            case syntax::TokenType::AMPERSAND:    return syntax::OperatorExpr::OperatorKind::AMPERSAND;
            case syntax::TokenType::EQ:           return syntax::OperatorExpr::OperatorKind::EQ;
            case syntax::TokenType::NEQ:          return syntax::OperatorExpr::OperatorKind::NEQ;
            case syntax::TokenType::LT:           return syntax::OperatorExpr::OperatorKind::LT;
            case syntax::TokenType::LE:           return syntax::OperatorExpr::OperatorKind::LE;
            case syntax::TokenType::GT:           return syntax::OperatorExpr::OperatorKind::GT;
            case syntax::TokenType::GE:           return syntax::OperatorExpr::OperatorKind::GE;
            case syntax::TokenType::BANG:         return syntax::OperatorExpr::OperatorKind::BANG;
            case syntax::TokenType::AND:          return syntax::OperatorExpr::OperatorKind::AND;
            case syntax::TokenType::OR:           return syntax::OperatorExpr::OperatorKind::OR;
            case syntax::TokenType::INCREMENT:    return syntax::OperatorExpr::OperatorKind::INCREMENT;
            case syntax::TokenType::DECREMENT:    return syntax::OperatorExpr::OperatorKind::DECREMENT;
            case syntax::TokenType::TILDE:        return syntax::OperatorExpr::OperatorKind::TILDE;
            case syntax::TokenType::BIT_OR:       return syntax::OperatorExpr::OperatorKind::BIT_OR;
            case syntax::TokenType::BIT_XOR:      return syntax::OperatorExpr::OperatorKind::BIT_XOR;
            case syntax::TokenType::SHIFT_LEFT:   return syntax::OperatorExpr::OperatorKind::SHIFT_LEFT;
            case syntax::TokenType::SHIFT_RIGHT:  return syntax::OperatorExpr::OperatorKind::SHIFT_RIGHT;

            case syntax::TokenType::ADD_ASSIGN:   return syntax::OperatorExpr::OperatorKind::ADD_ASSIGN;
            case syntax::TokenType::SUB_ASSIGN:   return syntax::OperatorExpr::OperatorKind::SUB_ASSIGN;
            case syntax::TokenType::MUL_ASSIGN:   return syntax::OperatorExpr::OperatorKind::MUL_ASSIGN;
            case syntax::TokenType::DIV_ASSIGN:   return syntax::OperatorExpr::OperatorKind::DIV_ASSIGN;
            case syntax::TokenType::MOD_ASSIGN:   return syntax::OperatorExpr::OperatorKind::MOD_ASSIGN;

            case syntax::TokenType::AND_ASSIGN:   return syntax::OperatorExpr::OperatorKind::AND_ASSIGN;
            case syntax::TokenType::OR_ASSIGN:    return syntax::OperatorExpr::OperatorKind::OR_ASSIGN;
            case syntax::TokenType::XOR_ASSIGN:   return syntax::OperatorExpr::OperatorKind::XOR_ASSIGN;
            case syntax::TokenType::SHL_ASSIGN:   return syntax::OperatorExpr::OperatorKind::SHL_ASSIGN;
            case syntax::TokenType::SHR_ASSIGN:   return syntax::OperatorExpr::OperatorKind::SHR_ASSIGN;

            default: throw std::runtime_error("wrong TokenType to OperatorKind");
        }
    }

    constexpr bool is_unary_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::OperatorExpr::OperatorKind::BANG:      // LOGIC_NOT
        case syntax::OperatorExpr::OperatorKind::TILDE:     // BIT_NOT
        case syntax::OperatorExpr::OperatorKind::AMPERSAND: // address-of
        case syntax::OperatorExpr::OperatorKind::STAR:      // deref
        case syntax::OperatorExpr::OperatorKind::PLUS:      // unary +
        case syntax::OperatorExpr::OperatorKind::MINUS:     // unary -
        case syntax::OperatorExpr::OperatorKind::INCREMENT:
        case syntax::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_prefix_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::OperatorExpr::OperatorKind::BANG:
        case syntax::OperatorExpr::OperatorKind::TILDE:
        case syntax::OperatorExpr::OperatorKind::AMPERSAND:
        case syntax::OperatorExpr::OperatorKind::STAR:
        case syntax::OperatorExpr::OperatorKind::PLUS:
        case syntax::OperatorExpr::OperatorKind::MINUS:
        case syntax::OperatorExpr::OperatorKind::INCREMENT:
        case syntax::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_postfix_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::OperatorExpr::OperatorKind::INCREMENT:
        case syntax::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_mul_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        // arithmetic
        case syntax::OperatorExpr::OperatorKind::STAR:
        case syntax::OperatorExpr::OperatorKind::SLASH:
        case syntax::OperatorExpr::OperatorKind::PERCENT:

        // bitwise
        case syntax::OperatorExpr::OperatorKind::AMPERSAND:
        case syntax::OperatorExpr::OperatorKind::BIT_XOR:
        case syntax::OperatorExpr::OperatorKind::BIT_OR:
        case syntax::OperatorExpr::OperatorKind::SHIFT_LEFT:
        case syntax::OperatorExpr::OperatorKind::SHIFT_RIGHT:
            return true;

        default: return false;
    }
}

constexpr bool is_add_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::OperatorExpr::OperatorKind::PLUS:
        case syntax::OperatorExpr::OperatorKind::MINUS:
            return true;
        default: return false;
    }
}

constexpr bool is_compare_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::OperatorExpr::OperatorKind::EQ:
        case syntax::OperatorExpr::OperatorKind::NEQ:
        case syntax::OperatorExpr::OperatorKind::LT:
        case syntax::OperatorExpr::OperatorKind::LE:
        case syntax::OperatorExpr::OperatorKind::GT:
        case syntax::OperatorExpr::OperatorKind::GE:
            return true;
        default: return false;
    }
}

constexpr bool is_logical_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::OperatorExpr::OperatorKind::AND:
        case syntax::OperatorExpr::OperatorKind::OR:
            return true;
        default: return false;
    }
}

constexpr bool is_assign_op(syntax::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::OperatorExpr::OperatorKind::ASSIGN:
        case syntax::OperatorExpr::OperatorKind::ADD_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::SUB_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::MUL_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::DIV_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::MOD_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::AND_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::OR_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::XOR_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::SHL_ASSIGN:
        case syntax::OperatorExpr::OperatorKind::SHR_ASSIGN:
            return true;
        default: return false;
    }
}

}
