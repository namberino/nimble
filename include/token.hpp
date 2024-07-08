#ifndef TOKEN_HPP
#define TOKEN_HPP

#pragma once
#include <string>
#include <any>

enum TokenType
{
    // single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // 1 or 2 characters tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literals tokens
    IDENTIFIER, STRING, NUMBER,

    // keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

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
