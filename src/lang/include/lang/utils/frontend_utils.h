#pragma once

#include <vector>
#include <lang/frontend/token.h>

namespace lang::utils
{
    // enum types to string
    constexpr const char* stringify(frontend::TokenType) noexcept;

    // print token or vector of tokens
    void print(const std::vector<frontend::Token>&) noexcept;
    void print(const frontend::Token&) noexcept;


    constexpr bool is_declarator(frontend::TokenType) noexcept;
    constexpr bool is_operator(frontend::TokenType) noexcept;
    constexpr bool is_literal(frontend::TokenType) noexcept;
    // constexpr bool is_(frontend::TokenType) noexcept;


// constexpr implementations ---------------------------------------------------

    constexpr const char* stringify(frontend::TokenType ty) noexcept {
        switch(ty) {
            // Identifiers and literals
            case frontend::TokenType::IDENTIFIER: return "IDENTIFIER";
            case frontend::TokenType::NUMBER:     return "NUMBER";
            case frontend::TokenType::STRING:     return "STRING";

            // Literal values
            case frontend::TokenType::TRUE:       return "TRUE";
            case frontend::TokenType::FALSE:      return "FALSE";

            // Operators
            case frontend::TokenType::ASSIGN:       return "ASSIGN";
            case frontend::TokenType::PLUS:         return "PLUS";
            case frontend::TokenType::MINUS:        return "MINUS";
            case frontend::TokenType::STAR:         return "STAR";
            case frontend::TokenType::SLASH:        return "SLASH";
            case frontend::TokenType::PERCENT:      return "PERCENT";
            case frontend::TokenType::AMPERSAND:    return "AMPERSAND";
            case frontend::TokenType::EQ:           return "EQ";
            case frontend::TokenType::NEQ:          return "NEQ";
            case frontend::TokenType::LT:           return "LT";
            case frontend::TokenType::LE:           return "LE";
            case frontend::TokenType::GT:           return "GT";
            case frontend::TokenType::GE:           return "GE";
            case frontend::TokenType::BANG:         return "BANG";
            case frontend::TokenType::AND:          return "AND";
            case frontend::TokenType::OR:           return "OR";
            case frontend::TokenType::INCREMENT:    return "INCREMENT";
            case frontend::TokenType::DECREMENT:    return "DECREMENT";
            case frontend::TokenType::TILDE:        return "TILDE";
            case frontend::TokenType::BIT_OR:       return "BIT_OR";
            // case frontend::TokenType::BIT_AND:      return "BIT_AND"; AMPERSAND
            case frontend::TokenType::BIT_XOR:      return "BIT_XOR";
            case frontend::TokenType::SHIFT_LEFT:   return "SHIFT_LEFT";
            case frontend::TokenType::SHIFT_RIGHT:  return "SHIFT_RIGHT";

            // Compound assignments
            case frontend::TokenType::ADD_ASSIGN:   return "ADD_ASSIGN";
            case frontend::TokenType::SUB_ASSIGN:   return "SUB_ASSIGN";
            case frontend::TokenType::MUL_ASSIGN:   return "MUL_ASSIGN";
            case frontend::TokenType::DIV_ASSIGN:   return "DIV_ASSIGN";
            case frontend::TokenType::MOD_ASSIGN:   return "MOD_ASSIGN";
            case frontend::TokenType::AND_ASSIGN:   return "AND_ASSIGN";
            case frontend::TokenType::OR_ASSIGN:    return "OR_ASSIGN";
            case frontend::TokenType::XOR_ASSIGN:   return "XOR_ASSIGN";
            case frontend::TokenType::SHL_ASSIGN:   return "SHL_ASSIGN";
            case frontend::TokenType::SHR_ASSIGN:   return "SHR_ASSIGN";


            // Delimiters
            case frontend::TokenType::LPAREN:       return "LPAREN";
            case frontend::TokenType::RPAREN:       return "RPAREN";
            case frontend::TokenType::LBRACE:       return "LBRACE";
            case frontend::TokenType::RBRACE:       return "RBRACE";
            case frontend::TokenType::LBRACKET:     return "LBRACKET";
            case frontend::TokenType::RBRACKET:     return "RBRACKET";
            case frontend::TokenType::SEMICOLON:    return "SEMICOLON";
            case frontend::TokenType::COLON:        return "COLON";
            case frontend::TokenType::DOUBLECOLON:  return "DOUBLECOLON";
            case frontend::TokenType::COMMA:        return "COMMA";
            case frontend::TokenType::DOT:          return "DOT";

            // Declaration keywords
            case frontend::TokenType::CONST:        return "CONST";
            case frontend::TokenType::STRUCT:       return "STRUCT";
            case frontend::TokenType::ENUM:         return "ENUM";

            // Keywords
            case frontend::TokenType::IF:           return "IF";
            case frontend::TokenType::ELSE:         return "ELSE";
            case frontend::TokenType::FOR:          return "FOR";
            case frontend::TokenType::WHILE:        return "WHILE";
            case frontend::TokenType::BREAK:        return "BREAK";
            case frontend::TokenType::CONTINUE:     return "CONTINUE";
            case frontend::TokenType::RETURN:       return "RETURN";
            case frontend::TokenType::IMPORT:       return "IMPORT";
            case frontend::TokenType::MODULE:       return "MODULE";
            case frontend::TokenType::NAMESPACE:    return "NAMESPACE";
            case frontend::TokenType::EXTERN:       return "EXTERN";
            
            // Special
            case frontend::TokenType::END:        return "END";
            case frontend::TokenType::ILLEGAL:    return "ILLEGAL";
        } return "UNKNOWN";
    }

    constexpr bool is_operator(frontend::TokenType tok) noexcept {
        return static_cast<int>(tok) > static_cast<int>(frontend::TokenType::FALSE) 
        &&     static_cast<int>(tok) < static_cast<int>(frontend::TokenType::LPAREN);
    }

    constexpr bool is_literal(frontend::TokenType tok) noexcept {
        return static_cast<int>(tok) > static_cast<int>(frontend::TokenType::IDENTIFIER) 
        &&     static_cast<int>(tok) < static_cast<int>(frontend::TokenType::ASSIGN);
    }

    constexpr bool is_declarator(frontend::TokenType tok) noexcept {
        return  static_cast<int>(tok) > static_cast<int>(frontend::TokenType::DOT)
        &&      static_cast<int>(tok) < static_cast<int>(frontend::TokenType::IF)
        ||      tok == frontend::TokenType::IDENTIFIER;
    }
}
