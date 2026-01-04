#pragma once

#include <string_view>
#include <unordered_map>
#include <lang/syntax/token.h>

namespace lang::syntax::lexer
{
    inline std::unordered_map<std::string_view, TokenType> keywords =
    {
        // values
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},

        // Operators
        {"=", TokenType::ASSIGN},
        {"+", TokenType::PLUS},
        {"-", TokenType::MINUS},
        {"*", TokenType::STAR},
        {"/", TokenType::SLASH},
        {"%", TokenType::PERCENT},
        {"&", TokenType::AMPERSAND},
        {"==", TokenType::EQ},
        {"!=", TokenType::NEQ},
        {"<", TokenType::LT},
        {"<=", TokenType::LE},
        {">", TokenType::GT},
        {">=", TokenType::GE},
        {"!", TokenType::BANG},
        {"&&", TokenType::AND},
        {"||", TokenType::OR},
        {"||", TokenType::OR},
        {"++", TokenType::INCREMENT},
        {"--", TokenType::DECREMENT},
        {"~",  TokenType::TILDE},
        {"|",  TokenType::BIT_OR},
        // {"&",  TokenType::BIT_AND}, = AMPERSAND
        {"^",  TokenType::BIT_XOR},
        {"<<", TokenType::SHIFT_LEFT},
        {">>", TokenType::SHIFT_RIGHT},

        // Compound assignments
        {"+=",  TokenType::ADD_ASSIGN},
        {"-=",  TokenType::SUB_ASSIGN},
        {"*=",  TokenType::MUL_ASSIGN},
        {"/=",  TokenType::DIV_ASSIGN},
        {"%=",  TokenType::MOD_ASSIGN},
        {"&=",  TokenType::AND_ASSIGN},
        {"|=",  TokenType::OR_ASSIGN},
        {"^=",  TokenType::XOR_ASSIGN},
        {"<<=", TokenType::SHL_ASSIGN},
        {">>=", TokenType::SHR_ASSIGN},
        
        // Delimiters  
        {"(", TokenType::LPAREN},
        {")", TokenType::RPAREN},
        {"{", TokenType::LBRACE},
        {"}", TokenType::RBRACE},
        {"[", TokenType::LBRACKET},
        {"]", TokenType::RBRACKET},
        {"<-", TokenType::LARROW},
        {"->", TokenType::RARROW},
        {";", TokenType::SEMICOLON},
        {"::", TokenType::DOUBLECOLON},
        {":", TokenType::COLON},
        {",", TokenType::COMMA},
        {".", TokenType::DOT},

        // Declaration 
        {"export", TokenType::EXPORT},
        {"extern", TokenType::EXTERN},
        {"fn", TokenType::FN},
        {"const", TokenType::CONST},
        {"mutable", TokenType::MUTABLE},
        {"struct", TokenType::STRUCT},
        {"alisa", TokenType::ALIAS},
        {"enum", TokenType::ENUM},

        // Other keywords
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"for", TokenType::FOR},
        {"while", TokenType::WHILE},
        {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE},
        {"return", TokenType::RETURN},
        {"import", TokenType::IMPORT},
        {"stack", TokenType::STACK}
    };
}
