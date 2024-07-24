//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <any>
#include <memory>
#include <string>
#include <stdexcept>
#include <cmath>

#include "expr.hpp"
#include "error.hpp"
#include "token.hpp"
#include "stmt.hpp"
#include "environment.hpp"
#include "builtins.hpp"
#include "callable.hpp"
#include "function.hpp"
#include "class.hpp"
#include "instance.hpp"
#include "list.hpp"
#include "util.hpp"

class BreakException : public std::runtime_error
{
    public:
        BreakException() : std::runtime_error("break") {};
};

class Interpreter : public ExprVisitor, public StmtVisitor
{
    public:
        std::shared_ptr<Environment> globals{new Environment};
    
    private:
        std::map<std::shared_ptr<Expr>, int> locals;
        std::shared_ptr<Environment> environment = globals;

    private:
        std::any lookup_var(const Token& name, std::shared_ptr<Expr> expr);
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
        void resolve(std::shared_ptr<Expr> expr, int depth);

        std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) override;
        std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
        std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
        std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
        std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
        std::any visitVarExpr(std::shared_ptr<VarExpr> expr) override;
        std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) override;
        std::any visitCallExpr(std::shared_ptr<CallExpr> expr) override;
        std::any visitFunctionExpr(std::shared_ptr<FunctionExpr> expr) override;
        std::any visitGetExpr(std::shared_ptr<GetExpr> expr) override;
        std::any visitSetExpr(std::shared_ptr<SetExpr> expr) override;
        std::any visitThisExpr(std::shared_ptr<ThisExpr> expr) override;
        std::any visitSuperExpr(std::shared_ptr<SuperExpr> expr) override;
        std::any visitListExpr(std::shared_ptr<ListExpr> expr) override;
        std::any visitSubscriptExpr(std::shared_ptr<SubscriptExpr> expr) override;

        std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) override;
        std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) override;
        std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) override;
        std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) override;
        std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) override;
        std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) override;
        std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) override;
        std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) override;
        std::any visitBreakStmt(std::shared_ptr<BreakStmt> stmt) override;
        std::any visitClassStmt(std::shared_ptr<ClassStmt> stmt) override;
        std::any visitImportStmt(std::shared_ptr<ImportStmt> stmt) override;
};

#endif
