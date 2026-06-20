#pragma once

#include <memory>
#include <vector>
#include <lang/syntax/token.h>
#include <lang/syntax/ast/ast.h>
#include <lang/syntax/ast/expr.h>

namespace lang::utils
{
    // enum types to string
    constexpr const char* stringify(syntax::TokenType) noexcept;

    // print token or vector of tokens
    void print(const std::vector<syntax::Token>&) noexcept;
    void print(const syntax::Token&) noexcept;


    constexpr bool is_declarator(syntax::TokenType) noexcept;
    constexpr bool is_operator(syntax::TokenType) noexcept;
    constexpr bool is_literal(syntax::TokenType) noexcept;
    // constexpr bool is_(syntax::TokenType) noexcept;


// constexpr implementations ---------------------------------------------------

    constexpr const char* stringify(syntax::TokenType ty) noexcept {
        switch(ty) {
            // Identifiers and literals
            case syntax::TokenType::IDENTIFIER:   return "IDENTIFIER";
            case syntax::TokenType::NUMBER:       return "NUMBER";
            case syntax::TokenType::STRING:       return "STRING";
            case syntax::TokenType::TRUE:         return "TRUE";
            case syntax::TokenType::FALSE:        return "FALSE";

            // Operators
            case syntax::TokenType::ASSIGN:       return "ASSIGN";
            case syntax::TokenType::PLUS:         return "PLUS";
            case syntax::TokenType::MINUS:        return "MINUS";
            case syntax::TokenType::STAR:         return "STAR";
            case syntax::TokenType::SLASH:        return "SLASH";
            case syntax::TokenType::PERCENT:      return "PERCENT";
            case syntax::TokenType::AMPERSAND:    return "AMPERSAND";
            case syntax::TokenType::EQ:           return "EQ";
            case syntax::TokenType::NEQ:          return "NEQ";
            case syntax::TokenType::LT:           return "LT";
            case syntax::TokenType::LE:           return "LE";
            case syntax::TokenType::GT:           return "GT";
            case syntax::TokenType::GE:           return "GE";
            case syntax::TokenType::BANG:         return "BANG";
            case syntax::TokenType::AND:          return "AND";
            case syntax::TokenType::OR:           return "OR";
            case syntax::TokenType::INCREMENT:    return "INCREMENT";
            case syntax::TokenType::DECREMENT:    return "DECREMENT";
            case syntax::TokenType::TILDE:        return "TILDE";
            case syntax::TokenType::BIT_OR:       return "BIT_OR";
            // case syntax::TokenType::BIT_AND:      return "BIT_AND"; AMPERSAND
            case syntax::TokenType::BIT_XOR:      return "BIT_XOR";
            case syntax::TokenType::SHIFT_LEFT:   return "SHIFT_LEFT";
            case syntax::TokenType::SHIFT_RIGHT:  return "SHIFT_RIGHT";

            // Compound assignments
            case syntax::TokenType::ADD_ASSIGN:   return "ADD_ASSIGN";
            case syntax::TokenType::SUB_ASSIGN:   return "SUB_ASSIGN";
            case syntax::TokenType::MUL_ASSIGN:   return "MUL_ASSIGN";
            case syntax::TokenType::DIV_ASSIGN:   return "DIV_ASSIGN";
            case syntax::TokenType::MOD_ASSIGN:   return "MOD_ASSIGN";
            case syntax::TokenType::AND_ASSIGN:   return "AND_ASSIGN";
            case syntax::TokenType::OR_ASSIGN:    return "OR_ASSIGN";
            case syntax::TokenType::XOR_ASSIGN:   return "XOR_ASSIGN";
            case syntax::TokenType::SHL_ASSIGN:   return "SHL_ASSIGN";
            case syntax::TokenType::SHR_ASSIGN:   return "SHR_ASSIGN";

            // Delimiters
            case syntax::TokenType::LPAREN:       return "LPAREN";
            case syntax::TokenType::RPAREN:       return "RPAREN";
            case syntax::TokenType::LBRACE:       return "LBRACE";
            case syntax::TokenType::RBRACE:       return "RBRACE";
            case syntax::TokenType::LBRACKET:     return "LBRACKET";
            case syntax::TokenType::RBRACKET:     return "RBRACKET";
            case syntax::TokenType::LARROW:       return "LARROW";
            case syntax::TokenType::RARROW:       return "RARROW";
            case syntax::TokenType::SEMICOLON:    return "SEMICOLON";
            case syntax::TokenType::COLON:        return "COLON";
            case syntax::TokenType::DOUBLECOLON:  return "DOUBLECOLON";
            case syntax::TokenType::COMMA:        return "COMMA";
            case syntax::TokenType::DOT:          return "DOT";

            // Declaration keywords
            case syntax::TokenType::EXPORT:       return "EXPORT";
            case syntax::TokenType::EXTERN:       return "EXTERN";
            case syntax::TokenType::FN:           return "FN";
            case syntax::TokenType::CONST:        return "CONST";
            case syntax::TokenType::MUTABLE:      return "MUTABLE";
            case syntax::TokenType::STRUCT:       return "STRUCT";
            case syntax::TokenType::ALIAS:       return "ALIAS";
            case syntax::TokenType::ENUM:         return "ENUM";

            // Other keywords
            case syntax::TokenType::IF:           return "IF";
            case syntax::TokenType::ELSE:         return "ELSE";
            case syntax::TokenType::FOR:          return "FOR";
            case syntax::TokenType::WHILE:        return "WHILE";
            case syntax::TokenType::BREAK:        return "BREAK";
            case syntax::TokenType::CONTINUE:     return "CONTINUE";
            case syntax::TokenType::RETURN:       return "RETURN";
            case syntax::TokenType::IMPORT:       return "IMPORT";
            case syntax::TokenType::STACK:        return "STACK";
            
            // Special
            case syntax::TokenType::END:          return "END";
            case syntax::TokenType::ILLEGAL:      return "ILLEGAL";
        } return "UNKNOWN";
    }

