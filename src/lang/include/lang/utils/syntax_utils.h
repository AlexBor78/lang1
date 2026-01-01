#pragma once

#include <vector>
#include <lang/syntax/token.h>

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
        return  static_cast<int>(tok) > static_cast<int>(syntax::TokenType::DOT)
        &&      static_cast<int>(tok) < static_cast<int>(syntax::TokenType::IF)
        ||      tok == syntax::TokenType::IDENTIFIER
        ||      tok == syntax::TokenType::AMPERSAND
        ||      tok == syntax::TokenType::STAR;
    }
}
