#include <print>

#include <lang/utils/stream.h>
#include <lang/frontend/lexer.h>
#include <lang/frontend/parser.h>

// #include <lang/semantic/typesystem.h>
// #include <lang/semantic/program.h>

#include <lang/utils/frontend_utils.h>
#include <lang/utils/ast_utils.h>

#include <lang/ast/stmt.h>
#include <lang/ast/expr.h>

int main()
{
    std::string_view path = "main.lang";
    lang::utils::FileIStream istream(path);

    std::print("Tokenizing...");
    lang::frontend::lexer::Lexer tokenizer(istream);
    auto tokens = tokenizer.tokenize(istream);
    std::println("Success!");
    lang::utils::print(tokens);

    std::println("\n\n\n");

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
