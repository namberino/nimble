//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#include "stmt.hpp"

BlockStmt::BlockStmt(std::vector<std::shared_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

std::any BlockStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitBlockStmt(shared_from_this());
}


ExpressionStmt::ExpressionStmt(std::shared_ptr<Expr> expression) 
    : expression(std::move(expression)) {}

std::any ExpressionStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitExpressionStmt(shared_from_this());
}


PrintStmt::PrintStmt(std::shared_ptr<Expr> expression) 
    : expression(std::move(expression)) {}

std::any PrintStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitPrintStmt(shared_from_this());
}


VarStmt::VarStmt(Token name, std::shared_ptr<Expr> initializer) 
    : name(std::move(name)), initializer(std::move(initializer)) {}

std::any VarStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitVarStmt(shared_from_this());
}


IfStmt::IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch)
    : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

std::any IfStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitIfStmt(shared_from_this());
}


WhileStmt::WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
    : condition(std::move(condition)), body(std::move(body)) {}

std::any WhileStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitWhileStmt(shared_from_this());
}


FunctionStmt::FunctionStmt(Token name, std::shared_ptr<FunctionExpr> fn)
    : name{std::move(name)}, fn{std::move(fn)} {}

std::any FunctionStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitFunctionStmt(shared_from_this());
}


ReturnStmt::ReturnStmt(Token keyword, std::shared_ptr<Expr> value)
    : keyword{std::move(keyword)}, value{std::move(value)} {}

std::any ReturnStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitReturnStmt(shared_from_this());
}


BreakStmt::BreakStmt() {}

std::any BreakStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitBreakStmt(shared_from_this());
}


ClassStmt::ClassStmt(Token name, std::shared_ptr<VarExpr> superclass, std::vector<std::shared_ptr<FunctionStmt>> methods)
    : name(std::move(name)), superclass(std::move(superclass)), methods(std::move(methods)) {}

std::any ClassStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitClassStmt(shared_from_this());
}


ImportStmt::ImportStmt(Token keyword, std::shared_ptr<LiteralExpr> target)
    : keyword(std::move(keyword)), target(std::move(target)) {}

std::any ImportStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitImportStmt(shared_from_this());
}
