#ifndef EXPR_HPP
#define EXPR_HPP

#pragma once
#include <vector>
#include <memory>
#include <any>
#include <utility>

#include "token.hpp"

struct AssignExpr;
struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;
struct VarExpr;
struct LogicalExpr;
struct CallExpr;

struct ExprVisitor
{
    virtual ~ExprVisitor() = default;
    virtual std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) = 0;
    virtual std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) = 0;
    virtual std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) = 0;
    virtual std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) = 0;
    virtual std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) = 0;
    virtual std::any visitVarExpr(std::shared_ptr<VarExpr> expr) = 0;
    virtual std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) = 0;
    virtual std::any visitCallExpr(std::shared_ptr<CallExpr> expr) = 0;
};

struct Expr
{
    virtual ~Expr() = default;
    virtual std::any accept(ExprVisitor& visitor) = 0;
};

struct AssignExpr : Expr, public std::enable_shared_from_this<AssignExpr>
{
    const Token name;
    const std::shared_ptr<Expr> value;

    AssignExpr(Token name, std::shared_ptr<Expr> value);
    std::any accept(ExprVisitor& visitor) override;
};

struct BinaryExpr : Expr, public std::enable_shared_from_this<BinaryExpr>
{
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor& visitor) override;
};

struct GroupingExpr : Expr, public std::enable_shared_from_this<GroupingExpr>
{
    const std::shared_ptr<Expr> expression;

    GroupingExpr(std::shared_ptr<Expr> expression);
    std::any accept(ExprVisitor& visitor) override;
};

struct LiteralExpr : Expr, public std::enable_shared_from_this<LiteralExpr>
{
    const std::any value;

    LiteralExpr(std::any value);
    std::any accept(ExprVisitor& visitor) override;
};

struct UnaryExpr : Expr, public std::enable_shared_from_this<UnaryExpr>
{
    const Token op;
    const std::shared_ptr<Expr> right;

    UnaryExpr(Token op, std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor& visitor) override;
};

struct VarExpr : Expr, public std::enable_shared_from_this<VarExpr>
{
    const Token name;

    VarExpr(Token name);
    std::any accept(ExprVisitor& visitor) override;
};

struct LogicalExpr : Expr, public std::enable_shared_from_this<LogicalExpr>
{
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    LogicalExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor& visitor) override;
};

struct CallExpr : Expr, public std::enable_shared_from_this<CallExpr>
{
    std::shared_ptr<Expr> callee;
    Token paren;
    std::vector<std::shared_ptr<Expr>> arguments;

    CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments);
    std::any accept(ExprVisitor& visitor) override;
};

#endif
