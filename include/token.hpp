//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#ifndef TOKEN_HPP
#define TOKEN_HPP

#pragma once
#include <string>
#include <any>

enum TokenType
{
    // single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, PERCENT, COLON,

    // 1 or 2 characters tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    STAR_STAR,

    // literals tokens
    IDENTIFIER, STRING, NUMBER,

    // keywords
    AND, BREAK, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, IMPORT,

    // end of file
    TOKEN_EOF
};

extern std::string token_to_string(TokenType type);

class Token
{
    public:
        TokenType type;
        std::string lexeme;
        std::any literal;
        int line;

        Token(TokenType type, std::string lexeme, std::any literal, int line);
        std::string to_string() const;
};

#endif
