#ifndef EXPR_HPP
#define EXPR_HPP

#pragma once
#include <vector>
#include <memory>
#include <any>
#include <utility>

#include "token.hpp"

struct Stmt;

struct AssignExpr;
struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;
struct VarExpr;
struct LogicalExpr;
struct CallExpr;
struct FunctionExpr;
struct GetExpr;
struct SetExpr;
struct ThisExpr;
struct SuperExpr;
struct ListExpr;
struct SubscriptExpr;

// visitor struct (for visitor pattern handling)
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
    virtual std::any visitFunctionExpr(std::shared_ptr<FunctionExpr> expr) = 0;
    virtual std::any visitGetExpr(std::shared_ptr<GetExpr> expr) = 0;
    virtual std::any visitSetExpr(std::shared_ptr<SetExpr> expr) = 0;
    virtual std::any visitThisExpr(std::shared_ptr<ThisExpr> expr) = 0;
    virtual std::any visitSuperExpr(std::shared_ptr<SuperExpr> expr) = 0;
    virtual std::any visitListExpr(std::shared_ptr<ListExpr> expr) = 0;
    virtual std::any visitSubscriptExpr(std::shared_ptr<SubscriptExpr> expr) = 0;
};

// default expression virtual struct
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

struct FunctionExpr : Expr, public std::enable_shared_from_this<FunctionExpr>
{
    std::vector<Token> parameters;
    std::vector<std::shared_ptr<Stmt>> body;

    FunctionExpr(std::vector<Token> parameters, std::vector<std::shared_ptr<Stmt>> body);
    std::any accept(ExprVisitor& visitor) override;
};

struct GetExpr : Expr, public std::enable_shared_from_this<GetExpr>
{
    const std::shared_ptr<Expr> object;
    const Token name;

    GetExpr(std::shared_ptr<Expr> object, Token name);
    std::any accept(ExprVisitor& visitor) override;
};

struct SetExpr : Expr, public std::enable_shared_from_this<SetExpr>
{
    const std::shared_ptr<Expr> object;
    const Token name;
    const std::shared_ptr<Expr> value;

    SetExpr(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value);
    std::any accept(ExprVisitor& visitor) override;
};

struct ThisExpr : Expr, public std::enable_shared_from_this<ThisExpr>
{
    const Token keyword;

    ThisExpr(Token keyword);
    std::any accept(ExprVisitor& visitor) override;
};

struct SuperExpr : Expr, public std::enable_shared_from_this<SuperExpr>
{
    const Token keyword;
    const Token method;

    SuperExpr(Token keyword, Token method);
    std::any accept(ExprVisitor& visitor) override;
};

struct ListExpr : Expr, public std::enable_shared_from_this<ListExpr>
{
    std::vector<std::shared_ptr<Expr>> elements;

    ListExpr(std::vector<std::shared_ptr<Expr>> elements);
    std::any accept(ExprVisitor& visitor) override;
};

struct SubscriptExpr : Expr, public std::enable_shared_from_this<SubscriptExpr>
{
    std::shared_ptr<Expr> name;
    Token paren;
    std::shared_ptr<Expr> index;
    std::shared_ptr<Expr> value;

    SubscriptExpr(std::shared_ptr<Expr> name, Token paren, std::shared_ptr<Expr> index, std::shared_ptr<Expr> value);
    std::any accept(ExprVisitor& visitor) override;
};

#endif
