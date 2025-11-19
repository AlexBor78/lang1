#include <memory>
#include <print>
#include <stream.h>
#include <tokenizer.h>
#include <utils.h>

#include <ast/stmt.h>
#include <ast/expr.h>

int main()
{
    lang::FileStream stream("main.lang");

    std::print("Tokenizing...");
    lang::Tokenizer tokenizer(stream);
    auto tokens = tokenizer.tokenize(stream);
    std::println("Success!");

    lang::utils::print(tokens);

    auto root = std::make_unique<lang::ast::StringLiteral>("literal btw");   
}
