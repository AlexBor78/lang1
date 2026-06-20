#include <memory>
#include <print>
#include <lang/utils/printast.h>
#include <lang/utils/syntax_utils.h>

namespace lang::utils
{
    void print(const syntax::Token& tok) noexcept {
        std::println("TokenType: {} symbol: {}",
            stringify(tok.ty),
            tok.sym
        );
    }

    void print(const std::vector<syntax::Token>& tokens) noexcept {
        for(const auto& tok : tokens) {
            print(tok);
        }
    }

    void print(const syntax::ast::BaseNode* ast) noexcept {
        PrintAST printer;
        printer.print(ast);
    }

    void print(const std::vector<std::unique_ptr<syntax::ast::BaseNode>>& asts) noexcept {
        PrintAST printer;
        for(const auto& ast : asts) {
            printer.print(ast.get());
            std::println();
        }
    }
}
