#pragma once

#include <vector>

#include <defs.h>
#include <stream.h>
#include <keywords.h>


namespace lang
{
    class Tokenizer
    {
    public:
        std::vector<Token> tokenize(Stream&);
        explicit Tokenizer(Stream& _stream):
            stream(_stream)
        {}

    private:
        std::vector<Token> tokens;
        std::string symbol;
        Stream& stream;

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
