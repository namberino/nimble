#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#pragma once
#include <iostream>
#include <any>
#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "expr.hpp"

class AstPrinter : public ExprVisitor
{
    private:
        template<class... E>
        std::string parenthesize(std::string_view name, E... expr);

    public:
        std::string print(std::shared_ptr<Expr> expr);
        std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
        std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
        std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
        std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
};

#endif
