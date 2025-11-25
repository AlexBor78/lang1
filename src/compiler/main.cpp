#include <print>

#include <lang/utils/istream.h>
#include <lang/utils/ostream.h>
#include <lang/frontend/lexer.h>
#include <lang/frontend/parser.h>

// to print tokens/ast
#include <lang/utils/ast_utils.h>
#include <lang/utils/frontend_utils.h>

int main() {
    lang::debug_break();
    std::string_view path = "main.lang";

    std::println("Tokenizing...");

    // ConsoleIStream is working
    // std::println("press ctr^D to stop");
    // lang::utils::ConsoleIStream istream;

    lang::utils::FileIStream istream(path);
    lang::frontend::lexer::Lexer lexer(&istream);
    auto tokens = lexer.tokenize();
    if(!lexer.is_success()) {
        std::println("error while tokenizing");
        return -1;
    }

    std::println("Success!");
    lang::utils::print(tokens);

    std::println("\n\n\n");
    std::println("Parsing...");
    lang::frontend::parser::Parser parser;
    if(parser.load_and_parse(tokens)) {
        std::println("error while parsing file");
        auto e = parser.get_error();
        std::println("{}", e.what());
    } else std::println("success parsed file {}", path);

    auto ast = parser.get_program();
    std::println("\n\n\n");
    lang::utils::print(ast);

    std::println("end");
}
