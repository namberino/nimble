#include "token.hpp"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

std::string Token::to_string() const
{
    std::string literal_text;

    switch (type)
    {
        case (IDENTIFIER):
            literal_text = lexeme;
            break;
        case (STRING):
            literal_text = std::any_cast<std::string>(literal);
            break;
        case (NUMBER):
            literal_text = std::to_string(std::any_cast<double>(literal));
            break;
        case (TRUE):
            literal_text = "true";
            break;
        case (FALSE):
            literal_text = "false";
            break;
        default:
            literal_text = "nil";
    }

    return "Type: " + ::token_to_string(type) + " | Lexeme: " + lexeme + " | Literal: " + literal_text;
} 

std::string token_to_string(TokenType type)
{
    static const std::string strings[] = {
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "LEFT_BRACKET", "RIGHT_BRACKET",
        "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR", "PERCENT", "COLON",
        "BANG", "BANG_EQUAL",
        "EQUAL", "EQUAL_EQUAL",
        "GREATER", "GREATER_EQUAL",
        "LESS", "LESS_EQUAL",
        "STAR_STAR",
        "IDENTIFIER", "STRING", "NUMBER",
        "AND", "BREAK", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
        "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE", "IMPORT",
        "TOKEN_EOF"
    };

    return strings[static_cast<int>(type)];
}
