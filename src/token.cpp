#include "token.hpp"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

const std::string& Token::get_text() const
{
    return lexeme;
}

std::string Token::to_string() const
{
    return std::to_string(static_cast<int>(type)) + ", lexeme: '" + lexeme + "' , literal: '" + literal_to_string() + "'";
}

std::string Token::literal_to_string() const
{
    switch (type)
    {
        case TokenType::STRING:
            return std::any_cast<std::string>(literal);
        case TokenType::NUMBER:
            return std::to_string(std::any_cast<double>(literal));
        default:
            return "";
    }
}
