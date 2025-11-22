#pragma once

#include <vector>

#include <lang/utils/stream.h>
#include <lang/frontend/keywords.h>
#include <lang/frontend/token.h>


namespace lang::frontend::lexer
{
    class Lexer
    {
    public:
        std::vector<Token> tokenize(utils::InputStream&);
        explicit Lexer(utils::InputStream& _stream):
            stream(_stream)
        {}

    private:
        std::vector<Token> tokens;
        std::string symbol;
        utils::InputStream& stream;

        bool is_word() const noexcept;
        bool is_number() const noexcept;
        bool is_string() const noexcept;
        
        void tokenize_word();
        void tokenize_punct();
        void tokenize_number();
        void tokenize_string();
        char tokenize_escape();

        void add_token(Token);
    };
}
