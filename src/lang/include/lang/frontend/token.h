#pragma once

#include <string>
#include <lang/common.h>

namespace lang::frontend
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
            
        // Assignment
        ASSIGN,     // =

        // Arithmetic
        PLUS,       // +
        MINUS,      // -
        STAR,       // *
        SLASH,      // /
        PERCENT,    // %

        // Unary / Address / Dereference
        AMPERSAND,  // &
        BANG,       // !
        TILDE,      // ~

        // Increment / Decrement
        INCREMENT,  // ++
        DECREMENT,  // --

        // Comparison
        EQ,         // ==
        NEQ,        // !=
        LT,         // <
        LE,         // <=
        GT,         // >
        GE,         // >=

        // Logical
        AND,        // &&
        OR,         // ||

        // Bitwise
        BIT_OR,         // |
        BIT_XOR,        // ^
        // BIT_AND = AMPERSAND,        // &
        SHIFT_LEFT,     // <<
        SHIFT_RIGHT,    // >>

        // Compound assignment
        ADD_ASSIGN,       // +=
        SUB_ASSIGN,       // -=
        MUL_ASSIGN,       // *=
        DIV_ASSIGN,       // /=
        MOD_ASSIGN,       // %=

        AND_ASSIGN,       // &=
        OR_ASSIGN,        // |=
        XOR_ASSIGN,       // ^=
        SHL_ASSIGN,       // <<=
        SHR_ASSIGN,       // >>=
        
        // Delimiters
        LPAREN,         // (
        RPAREN,         // )
        LBRACE,         // {
        RBRACE,         // }
        LBRACKET,       // [
        RBRACKET,       // ]
        SEMICOLON,      // ;
        COLON,          // :
        DOUBLECOLON,    // ::
        COMMA,          // ,
        DOT,            // .

        // Declaration keywords
        CONST,
        STRUCT,
        ENUM,
        MODULE,
        NAMESPACE,
        EXTERN,
        
        // Keywords
        IF,
        ELSE,
        FOR,
        WHILE,
        BREAK,
        CONTINUE,
        RETURN,
        IMPORT,
        
        // Special
        END,        // End of file
        ILLEGAL,    // Unknown token
    };

    struct Token
    {
        TokenType ty{TokenType::ILLEGAL};
        SourceLocation pos;
        std::string sym;
    };
}
