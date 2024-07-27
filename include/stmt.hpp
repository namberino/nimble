//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

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
struct MutStmt;
struct IfStmt;
struct WhileStmt;
struct FunctionStmt;
struct ReturnStmt;
struct BreakStmt;
struct ClassStmt;
struct ImportStmt;

struct StmtVisitor
{
    virtual ~StmtVisitor() = default;
    virtual std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) = 0;
    virtual std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) = 0;
    virtual std::any visitMutStmt(std::shared_ptr<MutStmt> stmt) = 0;
    virtual std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) = 0;
    virtual std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) = 0;
    virtual std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) = 0;
    virtual std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) = 0;
    virtual std::any visitBreakStmt(std::shared_ptr<BreakStmt> stmt) = 0;
    virtual std::any visitClassStmt(std::shared_ptr<ClassStmt> stmt) = 0;
    virtual std::any visitImportStmt(std::shared_ptr<ImportStmt> stmt) = 0;
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

struct MutStmt : Stmt, public std::enable_shared_from_this<MutStmt>
{
    const Token name;
    const std::shared_ptr<Expr> initializer;

    MutStmt(Token name, std::shared_ptr<Expr> initializer);
    std::any accept(StmtVisitor& visitor) override;
};

struct IfStmt : Stmt, public std::enable_shared_from_this<IfStmt>
{
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> then_branch;
    std::shared_ptr<Stmt> else_branch;

    IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch);
    std::any accept(StmtVisitor& visitor) override;
};

struct WhileStmt : Stmt, public std::enable_shared_from_this<WhileStmt>
{
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);
    std::any accept(StmtVisitor& visitor) override;
};

struct FunctionStmt : Stmt, public std::enable_shared_from_this<FunctionStmt>
{
    Token name;
    std::shared_ptr<FunctionExpr> fn;

    FunctionStmt(Token name, std::shared_ptr<FunctionExpr> fn);
    std::any accept(StmtVisitor& visitor) override;
};

struct ReturnStmt : Stmt, public std::enable_shared_from_this<ReturnStmt>
{
    const Token keyword;
    const std::shared_ptr<Expr> value;

    ReturnStmt(Token keyword, std::shared_ptr<Expr> value);
    std::any accept(StmtVisitor& visitor) override;
};

struct BreakStmt : Stmt, public std::enable_shared_from_this<BreakStmt>
{
    BreakStmt();
    std::any accept(StmtVisitor& visitor) override;
};

struct ClassStmt : Stmt, public std::enable_shared_from_this<ClassStmt>
{
    const Token name;
    const std::shared_ptr<MutExpr> superclass;
    const std::vector<std::shared_ptr<FunctionStmt>> methods;

    ClassStmt(Token name, std::shared_ptr<MutExpr> superclass, std::vector<std::shared_ptr<FunctionStmt>> methods);
    std::any accept(StmtVisitor& visitor) override;
};

struct ImportStmt : Stmt, public std::enable_shared_from_this<ImportStmt>
{
    Token keyword;
    std::shared_ptr<LiteralExpr> target;

    ImportStmt(Token keyword, std::shared_ptr<LiteralExpr> target);
    std::any accept(StmtVisitor& visitor) override;
};

#endif
