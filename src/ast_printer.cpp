#include "ast_printer.hpp"

std::string AstPrinter::print(std::shared_ptr<Expr> expr)
{
    return std::any_cast<std::string>(expr->accept(*this));
}

std::any AstPrinter::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr)
{
    return parenthesize(expr->op.lexeme, expr->left, expr->right);
}

std::any AstPrinter::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr)
{
    return parenthesize("group", expr->expression);
}

std::any AstPrinter::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr)
{
    auto& value_type = expr->value.type();

    if (value_type == typeid(nullptr)) // null data
        return "nil";
    else if (value_type == typeid(std::string)) // string data
        return std::any_cast<std::string>(expr->value);
    else if (value_type == typeid(double)) // double data
        return std::to_string(std::any_cast<double>(expr->value));
    else if (value_type == typeid(bool)) // boolean data
        return std::any_cast<bool>(expr->value) ? "true" : "false";
    
    return "Error in visitLiteralExpr: literal type not recognized";
}

std::any AstPrinter::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr)
{
    return parenthesize(expr->op.lexeme, expr->right);
}

template<class... E>
std::string AstPrinter::parenthesize(std::string_view name, E... expr)
{
    assert((... && std::is_same_v<E, std::shared_ptr<Expr>>));

    std::ostringstream builder;

    builder << "(" << name;
    ((builder << " " << print(expr)), ...);
    builder << ")";

    return builder.str();
}
