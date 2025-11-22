#include <print>
#include <cctype>
#include <iostream>
#include <stdexcept>

#include <lang/utils/error.h>
#include <lang/frontend/lexer.h>

namespace lang::frontend::lexer
{
    std::vector<Token> Lexer::tokenize(utils::InputStream& _stream) {
        tokens.clear();
        stream = _stream;
        while(!stream.is_eof()) try {
            if(isspace(stream.curr())) {
                stream.skip_whitespace();
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
            std::println(std::cerr, "lexer error: {}", e.what());
            stream.next();
        } return tokens;
    }

    void Lexer::add_token(Token tok) {
        tokens.emplace_back(tok);
    }

    bool Lexer::is_word() const noexcept {
        return isalpha(stream.curr()) || stream.curr() == '_';
    }

    bool Lexer::is_number() const noexcept {
        return isdigit(stream.curr())
        ||     stream.curr() == '.' && isdigit(stream.peak());
    }

    bool Lexer::is_string() const noexcept {
        return stream.curr() == '"';
    }

    bool Lexer::is_comment() const noexcept {
        if(stream.curr() != '/') return false;
        return stream.peak() == '/'
        ||     stream.peak() == '*';
    }

    void Lexer::tokenize_word()     {
        Position pos = stream.get_pos();
        std::string buf;

        buf += stream.curr();
        while(!stream.is_eof() && (isalnum(stream.peak()) || stream.peak() == '_'))
            buf += stream.next();
        
        stream.next();
        pos.length = stream.get_pos().start - pos.start;
        
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
        Position pos = stream.get_pos();
        std::string buf;

        for(int length = 3; length > 0; --length) {
            pos.length = length;
            buf.clear();

            for(int i = 0; i < length; ++i)
                buf += stream.peak(i);

            if(auto it = keywords.find(buf); it != keywords.end()) {
                stream.next(length);
                add_token({ 
                    .ty = it->second,
                    .pos = pos,
                    .sym = std::move(buf)
                });
                return;
            }
        }
    }

    void Lexer::tokenize_number() {
        Position pos = stream.get_pos();
        std::string buf;
        bool has_dot{false};
        if(stream.curr() == '.') {
            buf += '0';
        }

        while(!stream.is_eof() && is_number()) {
            if(stream.curr() == '.') {
                if(has_dot) throw CompileError("wrong number format", pos);
                has_dot = true;
            } buf += stream.curr();
            stream.next();
        } pos.length = stream.get_pos().start - pos.start;

        add_token({
            .ty = TokenType::NUMBER,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    void Lexer::tokenize_string() {
        Position pos = stream.get_pos();
        std::string buf;
        buf += stream.next();

        while(!stream.is_eof() && stream.peak() != '"') {
            if(stream.peak() == '\\') {
                stream.next(); // skip previous char
                buf += tokenize_escape();
                stream.next(); // skip  '\'
                continue;
            } buf += stream.next();
        } stream.next(2); // skip curr tok(char) and next('"')

        pos.length = stream.get_pos().start - pos.start;

        add_token({
            .ty = TokenType::STRING,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    char Lexer::tokenize_escape() {
        switch (stream.peak()) {
            case ('\\'):    return '\\';
            case ('\"'):    return '\"';
            case ('n'):     return '\n';
            case ('t'):     return '\t';
            case ('u'):     throw CompileError("Unicode is not supported", stream.get_pos());
            case ('U'):     throw CompileError("Unicode is not supported", stream.get_pos());
            default:        return stream.curr();
        }
    }

    void Lexer::process_comment(){
        if(stream.curr() != '/') return;
        if(stream.peak() == '/') process_comment_line();
        if(stream.peak() == '*') process_comment_block();
    }

    void Lexer::process_comment_line() {
        stream.read_line();
    }
    void Lexer::process_comment_block() {
        stream.next(2);
        while(!stream.is_eof()
        &&    stream.curr() != '*'
        &&    stream.peak() != '/') {
            stream.next();
        } stream.next(2);
    }
}
