#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <any>
#include <memory>
#include <string>
#include <stdexcept>

#include "expr.hpp"
#include "error.hpp"
#include "token.hpp"
#include "stmt.hpp"
#include "environment.hpp"
#include "builtins.hpp"
#include "nbl_callable.hpp"
#include "nbl_function.hpp"

class BreakException : public std::runtime_error
{
    public:
        BreakException() : std::runtime_error("break") {};
};

class Interpreter : public ExprVisitor, public StmtVisitor
{
    public: std::shared_ptr<Environment> globals{new Environment};
    private: std::shared_ptr<Environment> environment = globals;

    private:
        std::any evaluate(std::shared_ptr<Expr> expr);
        void execute(std::shared_ptr<Stmt> stmt);
        void check_num_operand(const Token& op, const std::any& operand);
        void check_num_operands(const Token& op, const std::any& left, const std::any& right);
        bool is_truthy(const std::any& obj);
        bool is_equal(const std::any& obj1, const std::any& obj2);
        std::string stringify(const std::any& obj);

    public:
        Interpreter();
        void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
        std::string interpret(const std::shared_ptr<Expr>& expr);
        void execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment);

        std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) override;
        std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
        std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
        std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
        std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
        std::any visitVarExpr(std::shared_ptr<VarExpr> expr) override;
        std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) override;
        std::any visitCallExpr(std::shared_ptr<CallExpr> expr) override;

        std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) override;
        std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) override;
        std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) override;
        std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) override;
        std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) override;
        std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) override;
        std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) override;
        std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) override;
        std::any visitBreakStmt(std::shared_ptr<BreakStmt> stmt) override;
};

#endif
