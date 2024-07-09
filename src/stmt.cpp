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
