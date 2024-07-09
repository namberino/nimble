#include "interpreter.hpp"

void Interpreter::interpret(std::shared_ptr<Expr> expression)
{
    try
    {
        std::any value = evaluate(expression);
        std::cout << stringify(value) + "\n";
    }
    catch (RuntimeError error)
    {
        Error::runtimeError(error);
    }
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr)
{
    // evaluate binary operators
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type)
    {
        // comparisors
        case BANG_EQUAL: return !is_equal(left, right);
        case EQUAL_EQUAL: return is_equal(left, right);
        case GREATER:
            check_num_operand(expr->op, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case GREATER_EQUAL:
            check_num_operand(expr->op, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case LESS:
            check_num_operand(expr->op, left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case LESS_EQUAL:
            check_num_operand(expr->op, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);

        // arithmetics
        case PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) + std::any_cast<double>(right);

            if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);

            throw RuntimeError{expr->op, "Operands must be 2 numbers or 2 strings"};
        case MINUS:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) - std::any_cast<double>(right);
        case STAR:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) * std::any_cast<double>(right);
        case SLASH:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) / std::any_cast<double>(right);
    
        default: break;
    }

    return {}; // unreachable, here to make the compiler happy
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr)
{
    // evaluating parentheses
    return evaluate(expr->expression);
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr)
{
    // evaluating literals
    return expr->value;
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr)
{
    // evaluate unary expressions
    std::any right = evaluate(expr->right);

    switch (expr->op.type)
    {
        case BANG: return !is_truthy(right);
        case MINUS:
            check_num_operand(expr->op, right);
            return -std::any_cast<double>(right);
        
        default: break;
    }

    return {}; // unreachable, here to make the compiler happy
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    // sends expression back into interpreter's visitor implementation
    return expr->accept(*this);
}

void Interpreter::check_num_operand(const Token& op, const std::any& operand)
{
    if (operand.type() == typeid(double))
        return;
    
    throw RuntimeError{op, "Operand must be a number"};
}

void Interpreter::check_num_operand(const Token& op, const std::any& left, const std::any& right)
{
    if (left.type() == typeid(double) && right.type() == typeid(double))
        return;
    
    throw RuntimeError{op, "Operands must be numbers"};
}

bool Interpreter::is_truthy(const std::any& obj)
{
    if (obj.type() == typeid(nullptr))
        return false;
    
    if (obj.type() == typeid(bool))
        return std::any_cast<bool>(obj);
    
    return true;
}

bool Interpreter::is_equal(const std::any& obj1, const std::any& obj2)
{
    if (obj1.type() == typeid(nullptr) && obj2.type() == typeid(nullptr))
        return true;

    if (obj1.type() == typeid(nullptr))
        return false;

    if (obj1.type() == typeid(std::string) && obj2.type() == typeid(std::string))
        return std::any_cast<std::string>(obj1) == std::any_cast<std::string>(obj2);

    if (obj1.type() == typeid(double) && obj2.type() == typeid(double))
        return std::any_cast<double>(obj1) == std::any_cast<double>(obj2);

    if (obj1.type() == typeid(bool) && obj2.type() == typeid(bool))
        return std::any_cast<bool>(obj1) == std::any_cast<bool>(obj2);

    return false;
}

std::string Interpreter::stringify(const std::any& obj)
{
    if (obj.type() == typeid(nullptr))
        return "nil";

    if (obj.type() == typeid(double))
    {
        std::string text = std::to_string(std::any_cast<double>(obj));

        if (text[text.length() - 2] == '.' && text[text.length() - 1] == '0')
            text = text.substr(0, text.length() - 2);

        return text;
    }

    if (obj.type() == typeid(std::string))
        return std::any_cast<std::string>(obj);

    if (obj.type() == typeid(bool))
        return std::any_cast<bool>(obj) ? "true" : "false";

    return "Error in stringify: Invalid object type";
}
