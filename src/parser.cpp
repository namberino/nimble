#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse()
{
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!is_at_end())
    {
        // statements.push_back(statement());
        statements.push_back(declaration());
    }

    return statements;
}

std::any Parser::parse_repl()
{
    allow_expression = true;
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!is_at_end())
    {
        statements.push_back(declaration());

        if (found_expression)
        {
            std::shared_ptr<Stmt> last = statements.back();
            return std::dynamic_pointer_cast<ExpressionStmt>(last)->expression;
        }

        allow_expression = false;
    }

    return statements;
}


std::shared_ptr<Stmt> Parser::statement()
{
    if (match(PRINT))
        return print_statement();

    if (match(LEFT_BRACE))
        return std::make_shared<BlockStmt>(block());

    return expression_statement();
}

std::shared_ptr<Stmt> Parser::print_statement()
{
    consume(LEFT_PAREN, "Expected '(' after 'print' command");
    std::shared_ptr<Expr> value = expression();
    consume(RIGHT_PAREN, "Missing ')' for 'print' command");
    consume(SEMICOLON, "Expected ';' after value");
    return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::expression_statement()
{
    std::shared_ptr<Expr> expr = expression();

    if (allow_expression && is_at_end())
        found_expression = true;
    else
        consume(SEMICOLON, "Expected ';' after expression");

    return std::make_shared<ExpressionStmt>(expr);
}

std::vector<std::shared_ptr<Stmt>> Parser::block()
{
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !is_at_end())
        statements.push_back(declaration());

    consume(RIGHT_BRACE, "Expected '}' after block");
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration()
{
    try
    {
        if (match(VAR))
            return var_declaration();

        return statement();
    }
    catch (ParseError error)
    {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::var_declaration()
{
    Token name = consume(IDENTIFIER, "Expected variable name");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match(EQUAL))
        initializer = expression();

    consume(SEMICOLON, "Expected ';' after variable declaration");
    return std::make_shared<VarStmt>(std::move(name), initializer);
}

std::shared_ptr<Expr> Parser::assignment()
{
    std::shared_ptr<Expr> expr = equality();

    if (match(EQUAL))
    {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        if (VarExpr* e = dynamic_cast<VarExpr*>(expr.get()))
        {
            Token name = e->name;
            return std::make_shared<AssignExpr>(std::move(name), value);
        }

        Error::error(std::move(equals), "Invalid assignment target");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::expression()
{
    return assignment();
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

    if (match(IDENTIFIER))
        return std::make_shared<VarExpr>(previous());

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
