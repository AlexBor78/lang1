#include <memory>
#include <print>
#include <stdexcept>
#include <utils/printast.h>
#include <utils/utils.h>

#pragma msg wtf is it even compiling

namespace lang::utils
{
    void print(const Token& tok) {
        std::println("TokenType: {} symbol: {}",
            stringify(tok.ty),
            tok.sym
        );
    }

    void print(const std::vector<Token>& tokens) {
        for(const auto& tok : tokens) {
            print(tok);
        }
    }

    void print(const ast::BaseNode* ast) {
        PrintAST printer;
        printer.print(ast);
    }

    void print(const std::vector<std::unique_ptr<ast::BaseNode>>& asts) {
        PrintAST printer;
        for(const auto& ast : asts) {
            printer.print(ast.get());
            std::println();
        }
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
