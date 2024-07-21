#include "lexer.hpp"
#include "error.hpp"

Lexer::Lexer(std::string source) : source(source) {}

std::vector<Token> Lexer::scan_tokens()
{
    // main loop
    while (!is_at_end())
    {
        start = current;
        scan_token();
    }

    // end of line token
    tokens.emplace_back(TokenType::TOKEN_EOF, "", nullptr, line);
    return tokens;
}

bool Lexer::is_at_end() const
{
    // check if is at the end of the source code
    return current >= static_cast<int>(source.length());
}

char Lexer::advance()
{
    // consume next character in the source file and return it
    char c = source[current];

    // utf char length handling
    if ((c & 0x80) == 0) // 1 byte
        current++;
    else if ((c & 0xE0) == 0xC0) // 2 bytes
        current += 2;
    else if ((c & 0xF0) == 0xE0) // 3 bytes
        current += 3;
    else if ((c & 0xF8) == 0xF0) // 4 bytes
        current += 4;
    
    return c;
}

bool Lexer::match(char expected)
{
    // since advance() uses post increment, the 'current' variable
    // actually points to the character after the current character
    // that's why this function works
    if (is_at_end())
        return false;

    if (source[current] != expected)
        return false;

    ++current;

    return true;
}

// this is like match but it doesn't consume the character
char Lexer::peek()
{
    if (is_at_end())
        return '\0';

    // this also works because current is pointing
    // to the next character, not the current character
    return source[current];
}

char Lexer::peek_next()
{
    // this is just peek but for characters after that
    // comes after the characters that peek() returns
    if (current + 1 >= static_cast<int>(source.length()))
        return '\0';

    return source[current + 1];
}

void Lexer::add_token(TokenType type, std::any literal)
{
    // grab the text and data of the current lexeme and creates a new token for it
    tokens.emplace_back(type, source.substr(start, current - start), literal, line);
}

void Lexer::add_token(TokenType type)
{
    add_token(type, nullptr);
}

void Lexer::string()
{
    // while not reached second quote or end of source code
    while (peek() != '"' && !is_at_end())
    {
        if (peek() == '\n')
            ++line;

        advance();
    }

    // unterminated string
    if (is_at_end())
    {
        Error::error(line, "Unterminated string");
        return;
    }

    advance(); // consume the closing '"'

    // trim surrounding quotes with substring
    add_token(TokenType::STRING, source.substr(start + 1, current - start - 2));
}

void Lexer::number()
{
    // while the next character to consume is a digit
    while (std::isdigit(peek()))
    {
        advance(); // consume
    }

    // look for a decimal point
    if (peek() == '.' && std::isdigit(peek_next()))
    {
        advance(); // move pass the decimal point

        // continue consuming the digits
        while (std::isdigit(peek()))
            advance();
    }

    add_token(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
}

void Lexer::identifier()
{
    // check for utf-8 characters
    while (std::isalnum(peek()) || peek() == '_' || (peek() & 0x80) != 0)
        advance(); // consume

    const auto text = source.substr(start, current - start); // the word currently being evaluated
    const auto match = keywords.find(text); // find identifier token

    if (match != keywords.end()) // if found a match
        add_token(match->second);
    else
        add_token(TokenType::IDENTIFIER); // default to returning the identifier token type
}

void Lexer::scan_token()
{
    auto c = advance();

    // character recognization
    switch (c)
    {
        // single char token recognition
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case '[': add_token(TokenType::LEFT_BRACKET); break;
        case ']': add_token(TokenType::RIGHT_BRACKET); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(TokenType::MINUS); break;
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '%': add_token(TokenType::PERCENT); break;
        case ':': add_token(TokenType::COLON); break;

        // 1 or 2 chars token recognition
        case '*':
            add_token(match('*') ? TokenType::STAR_STAR : TokenType::STAR);
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

        // comment lexeme
        case '/':
            if (match('/')) // if the next char is also a '/'
            {
                // comment goes until the end of the line
                while (peek() != '\n' && !is_at_end())
                    advance(); // keep consuming characters till the end of the line
            }
            else
            {
                add_token(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break; // ignore whitespace

        case '\n':
            line++; // new line
            break;

        // string literal recognition
        case '"':
            string();
            break;

        default:
            if (std::isdigit(c)) // handle number literals
                number();
            else if (std::isalpha(c) || c == '_' || (c & 0x80) != 0) // handle identifiers
                identifier();
            else
                Error::error(line, std::string("Unexpected character: ") + "'" + c + "'");

            break;
    }
}

const std::map<std::string, TokenType> Lexer::keywords = {
    {"and", TokenType::AND},
    {"break", TokenType::BREAK},
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
    {"import", TokenType::IMPORT}
};
