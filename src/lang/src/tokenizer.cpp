#include <cctype>
#include <print>
#include <stdexcept>
#include <iostream>

#include <tokenizer.h>

namespace lang
{
    std::vector<Token> Tokenizer::tokenize(Stream& _stream)
    {
        tokens.clear();
        stream = _stream;
        while(!stream.is_eof())
        {
            try {
                if(isspace(stream.curr())) {
                    // std::println("space");
                    stream.skip_whitespace();
                    continue;
                }

                if(is_number()) {
                    // std::println("number");
                    tokenize_number();
                    continue;
                }
                if(is_string()) {
                    // std::println("string");
                    tokenize_string();
                    continue;
                }
                if(is_word()) {
                    // std::println("word");
                    tokenize_word();
                    continue;
                }
                
                // std::println("punct");
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
        }

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