    constexpr bool is_operator(syntax::TokenType tok) noexcept {
        return static_cast<int>(tok) > static_cast<int>(syntax::TokenType::FALSE) 
        &&     static_cast<int>(tok) < static_cast<int>(syntax::TokenType::LPAREN);
    }

    constexpr bool is_literal(syntax::TokenType tok) noexcept {
        return static_cast<int>(tok) > static_cast<int>(syntax::TokenType::IDENTIFIER) 
        &&     static_cast<int>(tok) < static_cast<int>(syntax::TokenType::ASSIGN);
    }

    constexpr bool is_declarator(syntax::TokenType tok) noexcept {
        return  (static_cast<int>(tok) > static_cast<int>(syntax::TokenType::DOT)
        &&      static_cast<int>(tok) < static_cast<int>(syntax::TokenType::IF))
        ||      tok == syntax::TokenType::IDENTIFIER
        ||      tok == syntax::TokenType::AMPERSAND
        ||      tok == syntax::TokenType::STAR;
    }
		
    // enum types to string
    constexpr const char* stringify(syntax::ast::OperatorExpr::OperatorKind) noexcept;

    // TokenType to OperatorExpr::OperatorKind
    constexpr syntax::ast::OperatorExpr::OperatorKind token_to_op(syntax::TokenType);

    // print ast or vector of ast's
    void print(const std::vector<std::unique_ptr<syntax::ast::BaseNode>>&) noexcept;
    void print(const syntax::ast::BaseNode*) noexcept;

    constexpr bool is_unary_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_prefix_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_postfix_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_mul_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_add_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_compare_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_logical_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;
    constexpr bool is_assign_op(syntax::ast::OperatorExpr::OperatorKind) noexcept;


// constexpr implementations ---------------------------------------------------

