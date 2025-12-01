#pragma once

#include <memory>
#include <vector>
#include <lang/utils/error.h>
#include <lang/utils/logger.h>
#include <lang/utils/stream.h>
#include <lang/syntax/token.h> 
#include <lang/syntax/keywords.h> 

namespace lang::syntax::lexer
{
    class Lexer
    {
    public: // api
        std::vector<Token> tokenize();
        std::vector<Token> tokenize(utils::InputStream*);

        bool is_success() const noexcept;

        void set_logger_infostream(std::unique_ptr<utils::OutputStream>) noexcept;
        void set_logger_errstream(std::unique_ptr<utils::OutputStream>) noexcept;
        
        Lexer() {init_logger();}
        explicit Lexer(utils::InputStream* _stream):
            stream(_stream)
        {init_logger();}

    private: // vars
        // not owned
        bool success{true};
        std::vector<Token> tokens;
        utils::InputStream* stream{nullptr};
        utils::Logger logger{utils::Logger::LogLevel::ALL};

    private: // inside api
        void breakpoint() noexcept;
        void init_logger() noexcept;

        errors::LexerError stream_null() const;
        errors::LexerError stream_bad() const;
        errors::LexerError reached_eof() const;
        errors::LexerError passed_zero_to_eof() const;

        errors::LexerError word_start_num(SourceLocation) const;
        errors::LexerError not_closed_comment_block(SourceLocation) const;
        errors::LexerError not_closed_string(SourceLocation) const;
        errors::LexerError wrong_number_format(SourceLocation) const;
        errors::LexerError unicode_not_suported(SourceLocation) const;

        void check_stream() const;
        void check_data() const;

        bool is_eof(size_t n = 1) const;
        char peek(size_t offset = 0) const;
        char advance(size_t offset = 0);
        void skip(size_t n = 1);

        static SourceLocation update_pos(SourceLocation, char) noexcept;
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
