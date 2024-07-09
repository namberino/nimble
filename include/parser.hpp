#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <cassert>
#include <utility>

#include "expr.hpp"
#include "error.hpp"
#include "token.hpp"
#include "stmt.hpp"

struct ParseError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class Parser
{
    const std::vector<Token>& tokens;
    int current = 0;

    private:
        bool allow_expression;
        bool found_expression = false;

        std::shared_ptr<Stmt> statement();
        std::shared_ptr<Stmt> print_statement();
        std::shared_ptr<Stmt> expression_statement();
        std::vector<std::shared_ptr<Stmt>> block();
        std::shared_ptr<Stmt> declaration();
        std::shared_ptr<Stmt> var_declaration();

        std::shared_ptr<Expr> assignment();
        std::shared_ptr<Expr> expression();
        std::shared_ptr<Expr> equality();
        std::shared_ptr<Expr> comparison();
        std::shared_ptr<Expr> term();
        std::shared_ptr<Expr> factor();
        std::shared_ptr<Expr> unary();
        std::shared_ptr<Expr> primary();

        template <class... T>
        bool match(T... type);
        Token consume(TokenType type, std::string msg);
        bool check(TokenType type);
        Token advance();
        bool is_at_end();
        Token peek();
        Token previous();
        ParseError error(const Token& token, std::string msg);
        void synchronize();

    public:
        Parser(const std::vector<Token>& tokens);
        std::vector<std::shared_ptr<Stmt>> parse();
        std::any parse_repl();
};

#endif
