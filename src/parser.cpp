#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens) {}

std::shared_ptr<Expr> Parser::parse()
{
    try
    {
        return expression();
    }
    catch(ParseError error)
    {
        return nullptr;
    }
}


std::shared_ptr<Expr> Parser::expression()
{
    return equality();
}

std::shared_ptr<Expr> Parser::equality()
{
    std::shared_ptr<Expr> expr = comparison();

    while (match(BANG_EQUAL, EQUAL_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
    std::shared_ptr<Expr> expr = term();

    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::term()
{
    std::shared_ptr<Expr> expr = factor();

    while (match(MINUS, PLUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::factor()
{
    std::shared_ptr<Expr> expr = unary();

    while (match(SLASH, STAR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
    if (match(BANG, MINUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<UnaryExpr>(std::move(op), right);
    }

    return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
    if (match(FALSE))
        return std::make_shared<LiteralExpr>(false);

    if (match(TRUE))
        return std::make_shared<LiteralExpr>(true);

    if (match(NIL))
        return std::make_shared<LiteralExpr>(nullptr);
        
    if (match(NUMBER, STRING))
        return std::make_shared<LiteralExpr>(previous().literal);

    if (match(LEFT_PAREN))
    {
        std::shared_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression");
        return std::make_shared<GroupingExpr>(expr);
    }

    throw error(peek(), "Expected an expression");
}

// check if current token as any type
template <class... T>
bool Parser::match(T... type)
{
    assert((... && std::is_same_v<T, TokenType>));

    if ((... || check(type)))
    {
        advance(); // consumes token
        return true;
    }

    return false;
}

Token Parser::consume(TokenType type, std::string msg)
{
    if (check(type))
        return advance();

    throw error(peek(), msg);
}

bool Parser::check(TokenType type)
{
    if (is_at_end())
        return false;

    return peek().type == type;
}

Token Parser::advance()
{
    if (!is_at_end())
        ++current;
    
    return previous();
}

bool Parser::is_at_end()
{
    return peek().type == TOKEN_EOF;
}

Token Parser::peek()
{
    return tokens.at(current);
}

Token Parser::previous()
{
    return tokens.at(current - 1);
}

ParseError Parser::error(const Token& token, std::string msg)
{
    Error::error(token, msg);
    return ParseError{""};
}

void Parser::synchronize()
{
    advance();

    while (!is_at_end())
    {
        if (previous().type == SEMICOLON)
            return;

        switch (peek().type)
        {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }

        advance();
    }
}
