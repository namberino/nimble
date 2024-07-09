#ifndef STMT_HPP
#define STMT_HPP

#pragma once
#include <any>
#include <memory>
#include <vector>
#include <utility>

#include "token.hpp"
#include "expr.hpp"

struct BlockStmt;
struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;

struct StmtVisitor
{
    virtual ~StmtVisitor() = default;
    virtual std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) = 0;
    virtual std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) = 0;
    virtual std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) = 0;
};

struct Stmt
{
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt>
{
    const std::vector<std::shared_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::shared_ptr<Stmt>> statements);
    std::any accept(StmtVisitor& visitor) override;
};

struct ExpressionStmt : Stmt, public std::enable_shared_from_this<ExpressionStmt>
{
    const std::shared_ptr<Expr> expression;

    ExpressionStmt(std::shared_ptr<Expr> expression);
    std::any accept(StmtVisitor& visitor) override;
};

struct PrintStmt : Stmt, public std::enable_shared_from_this<PrintStmt>
{
    const std::shared_ptr<Expr> expression;

    PrintStmt(std::shared_ptr<Expr> expression);
    std::any accept(StmtVisitor& visitor) override;
};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt>
{
    const Token name;
    const std::shared_ptr<Expr> initializer;

    VarStmt(Token name, std::shared_ptr<Expr> initializer);
    std::any accept(StmtVisitor& visitor) override;
};

#endif
