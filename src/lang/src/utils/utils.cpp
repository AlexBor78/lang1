#include <memory>
#include <print>
#include <lang/utils/printast.h>
#include <lang/utils/frontend_utils.h>

namespace lang::utils
{
    void print(const frontend::Token& tok) noexcept {
        std::println("TokenType: {} symbol: {}",
            stringify(tok.ty),
            tok.sym
        );
    }

    void print(const std::vector<frontend::Token>& tokens) noexcept {
        for(const auto& tok : tokens) {
            print(tok);
        }
    }

    void print(const ast::BaseNode* ast) noexcept {
        PrintAST printer;
        printer.print(ast);
    }

    void print(const std::vector<std::unique_ptr<ast::BaseNode>>& asts) noexcept {
        PrintAST printer;
        for(const auto& ast : asts) {
            printer.print(ast.get());
            std::println();
        }
    }
}
