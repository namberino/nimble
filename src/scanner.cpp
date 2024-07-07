#include "scanner.hpp"
#include "error.hpp"

Scanner::Scanner(std::string source) : source(std::move(source))
{
    keywords = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE},
    };
}

std::vector<Token> Scanner::scan_tokens()
{
    while (!is_at_end())
    {
        start = current;
        scan_token();
    }

    tokens.emplace_back(TokenType::TOKEN_EOF, "", std::any{}, line);
    return tokens;
}

bool Scanner::is_at_end() const
{
    return current >= static_cast<int>(source.size());
}

char Scanner::advance()
{
    ++current;
    return source.at(current - 1);
}

bool Scanner::match(char expected)
{
    if (is_at_end())
        return false;

    if (source.at(current) != expected)
        return false;

    ++current;

    return true;
}

char Scanner::peek()
{
    if (is_at_end())
        return '\0';

    return source.at(current);
}

char Scanner::peek_next()
{
    if (current + 1 >= static_cast<int>(source.size()))
        return '\0';

    return source.at(current + 1);
}

void Scanner::add_token(TokenType type, std::any literal)
{
    tokens.emplace_back(type, source.substr(start, current - start), literal, line);
}

void Scanner::add_token(TokenType type)
{
    add_token(type, std::any{});
}

void Scanner::string()
{
    while (peek() != '"' && !is_at_end())
    {
        if (peek() == '\n')
            ++line;

        advance();
    }

    // un-terminated string
    if (is_at_end())
        Error::error(line, "Un-terminated string");

    advance(); // the closing '"'

    // trim surrounding quotes
    add_token(TokenType::STRING, source.substr(start + 1, current - start - 2));
}

void Scanner::number()
{
    while (std::isdigit(peek()))
    {
        advance();
    }

    // look for a decimal point
    if (peek() == '.' && std::isdigit(peek_next()))
    {
        // consume the "."
        advance();

        while (std::isdigit(peek()))
            advance();
    }

    add_token(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
}

void Scanner::identifier()
{
    while (std::isdigit(peek()) || std::isalpha(peek()) || peek() == '_')
        advance();

    const auto text = source.substr(start, current - start);

    if (const auto it = keywords.find(text); it != keywords.end())
        add_token(it->second);
    else
        add_token(TokenType::IDENTIFIER);
}

void Scanner::scan_token()
{
    auto c = advance();
    switch (c)
    {
        case '(':
            add_token(TokenType::LEFT_PAREN);
            break;

        case ')':
            add_token(TokenType::RIGHT_PAREN);
            break;

        case '{':
            add_token(TokenType::LEFT_BRACE);
            break;

        case '}':
            add_token(TokenType::RIGHT_BRACE);
            break;

        case ',':
            add_token(TokenType::COMMA);
            break;

        case '.':
            add_token(TokenType::DOT);
            break;

        case '-':
            add_token(TokenType::MINUS);
            break;

        case '+':
            add_token(TokenType::PLUS);
            break;

        case ';':
            add_token(TokenType::SEMICOLON);
            break;

        case '*':
            add_token(TokenType::STAR);
            break;

        case '!':
            add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;

        case '=':
            add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;

        case '<':
            add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;

        case '>':
            add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;

        case '/':
            if (match('/'))
            {
                // comment goes until the end of the line
                while (peek() != '\n' && !is_at_end())
                    advance();
            }
            else
            {
                add_token(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break;// ignore whitespace

        case '\n':
            line++;
            break;

        case '"':
            string();
            break;

        default:
            if (std::isdigit(c))
                number();
            else if (std::isalpha(c) || c == '_')
                identifier();
            else
                Error::error(line, std::string("Unexpected character: ") + "'" + c + "'");

            break;
    }
}
