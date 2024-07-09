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
#include "stmt.hpp"
#include "environment.hpp"

class Interpreter : public ExprVisitor, public StmtVisitor
{
    std::shared_ptr<Environment> environment{new Environment};

    private:
        std::any evaluate(std::shared_ptr<Expr> expr);
        void execute(std::shared_ptr<Stmt> stmt);
        void execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment);
        void check_num_operand(const Token& op, const std::any& operand);
        void check_num_operands(const Token& op, const std::any& left, const std::any& right);
        bool is_truthy(const std::any& obj);
        bool is_equal(const std::any& obj1, const std::any& obj2);
        std::string stringify(const std::any& obj);

    public:
        void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
        std::string interpret(const std::shared_ptr<Expr>& expr);

        std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) override;
        std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
        std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
        std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
        std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
        std::any visitVarExpr(std::shared_ptr<VarExpr> expr) override;

        std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) override;
        std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) override;
        std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) override;
        std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) override;
};

#endif
