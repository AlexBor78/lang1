#pragma once

#include <vector>

#include <lang/utils/error.h>
#include <lang/utils/stream.h>
#include <lang/frontend/token.h> 
#include <lang/frontend/keywords.h> 


namespace lang::frontend::lexer
{
    class Lexer
    {
    public: // api
        std::vector<Token> tokenize();
        std::vector<Token> tokenize(utils::InputStream*);
        explicit Lexer(utils::InputStream* _stream):
            stream(_stream)
        {}

    private: // vars
        // not owned
        utils::InputStream* stream{nullptr};
        std::vector<Token> tokens;

    private: // stream working

        Error stream_null() const;
        Error stream_bad() const;
        Error reached_eof() const;
        Error word_start_num() const;
        Error not_closed_block() const;

        void check_stream() const;
        void check_data() const;

        bool is_eof(size_t n = 1) const;
        char peek(size_t offset = 0) const;
        char advance(size_t offset = 0);
        void skip(size_t n = 1);

        SourceLocation get_pos() const;
        std::string read_word();
        void skip_whitespace();

    private: // tokenizing

        bool is_word() const noexcept;
        bool is_number() const noexcept;
        bool is_string() const noexcept;
        bool is_comment() const noexcept;
        
        void tokenize_word();
        void tokenize_punct();
        void tokenize_number();
        void tokenize_string();
        char tokenize_escape();
        
        void process_comment();
        void process_comment_line();
        void process_comment_block();

        void add_token(Token);
    };
}
