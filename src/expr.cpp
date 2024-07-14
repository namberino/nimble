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


LogicalExpr::LogicalExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

std::any LogicalExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitLogicalExpr(shared_from_this());
}

CallExpr::CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
    : callee{std::move(callee)}, paren{std::move(paren)}, arguments{std::move(arguments)} {}

std::any CallExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitCallExpr(shared_from_this());
}


FunctionExpr::FunctionExpr(std::vector<Token> parameters, std::vector<std::shared_ptr<Stmt>> body)
    : parameters(std::move(parameters)), body(std::move(body)) {}

std::any FunctionExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitFunctionExpr(shared_from_this());
}


GetExpr::GetExpr(std::shared_ptr<Expr> object, Token name)
    : object(std::move(object)), name(std::move(name)) {}

std::any GetExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitGetExpr(shared_from_this());
}


SetExpr::SetExpr(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
    : object(std::move(object)), name(std::move(name)), value(std::move(value)) {}

std::any SetExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitSetExpr(shared_from_this());
}


ThisExpr::ThisExpr(Token keyword)
    : keyword(std::move(keyword)) {}

std::any ThisExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitThisExpr(shared_from_this());
}


SuperExpr::SuperExpr(Token keyword, Token method)
    : keyword(std::move(keyword)), method(std::move(method)) {}

std::any SuperExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitSuperExpr(shared_from_this());
}


ListExpr::ListExpr(std::vector<std::shared_ptr<Expr>> elements)
    : elements(std::move(elements)) {}

std::any ListExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitListExpr(shared_from_this());
}


SubscriptExpr::SubscriptExpr(std::shared_ptr<Expr> name, Token paren, std::shared_ptr<Expr> index)
    : name(std::move(name)), paren(std::move(paren)), index(std::move(index)) {}

std::any SubscriptExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitSubscriptExpr(shared_from_this());
}


ListSetExpr::ListSetExpr(std::shared_ptr<Expr> name, std::shared_ptr<Expr> index, std::shared_ptr<Expr> value)
    : name(std::move(name)), index(std::move(index)), value(std::move(value)) {}

std::any ListSetExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitListSetExpr(shared_from_this());
}
