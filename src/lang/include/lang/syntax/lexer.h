#pragma once

#include <memory>
#include <vector>
#include <common/common.h>
#include <common/utils/logger.h>
#include <lang/utils/diagnostic.h> 
#include <common/streams/stream.h>
#include <lang/syntax/token.h> 
#include <lang/syntax/keywords.h> 

namespace lang::syntax::lexer
{
    class Lexer
    {
    public: // api
        std::vector<Token> tokenize();
        std::vector<Token> tokenize(common::streams::InputStream*);

        bool is_success() const noexcept;

        void set_logger_infostream(std::unique_ptr<common::streams::OutputStream>) noexcept;
        void set_logger_errstream(std::unique_ptr<common::streams::OutputStream>) noexcept;
        
        Lexer() {init_logger();}
        explicit Lexer(common::streams::InputStream* _stream):
            stream(_stream)
        {init_logger();}

    private: // vars
        // not owned
        bool success{true};
        std::vector<Token> tokens;
        common::streams::InputStream* stream{nullptr};
        common::utils::Logger logger{common::utils::Logger::LogLevel::ALL};

    private: // inside api
        void breakpoint() noexcept;
        void init_logger() noexcept;

        errors::LexerError stream_null() const;
        errors::LexerError stream_bad() const;
        errors::LexerError reached_eof() const;
        errors::LexerError passed_zero_to_eof() const;

        errors::LexerError word_start_num(common::SourceLocation) const;
        errors::LexerError not_closed_comment_block(common::SourceLocation) const;
        errors::LexerError not_closed_string(common::SourceLocation) const;
        errors::LexerError wrong_number_format(common::SourceLocation) const;
        errors::LexerError unicode_not_suported(common::SourceLocation) const;

        void check_stream() const;
        void check_data() const;

        bool is_eof(size_t n = 1) const;
        char peek(size_t offset = 0) const;
        char advance(size_t offset = 0);
        void skip(size_t n = 1);

        static common::SourceLocation update_pos(common::SourceLocation, char) noexcept;
        common::SourceLocation get_pos() const;
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
