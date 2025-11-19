#pragma once

#include <string_view>
#include <unordered_map>
#include <defs.h>

namespace lang
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
        
        // Delimiters  
        {"(", TokenType::LPAREN},
        {")", TokenType::RPAREN},
        {"{", TokenType::LBRACE},
        {"}", TokenType::RBRACE},
        {"[", TokenType::LBRACKET},
        {"]", TokenType::RBRACKET},
        {";", TokenType::SEMICOLON},
        {"::", TokenType::DOUBLECOLON},
        {":", TokenType::COLON},
        {",", TokenType::COMMA},
        {".", TokenType::DOT},

        // Declaration 
        {"const", TokenType::CONST},
        {"struct", TokenType::STRUCT},
        {"enum", TokenType::ENUM},

        // Keywords
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"for", TokenType::FOR},
        {"while", TokenType::WHILE},
        {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE},
        {"return", TokenType::RETURN},
        {"import", TokenType::IMPORT},
        {"module", TokenType::MODULE},
        {"namespace", TokenType::NAMESPACE}
    };
}
