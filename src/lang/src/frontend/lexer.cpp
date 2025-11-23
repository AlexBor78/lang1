#include <cstddef>
#include <print>
#include <cctype>
#include <iostream>

#include <lang/common.h> //  debug_break()
#include <lang/utils/error.h>
#include <lang/frontend/lexer.h>

namespace lang::frontend::lexer
{
// api
    std::vector<Token> Lexer::tokenize(utils::InputStream* _stream) {
        stream = _stream;
        return tokenize();
    }
    std::vector<Token> Lexer::tokenize() {
        debug_break();
        tokens.clear();
        while(!is_eof()) try {
            // for ConsoleIStream:
            // stop if got ctrl^D on POSIX or ctr+Z on shitdows
            if (peek() == '\x04' || peek() == '\x1A') {
                break;
            }
            if(isspace(peek())) {
                skip_whitespace();
                continue;
            }
            if(is_number()) {
                tokenize_number();
                continue;
            }
            if(is_string()) {
                tokenize_string();
                continue;
            }
            if(is_word()) {
                tokenize_word();
                continue;
            }
            if(is_comment()) {
                process_comment();
                continue;
            }
            
            tokenize_punct();
        } catch(const std::exception& e) {
            std::println(std::cerr, "{}", e.what());
            if(!is_eof()) skip();
            else break;
        }
        // tokens.emplace_back(Token{
        //     .ty = TokenType::END
        // });
        tokens.shrink_to_fit();
        return tokens;
    }

// stream working

    Error Lexer::stream_null() const {
        return Error("lexer error: stream is null");
    }
    Error Lexer::stream_bad() const {
        return Error("lexer error: stream is bad");
    }
    Error Lexer::reached_eof() const {
        return Error("lexer error: reached eof");
    }
    Error Lexer::word_start_num() const {
        return Error("lexer error: word can not starts from number");
    }
    Error Lexer::not_closed_block() const {
        return Error("lexer error: \"/*\" comment block is not closed");
    }

    void Lexer::check_stream() const {
        if(!stream) throw stream_null();
        if(stream->bad()) throw stream_bad();
    }
    void Lexer::check_data() const {
        if(is_eof()) throw reached_eof();
    }
    bool Lexer::is_eof(size_t n) const {
        check_stream();
        return stream->is_eof(n);
    }
    char Lexer::peek(size_t offset) const {
        check_data();
        return stream->peek(offset);
    }
    char Lexer::advance(size_t offset) {
        check_data();
        return stream->advance(offset);
    }
    void Lexer::skip(size_t n) {
        stream->skip(n);
    }
    std::string Lexer::read_word() {
        check_data();
        return stream->read_word();
    }
    void Lexer::skip_whitespace() {
        check_data();
        stream->skip_whitespace();
    }
    SourceLocation Lexer::get_pos() const {
        check_stream();
        return stream->get_pos();
    }

// tokenizing

    void Lexer::add_token(Token tok) {
        tokens.emplace_back(tok);
    }

    bool Lexer::is_word() const noexcept {
        if(is_eof()) return false;
        return isalnum(peek()) || peek() == '_';
    }

    bool Lexer::is_number() const noexcept {
        if(is_eof()) return false;
        return isdigit(peek())
        ||     peek() == '.' && isdigit(peek(1));
    }

    bool Lexer::is_string() const noexcept {
        if(is_eof()) return false;
        return peek() == '"';
    }

    bool Lexer::is_comment() const noexcept {
        if(is_eof() || peek() != '/') return false;
        return peek(1) == '/'
        ||     peek(1) == '*';
    }

    void Lexer::tokenize_word() {
        debug_break();
        SourceLocation pos = get_pos();
        std::string buf = read_word();
        
        pos.length = get_pos().start.index - pos.start.index;
        
        if(auto it = keywords.find(buf); it != keywords.end()) {
            add_token({
                .ty = it->second,
                .pos = pos,
                .sym = std::move(buf)
            }); return;
        }

        add_token({
            .ty = TokenType::IDENTIFIER,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    void Lexer::tokenize_punct() {
        debug_break();
        SourceLocation pos = get_pos();
        std::string buf;

        for(int length = 3; length > 0; --length) {
            if(is_eof(length)) continue;

            pos.length = length;
            buf.clear();

            for(int i = 0; i < length; ++i)
                buf += peek(i);

            if(auto it = keywords.find(buf); it != keywords.end()) {
                skip(length);
                add_token({ 
                    .ty = it->second,
                    .pos = pos,
                    .sym = std::move(buf)
                }); return;
            }
        }
    }

    void Lexer::tokenize_number() {
        debug_break();
        SourceLocation pos = get_pos();
        std::string buf;
        bool has_dot{false};
        if(peek() == '.') {
            buf += '0';
        }

        while(!is_eof() && is_number()) {
            if(peek() == '.') {
                if(has_dot) throw CompileError("wrong number format", pos);
                has_dot = true;
            } buf += advance();
        } pos.length = get_pos().start.index - pos.start.index;

        add_token({
            .ty = TokenType::NUMBER,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    void Lexer::tokenize_string() {
        debug_break();
        SourceLocation pos = get_pos();
        std::string buf;

        skip(); // skip '"'
        while(!is_eof() && peek() != '"') {
            if(peek() == '\\') {
                skip(); // skip '\'
                buf += tokenize_escape();
                continue;
            } buf += advance();
        } skip(); // skip '"'

        pos.length = get_pos().start.index - pos.start.index;

        add_token({
            .ty = TokenType::STRING,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    char Lexer::tokenize_escape() {
        debug_break();
        switch (peek()) {
            case ('\\'):    skip(); return '\\';
            case ('\"'):    skip(); return '\"';
            case ('n'):     skip(); return '\n';
            case ('t'):     skip(); return '\t';
            case ('u'):     throw CompileError("Unicode is not supported", get_pos());
            case ('U'):     throw CompileError("Unicode is not supported", get_pos());
            default:        return peek();
        }
    }

    void Lexer::process_comment(){
        if(peek() != '/') return;
        if(peek(1) == '/') process_comment_line();
        if(peek(1) == '*') process_comment_block();
    }

    void Lexer::process_comment_line() {
        debug_break();
        skip(2); // skip "//"
        while(!is_eof() && peek() != '\n') skip();
    }
    void Lexer::process_comment_block() {
        debug_break();
        skip(2); // skip "/*"
        while(!is_eof(2)) {
            if(peek(0) == '*'
            && peek(1) == '/') {
                skip(2); // sip "*/"
                return;
            }
            skip();
        } throw not_closed_block();
    }
}
