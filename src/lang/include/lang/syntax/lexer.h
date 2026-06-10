#pragma once

#include <memory>
#include <vector>
#include <common/common.h>
#include <common/utils/logger.h>
#include <lang/utils/diagnostic.h> 
#include <common/streams/stream.h>
#include <lang/syntax/token.h> 
#include <lang/syntax/keywords.h> 
#include <lang/syntax/source_file.h>

namespace lang::syntax::lexer
{
    class Lexer
    {
    public: // api
        std::vector<Token> tokenize();
//        std::vector<Token> tokenize(common::streams::InputStream*);

        bool is_success() const noexcept;

        void set_logger_infostream(std::unique_ptr<common::streams::OutputStream>) noexcept;
        void set_logger_errstream(std::unique_ptr<common::streams::OutputStream>) noexcept;
        
        explicit Lexer(
					std::string_view _source 
				,	common::utils::Logger* _logger
				):source(_source)
				,	logger(_logger)
        {init_logger();}

    private: // vars
//        common::streams::InputStream* stream{nullptr};
				std::string_view source;
				size_t cursor{0};
				common::SourceLocation current_pos = {};

//				common::memory::ArenaAloc* arena;

        bool success{true};
        std::vector<Token> tokens;
        common::utils::Logger* logger;

    private: // inside api
        void breakpoint() noexcept;
        void init_logger() noexcept;

        diagnostic::LexerError stream_null() const;
        diagnostic::LexerError stream_bad() const;
        diagnostic::LexerError reached_eof() const;
        diagnostic::LexerError passed_zero_to_eof() const;

        diagnostic::LexerError unexpected_token(common::SourceLocation) const;
        diagnostic::LexerError word_start_num(common::SourceLocation) const;
        diagnostic::LexerError not_closed_comment_block(common::SourceLocation) const;
        diagnostic::LexerError not_closed_string(common::SourceLocation) const;
        diagnostic::LexerError wrong_number_format(common::SourceLocation) const;
        diagnostic::LexerError unicode_not_suported(common::SourceLocation) const;

        void check_stream() const;
        void check_data() const;

				void load_file();
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
