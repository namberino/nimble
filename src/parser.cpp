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

    if (match(IF))
        return if_statement();

    if (match(FOR))
        return for_statement();

    if (match(WHILE))
        return while_statement();
    
    if (match(RETURN))
        return return_statement();

    if (match(BREAK))
        return break_statement();

    if (match(LEFT_BRACE))
        return std::make_shared<BlockStmt>(block());
    
    return expression_statement();
}

std::shared_ptr<Stmt> Parser::print_statement()
{
    consume(LEFT_PAREN, "Expected '(' after 'print' statement");
    // shared_ptr used here be cause it's accessing another shared_ptr object already created by expression()
    std::shared_ptr<Expr> value = expression();
    consume(RIGHT_PAREN, "Missing ')' for 'print' statement");
    consume(SEMICOLON, "Expected ';' after value");

    // make_shared combines allocation of the object and control block into a single memory allocation
    // more efficient and better exception safety
    return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::if_statement()
{
    consume(LEFT_PAREN, "Expected '(' after 'if' statement");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after 'if' condition");

    std::shared_ptr<Stmt> then_branch = statement();
    std::shared_ptr<Stmt> else_branch = nullptr;

    if (match(ELSE))
        else_branch = statement();

    return std::make_shared<IfStmt>(condition, then_branch, else_branch);
}

std::shared_ptr<Stmt> Parser::for_statement()
{
    try 
    {
        consume(LEFT_PAREN, "Expected '(' after 'for' statement");
        std::shared_ptr<Stmt> initializer;

        if (match(SEMICOLON)) // initializer omitted
            initializer = nullptr;
        else if (match(VAR)) // variable declaration
            initializer = var_declaration();
        else // expression
            initializer = expression_statement();

        std::shared_ptr<Expr> condition = nullptr;
        if (!check(SEMICOLON)) // clause not omitted
            condition = expression();
        consume(SEMICOLON, "Expected ';' after loop condition");
        
        std::shared_ptr<Expr> increment = nullptr;
        if (!check(RIGHT_PAREN)) // clause not omitted
            increment = expression();
        consume(RIGHT_PAREN, "Expected ')' after 'for' clauses");

        std::shared_ptr<Stmt> body = statement();
        if (increment != nullptr)
            // executes after the body in each iteration of the loop
            // replace body with a block that contains the body with an expression statement that evaluates the increment
            body = std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{body, std::make_shared<ExpressionStmt>(increment)});

        if (condition == nullptr)
            // true if condition is omitted
            condition = std::make_shared<LiteralExpr>(true);
        body = std::make_shared<WhileStmt>(condition, body); // build for loop with while loop

        if (initializer != nullptr) // runs once
            // replace statement with a block that runs the initializer and execute the loop
            body = std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{initializer, body});

        return body;
    }
    catch (...) // throw error for trying to use break outside a loop
    {
        loop_depth--;
        throw;
    }

    loop_depth--;
}

std::shared_ptr<Stmt> Parser::while_statement()
{
    try
    {
        loop_depth++;

        consume(LEFT_PAREN, "Expected '(' after 'while' statement");
        std::shared_ptr<Expr> condition = expression();
        consume(RIGHT_PAREN, "Expected ')' after 'while' condition");
        std::shared_ptr<Stmt> body = statement();

        return std::make_shared<WhileStmt>(condition, body);
    }
    catch (...) // throw error for trying to use break outside a loop
    {
        loop_depth--;
        throw;
    }

    loop_depth--;
}

std::shared_ptr<Stmt> Parser::return_statement()
{
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    
    if (!check(SEMICOLON))
        value = expression();
    consume(SEMICOLON, "Expected ';' after return value");

    return std::make_shared<ReturnStmt>(keyword, value);
}

std::shared_ptr<Stmt> Parser::break_statement()
{
    if (loop_depth == 0)
        error(previous(), "Must be inside a loop to use 'break'");
    consume(SEMICOLON, "Expected ';' after 'break'");

    return std::make_shared<BreakStmt>();
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

        if (match(FUN))
            return function("function");

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

std::shared_ptr<FunctionStmt> Parser::function(std::string kind)
{
    Token name = consume(IDENTIFIER, "Expected " + kind + " name");

    consume(LEFT_PAREN, "Expected '(' after " + kind + " name");

    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN))
    {
        do
        {
            if (parameters.size() >= 255)
                error(peek(), "Can't have more than 255 parameters.");

            parameters.push_back(consume(IDENTIFIER, "Expected parameter name"));
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expected ')' after parameters");

    consume(LEFT_BRACE, "Expected '{' before " + kind + " body");
    std::vector<std::shared_ptr<Stmt>> body = block();

    return std::make_shared<FunctionStmt>(std::move(name), std::move(parameters), std::move(body));
}

std::shared_ptr<Expr> Parser::assignment()
{
    std::shared_ptr<Expr> expr = or_expression();

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

std::shared_ptr<Expr> Parser::or_expression()
{
    std::shared_ptr<Expr> expression = and_expression();

    while (match(OR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = and_expression();
        expression = std::make_shared<LogicalExpr>(expression, std::move(op), right);
    }

    return expression;
}

std::shared_ptr<Expr> Parser::and_expression()
{
    std::shared_ptr<Expr> expression = equality();

    while (match(AND))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expression = std::make_shared<LogicalExpr>(expression, std::move(op), right);
    }

    return expression;
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

    return call();
}

std::shared_ptr<Expr> Parser::finish_call(std::shared_ptr<Expr> callee)
{
    std::vector<std::shared_ptr<Expr>> arguments;

    if (!check(RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 255)
                error(peek(), "Can't have more than 255 arguments");

            arguments.push_back(expression());
        } while (match(COMMA));
    }

    Token paren = consume(RIGHT_PAREN, "Expected ')' after arguments");

    return std::make_shared<CallExpr>(callee, std::move(paren), std::move(arguments));
}

std::shared_ptr<Expr> Parser::call()
{
    std::shared_ptr<Expr> expr = primary();

    while (true)
    {
        if (match(LEFT_PAREN))
            expr = finish_call(expr);
        else
            break;
    }

    return expr;
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