    constexpr const char* stringify(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
        switch (op) {
            case syntax::ast::OperatorExpr::OperatorKind::ASSIGN:     return "ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::PLUS:       return "PLUS";
            case syntax::ast::OperatorExpr::OperatorKind::MINUS:      return "MINUS";
            case syntax::ast::OperatorExpr::OperatorKind::STAR:       return "STAR";
            case syntax::ast::OperatorExpr::OperatorKind::SLASH:      return "SLASH";
            case syntax::ast::OperatorExpr::OperatorKind::PERCENT:    return "PERCENT";
            case syntax::ast::OperatorExpr::OperatorKind::AMPERSAND:  return "AMPERSAND";
            case syntax::ast::OperatorExpr::OperatorKind::EQ:         return "EQ";
            case syntax::ast::OperatorExpr::OperatorKind::NEQ:        return "NEQ";
            case syntax::ast::OperatorExpr::OperatorKind::LT:         return "LT";
            case syntax::ast::OperatorExpr::OperatorKind::LE:         return "LE";
            case syntax::ast::OperatorExpr::OperatorKind::GT:         return "GT";
            case syntax::ast::OperatorExpr::OperatorKind::GE:         return "GE";
            case syntax::ast::OperatorExpr::OperatorKind::BANG:       return "BANG";
            case syntax::ast::OperatorExpr::OperatorKind::AND:        return "AND";
            case syntax::ast::OperatorExpr::OperatorKind::OR:         return "OR";
            case syntax::ast::OperatorExpr::OperatorKind::INCREMENT:  return "INCREMENT";
            case syntax::ast::OperatorExpr::OperatorKind::DECREMENT:  return "DECREMENT";
            case syntax::ast::OperatorExpr::OperatorKind::TILDE:      return "TILDE";
            case syntax::ast::OperatorExpr::OperatorKind::BIT_OR:     return "BIT_OR";
            case syntax::ast::OperatorExpr::OperatorKind::BIT_XOR:    return "BIT_XOR";
            case syntax::ast::OperatorExpr::OperatorKind::SHIFT_LEFT: return "SHIFT_LEFT";
            case syntax::ast::OperatorExpr::OperatorKind::SHIFT_RIGHT:return "SHIFT_RIGHT";

            case syntax::ast::OperatorExpr::OperatorKind::ADD_ASSIGN: return "ADD_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::SUB_ASSIGN: return "SUB_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::MUL_ASSIGN: return "MUL_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::DIV_ASSIGN: return "DIV_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::MOD_ASSIGN: return "MOD_ASSIGN";

            case syntax::ast::OperatorExpr::OperatorKind::AND_ASSIGN: return "AND_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::OR_ASSIGN:  return "OR_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::XOR_ASSIGN: return "XOR_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::SHL_ASSIGN: return "SHL_ASSIGN";
            case syntax::ast::OperatorExpr::OperatorKind::SHR_ASSIGN: return "SHR_ASSIGN";
        } return "UNKNOWN";
    }

    constexpr syntax::ast::OperatorExpr::OperatorKind token_to_op(syntax::TokenType tok) {
        switch (tok) {
            case syntax::TokenType::ASSIGN:       return syntax::ast::OperatorExpr::OperatorKind::ASSIGN;
            case syntax::TokenType::PLUS:         return syntax::ast::OperatorExpr::OperatorKind::PLUS;
            case syntax::TokenType::MINUS:        return syntax::ast::OperatorExpr::OperatorKind::MINUS;
            case syntax::TokenType::STAR:         return syntax::ast::OperatorExpr::OperatorKind::STAR;
            case syntax::TokenType::SLASH:        return syntax::ast::OperatorExpr::OperatorKind::SLASH;
            case syntax::TokenType::PERCENT:      return syntax::ast::OperatorExpr::OperatorKind::PERCENT;
            case syntax::TokenType::AMPERSAND:    return syntax::ast::OperatorExpr::OperatorKind::AMPERSAND;
            case syntax::TokenType::EQ:           return syntax::ast::OperatorExpr::OperatorKind::EQ;
            case syntax::TokenType::NEQ:          return syntax::ast::OperatorExpr::OperatorKind::NEQ;
            case syntax::TokenType::LT:           return syntax::ast::OperatorExpr::OperatorKind::LT;
            case syntax::TokenType::LE:           return syntax::ast::OperatorExpr::OperatorKind::LE;
            case syntax::TokenType::GT:           return syntax::ast::OperatorExpr::OperatorKind::GT;
            case syntax::TokenType::GE:           return syntax::ast::OperatorExpr::OperatorKind::GE;
            case syntax::TokenType::BANG:         return syntax::ast::OperatorExpr::OperatorKind::BANG;
            case syntax::TokenType::AND:          return syntax::ast::OperatorExpr::OperatorKind::AND;
            case syntax::TokenType::OR:           return syntax::ast::OperatorExpr::OperatorKind::OR;
            case syntax::TokenType::INCREMENT:    return syntax::ast::OperatorExpr::OperatorKind::INCREMENT;
            case syntax::TokenType::DECREMENT:    return syntax::ast::OperatorExpr::OperatorKind::DECREMENT;
            case syntax::TokenType::TILDE:        return syntax::ast::OperatorExpr::OperatorKind::TILDE;
            case syntax::TokenType::BIT_OR:       return syntax::ast::OperatorExpr::OperatorKind::BIT_OR;
            case syntax::TokenType::BIT_XOR:      return syntax::ast::OperatorExpr::OperatorKind::BIT_XOR;
            case syntax::TokenType::SHIFT_LEFT:   return syntax::ast::OperatorExpr::OperatorKind::SHIFT_LEFT;
            case syntax::TokenType::SHIFT_RIGHT:  return syntax::ast::OperatorExpr::OperatorKind::SHIFT_RIGHT;

            case syntax::TokenType::ADD_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::ADD_ASSIGN;
            case syntax::TokenType::SUB_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::SUB_ASSIGN;
            case syntax::TokenType::MUL_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::MUL_ASSIGN;
            case syntax::TokenType::DIV_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::DIV_ASSIGN;
            case syntax::TokenType::MOD_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::MOD_ASSIGN;

            case syntax::TokenType::AND_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::AND_ASSIGN;
            case syntax::TokenType::OR_ASSIGN:    return syntax::ast::OperatorExpr::OperatorKind::OR_ASSIGN;
            case syntax::TokenType::XOR_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::XOR_ASSIGN;
            case syntax::TokenType::SHL_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::SHL_ASSIGN;
            case syntax::TokenType::SHR_ASSIGN:   return syntax::ast::OperatorExpr::OperatorKind::SHR_ASSIGN;

            default: throw std::runtime_error("wrong TokenType to OperatorKind");
        }
    }

