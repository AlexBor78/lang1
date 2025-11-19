#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace lang
{
    enum class TokenType
    {
        // Identifiers and literals
        IDENTIFIER,
        NUMBER,
        STRING,

        // Literal values
        TRUE,
        FALSE,
        
        // Operators
        ASSIGN,     // =
        PLUS,       // +
        MINUS,      // -
        STAR,       // *
        SLASH,      // /
        PERCENT,    // %
        EQ,         // ==
        NEQ,        // !=
        LT,         // <
        LE,         // <=
        GT,         // >
        GE,         // >=
        BANG,       // !
        AND,        // &&
        OR,         // ||
        INCREMENT,  // ++
        DECREMENT,  // --
        
        // Delimiters
        LPAREN,     // (
        RPAREN,     // )
        LBRACE,     // {
        RBRACE,     // }
        LBRACKET,   // [
        RBRACKET,   // ]
        SEMICOLON,  // ;
        COLON,      // :
        DOUBLECOLON,// ::
        COMMA,      // ,
        DOT,        // .

        // Declaration keywords
        CONST,
        STRUCT,
        ENUM,
        
        // Keywords
        IF,
        ELSE,
        FOR,
        WHILE,
        BREAK,
        CONTINUE,
        RETURN,
        IMPORT,
        MODULE,
        NAMESPACE,
        
        // Special
        END,        // End of file
        ILLEGAL,    // Unknown token
    };

    struct Position
    {
        std::string_view path;
        size_t line{0}; // unused
        size_t column{0}; // unused
        size_t start{0}; // character pos
        size_t length{1};
    };

    struct Token
    {
        TokenType ty{TokenType::ILLEGAL};
        Position pos;
        std::string sym;
    };

}
