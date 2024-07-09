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
