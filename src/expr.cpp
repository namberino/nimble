#include "expr.hpp"

AssignExpr::AssignExpr(Token name, std::shared_ptr<Expr> value)
    : name(std::move(name)), value(std::move(value)) {}

std::any AssignExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitAssignExpr(shared_from_this());
}


BinaryExpr::BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

std::any BinaryExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitBinaryExpr(shared_from_this());
}


GroupingExpr::GroupingExpr(std::shared_ptr<Expr> expression)
    : expression(std::move(expression)) {}

std::any GroupingExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitGroupingExpr(shared_from_this());
}


LiteralExpr::LiteralExpr(std::any value)
    : value(std::move(value)) {}

std::any LiteralExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitLiteralExpr(shared_from_this());
}


UnaryExpr::UnaryExpr(Token op, std::shared_ptr<Expr> right)
    : op(std::move(op)), right(std::move(right)) {}

std::any UnaryExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitUnaryExpr(shared_from_this());
}

VarExpr::VarExpr(Token name)
    : name(std::move(name)) {}

std::any VarExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitVarExpr(shared_from_this());
}
