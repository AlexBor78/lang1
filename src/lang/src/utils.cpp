#include "ast/expr.h"
#include <print>
#include <stdexcept>
#include <utils.h>

namespace lang::utils
{
    constexpr const char* stringify(TokenType ty)
    {
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
            case TokenType::COLON:      return "COLON";
            case TokenType::DOUBLECOLON:return "DOUBLECOLON";
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
            case TokenType::IMPORT:     return "IMPORT";
            case TokenType::MODULE:     return "MODULE";
            case TokenType::NAMESPACE:  return "NAMESPACE";
            
            // Special
            case TokenType::END:        return "END";
            case TokenType::ILLEGAL:    return "ILLEGAL";
        } return "UNKNOWN";
    }

    void print(const Token& tok) {
        std::println("TokenType: {}, symbol {}",
            stringify(tok.ty),
            tok.sym
        );
    }

    void print(const std::vector<Token>& tokens) {
        for(const auto& tok : tokens) {
            print(tok);
        }
    }

    constexpr const char* stringify(ast::OperatorExpr::OperatorKind op) {
        switch (op) {
            case ast::OperatorExpr::OperatorKind::ASSIGN:     return "ASSIGN";
            case ast::OperatorExpr::OperatorKind::PLUS:       return "PLUS";
            case ast::OperatorExpr::OperatorKind::MINUS:      return "MINUS";
            case ast::OperatorExpr::OperatorKind::STAR:       return "STAR";
            case ast::OperatorExpr::OperatorKind::SLASH:      return "SLASH";
            case ast::OperatorExpr::OperatorKind::PERCENT:    return "PERCENT";
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
        } return "UNKNOWN";
    }

    ast::OperatorExpr::OperatorKind token_to_op(TokenType tok) {
        switch (tok) {
            case TokenType::ASSIGN:     return ast::OperatorExpr::OperatorKind::ASSIGN;
            case TokenType::PLUS:       return ast::OperatorExpr::OperatorKind::PLUS;
            case TokenType::MINUS:      return ast::OperatorExpr::OperatorKind::MINUS;
            case TokenType::STAR:       return ast::OperatorExpr::OperatorKind::STAR;
            case TokenType::SLASH:      return ast::OperatorExpr::OperatorKind::SLASH;
            case TokenType::PERCENT:    return ast::OperatorExpr::OperatorKind::PERCENT;
            case TokenType::EQ:         return ast::OperatorExpr::OperatorKind::EQ;
            case TokenType::NEQ:        return ast::OperatorExpr::OperatorKind::NEQ;
            case TokenType::LT:         return ast::OperatorExpr::OperatorKind::LT;
            case TokenType::LE:         return ast::OperatorExpr::OperatorKind::LE;
            case TokenType::GT:         return ast::OperatorExpr::OperatorKind::GT;
            case TokenType::GE:         return ast::OperatorExpr::OperatorKind::GE;
            case TokenType::BANG:       return ast::OperatorExpr::OperatorKind::BANG;
            case TokenType::AND:        return ast::OperatorExpr::OperatorKind::AND;
            case TokenType::OR:         return ast::OperatorExpr::OperatorKind::OR;
            case TokenType::INCREMENT:  return ast::OperatorExpr::OperatorKind::INCREMENT;
            case TokenType::DECREMENT:  return ast::OperatorExpr::OperatorKind::DECREMENT;
            default: throw std::runtime_error("wrong TokenType to OperatorKind");
        }
    }
}