    constexpr bool is_unary_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::ast::OperatorExpr::OperatorKind::BANG:      // LOGIC_NOT
        case syntax::ast::OperatorExpr::OperatorKind::TILDE:     // BIT_NOT
        case syntax::ast::OperatorExpr::OperatorKind::AMPERSAND: // address-of
        case syntax::ast::OperatorExpr::OperatorKind::STAR:      // deref
        case syntax::ast::OperatorExpr::OperatorKind::PLUS:      // unary +
        case syntax::ast::OperatorExpr::OperatorKind::MINUS:     // unary -
        case syntax::ast::OperatorExpr::OperatorKind::INCREMENT:
        case syntax::ast::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_prefix_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::ast::OperatorExpr::OperatorKind::BANG:
        case syntax::ast::OperatorExpr::OperatorKind::TILDE:
        case syntax::ast::OperatorExpr::OperatorKind::AMPERSAND:
        case syntax::ast::OperatorExpr::OperatorKind::STAR:
        case syntax::ast::OperatorExpr::OperatorKind::PLUS:
        case syntax::ast::OperatorExpr::OperatorKind::MINUS:
        case syntax::ast::OperatorExpr::OperatorKind::INCREMENT:
        case syntax::ast::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_postfix_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::ast::OperatorExpr::OperatorKind::INCREMENT:
        case syntax::ast::OperatorExpr::OperatorKind::DECREMENT:
            return true;
        default: return false;
    }
}

constexpr bool is_mul_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        // arithmetic
        case syntax::ast::OperatorExpr::OperatorKind::STAR:
        case syntax::ast::OperatorExpr::OperatorKind::SLASH:
        case syntax::ast::OperatorExpr::OperatorKind::PERCENT:

        // bitwise
        case syntax::ast::OperatorExpr::OperatorKind::AMPERSAND:
        case syntax::ast::OperatorExpr::OperatorKind::BIT_XOR:
        case syntax::ast::OperatorExpr::OperatorKind::BIT_OR:
        case syntax::ast::OperatorExpr::OperatorKind::SHIFT_LEFT:
        case syntax::ast::OperatorExpr::OperatorKind::SHIFT_RIGHT:
            return true;

        default: return false;
    }
}

constexpr bool is_add_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::ast::OperatorExpr::OperatorKind::PLUS:
        case syntax::ast::OperatorExpr::OperatorKind::MINUS:
            return true;
        default: return false;
    }
}

constexpr bool is_compare_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::ast::OperatorExpr::OperatorKind::EQ:
        case syntax::ast::OperatorExpr::OperatorKind::NEQ:
        case syntax::ast::OperatorExpr::OperatorKind::LT:
        case syntax::ast::OperatorExpr::OperatorKind::LE:
        case syntax::ast::OperatorExpr::OperatorKind::GT:
        case syntax::ast::OperatorExpr::OperatorKind::GE:
            return true;
        default: return false;
    }
}

constexpr bool is_logical_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::ast::OperatorExpr::OperatorKind::AND:
        case syntax::ast::OperatorExpr::OperatorKind::OR:
            return true;
        default: return false;
    }
}

constexpr bool is_assign_op(syntax::ast::OperatorExpr::OperatorKind op) noexcept {
    switch(op) {
        case syntax::ast::OperatorExpr::OperatorKind::ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::ADD_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::SUB_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::MUL_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::DIV_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::MOD_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::AND_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::OR_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::XOR_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::SHL_ASSIGN:
        case syntax::ast::OperatorExpr::OperatorKind::SHR_ASSIGN:
            return true;
        default: return false;
    }
}
}
