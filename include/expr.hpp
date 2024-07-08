#ifndef EXPR_HPP
#define EXPR_HPP

#pragma once

class Expr
{
    public:
        virtual ~Expr() = default;
};

class BinaryExpr : public Expr
{
    private:
        std::unique_ptr<Expr> left;
        Token op;
        std::unique_ptr<Expr> right;

    public:
        BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
};

#endif
