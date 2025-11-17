#pragma once

#include <defs.h>

namespace lang
{
    constexpr const char* token_type_to_string(TokenType ty) {
    switch(ty) {
        // Identifiers and literals
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::STRING:     return "STRING";

        // Literal values
        case TokenType::TRUE:       return "TRUE";
        case TokenType::FALSE:      return "FALSE";

        // Operators
        case TokenType::ASSIGN:     return "ASSIGN";
        case TokenType::PLUS:       return "PLUS";
        case TokenType::MINUS:      return "MINUS";
        case TokenType::STAR:       return "STAR";
        case TokenType::SLASH:      return "SLASH";
        case TokenType::PERCENT:    return "PERCENT";
        case TokenType::EQ:         return "EQ";
        case TokenType::NEQ:        return "NEQ";
        case TokenType::LT:         return "LT";
        case TokenType::LE:         return "LE";
        case TokenType::GT:         return "GT";
        case TokenType::GE:         return "GE";
        case TokenType::BANG:       return "BANG";
        case TokenType::AND:        return "AND";
        case TokenType::OR:         return "OR";
        case TokenType::INCREMENT:  return "INCREMENT";
        case TokenType::DECREMENT:  return "DECREMENT";

        // Delimiters
        case TokenType::LPAREN:     return "LPAREN";
        case TokenType::RPAREN:     return "RPAREN";
        case TokenType::LBRACE:     return "LBRACE";
        case TokenType::RBRACE:     return "RBRACE";
        case TokenType::LBRACKET:   return "LBRACKET";
        case TokenType::RBRACKET:   return "RBRACKET";
        case TokenType::SEMICOLON:  return "SEMICOLON";
        case TokenType::COMMA:      return "COMMA";
        case TokenType::DOT:        return "DOT";

        // Declaration keywords
        case TokenType::CONST:      return "CONST";
        case TokenType::STRUCT:     return "STRUCT";
        case TokenType::ENUM:       return "ENUM";

        // Keywords
        case TokenType::IF:         return "IF";
        case TokenType::ELSE:       return "ELSE";
        case TokenType::FOR:        return "FOR";
        case TokenType::WHILE:      return "WHILE";
        case TokenType::BREAK:      return "BREAK";
        case TokenType::CONTINUE:   return "CONTINUE";
        case TokenType::RETURN:     return "RETURN";

        // Special
        case TokenType::END:        return "END";
        case TokenType::ILLEGAL:    return "ILLEGAL";
    }
    return "UNKNOWN";
}

}