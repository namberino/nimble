#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <memory>
#include <string>
#include <any>
#include <stdexcept>
#include <iostream>

#include "expr.hpp"
#include "error.hpp"
#include "token.hpp"

class Interpreter : public ExprVisitor
{
    private:
        std::any evaluate(std::shared_ptr<Expr> expr);
        void check_num_operand(const Token& op, const std::any& operand);
        void check_num_operand(const Token& op, const std::any& left, const std::any& right);
        bool is_truthy(const std::any& obj);
        bool is_equal(const std::any& obj1, const std::any& obj2);
        std::string stringify(const std::any& obj);

    public:
        void interpret(std::shared_ptr<Expr> expression);
        std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
        std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
        std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
        std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
};

#endif
