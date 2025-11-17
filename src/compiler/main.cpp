#include <print>
#include <stream.h>
#include <tokenizer.h>
#include <utils.h>

#include <iostream>



int main()
{
    lang::StringStream stream(
        ""
        "int main() {\n"
        "\tprintf(\"Hello, \\\"World\\\"!\");\n"
        "\tconst int var = .52;\n"
        "\tfor(int i = 0; i < var; ++i) {\n"
        "\t\tdo_something();"
        "\t}"
        "}"
    );

    // lang::StringStream stream("if int and while for { } .23 423 ++ +=");

    std::print("Tokenizing...");
    lang::Tokenizer tokenizer(stream);
    auto tokens = tokenizer.tokenize(stream);
    std::println("Success!");

    for(auto tok : tokens)
    {
        std::println("token: {}, \t\t\t symbol: {}", lang::token_type_to_string(tok.ty), tok.sym);
    }
}
