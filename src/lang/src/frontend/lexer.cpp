#include <cctype>
#include <stdexcept>
#include <iostream>

#include <lang/frontend/lexer.h>

namespace lang::frontend::lexer
{
    std::vector<Token> Tokenizer::tokenize(utils::InputStream& _stream)
    {
        tokens.clear();
        stream = _stream;
        while(!stream.is_eof())
        {
            try {
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
                
                tokenize_punct();
            }
            catch(const std::exception& e) {
                std::cerr << "error: " << e.what() << std::endl;
                stream.next();
            }
        }        
        return tokens;
    }

    void Tokenizer::add_token(Token tok)
    {
        tokens.emplace_back(tok);
    }

    bool Tokenizer::is_word() const noexcept
    {
        if(isalpha(stream.curr()) || stream.curr() == '_')
            return true;
        return false;
    }

    bool Tokenizer::is_number() const noexcept
    {
        if(isdigit(stream.curr()))
            return true;
        if(stream.curr() == '.' && isdigit(stream.peak()))
            return true;
        return false;
    }

    bool Tokenizer::is_string() const noexcept
    {
        if(stream.curr() == '"')
            return true;
        return false;
    }

    void Tokenizer::tokenize_word()
    {
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

    void Tokenizer::tokenize_punct()
    {
        Position pos = stream.get_pos();
        std::string buf;
        buf = stream.curr();

        pos.length = 2;
        if(!stream.is_eof()) {
            buf += stream.peak();
            if(auto it = keywords.find(buf); it != keywords.end()) {
                stream.next(2);
                add_token({ 
                    .ty = it->second,
                    .pos = pos,
                    .sym = std::move(buf)
                });
                return;
            }
        }

        buf = stream.curr();
        pos.length = 1;
        if(auto it = keywords.find(buf); it != keywords.end()) {
            stream.next();
            add_token({ 
                .ty = it->second,
                .pos = pos,
                .sym = std::move(buf)
            });
            return;
        }
    }

    void Tokenizer::tokenize_number()
    {
        Position pos = stream.get_pos();
        std::string buf;
        bool has_dot{false};
        if(stream.curr() == '.') {
            buf += '0'; 
        }

        while(!stream.is_eof() && is_number()) {
            if(stream.curr() == '.') {
                if(has_dot) throw std::runtime_error("wrong number format");
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

    void Tokenizer::tokenize_string()
    {
        Position pos = stream.get_pos();
        std::string buf;
        buf += stream.next();

        while(!stream.is_eof() && stream.peak() != '"') {
            if(stream.peak() == '\\') {
                buf += tokenize_escape();
                stream.next();
                continue;
            } buf += stream.next();
        } stream.next(2);
        pos.length = stream.get_pos().start - pos.start;

        add_token({
            .ty = TokenType::STRING,
            .pos = pos,
            .sym = std::move(buf)
        });
    }

    char Tokenizer::tokenize_escape()
    {
        stream.next();
        switch (stream.peak()) {
            case('\\'):
                return '\\';
            case ('\"'):
                return '\"';
            case ('n'):
                return '\n';
            case ('t'):
                return '\t';
            default:
                return stream.curr();
        }
    }
}
