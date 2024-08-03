//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#ifndef RESOLVER_HPP
#define RESOLVER_HPP

#pragma once
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>

#include "interpreter.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "error.hpp"

namespace fs = std::filesystem;

// function types for resolution
enum class FunctionType
{
    NONE,
    FUNCTION,
    INITIALIZER,
    METHOD
};

// class types for resolution
enum class ClassType
{
    NONE,
    CLASS,
    SUBCLASS
};

class Resolver : public ExprVisitor, public StmtVisitor
{
    private:
        Interpreter& interpreter;
        std::vector<std::map<std::string, bool>> scopes;
        FunctionType current_func = FunctionType::NONE;
        ClassType current_class = ClassType::NONE;

        std::string executed_path;
        fs::path base_path = get_base_path();
        std::string core_lib_dir = (base_path / "lib").generic_string();

        fs::path get_base_path();

        void resolve(std::shared_ptr<Stmt> stmt);
        void resolve(std::shared_ptr<Expr> expr);
        void resolve_function(std::shared_ptr<FunctionExpr> fn, FunctionType type);
        void resolve_local(std::shared_ptr<Expr> expr, const Token& name);
        void declare(const Token& name);
        void define(const Token& name);
        void begin_scope();
        void end_scope();

    public:
        Resolver(Interpreter& interpreter, std::string& executed_path);
        void resolve(const std::vector<std::shared_ptr<Stmt>>& statements);

        std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) override;
        std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
        std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
        std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
        std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
        std::any visitMutExpr(std::shared_ptr<MutExpr> expr) override;
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
        std::any visitMutStmt(std::shared_ptr<MutStmt> stmt) override;
        std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) override;
        std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) override;
        std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) override;
        std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) override;
        std::any visitBreakStmt(std::shared_ptr<BreakStmt> stmt) override;
        std::any visitClassStmt(std::shared_ptr<ClassStmt> stmt) override;
        std::any visitImportStmt(std::shared_ptr<ImportStmt> stmt) override;
};

#endif
