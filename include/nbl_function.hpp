#ifndef NBL_FUNCTION_HPP
#define NBL_FUNCTION_HPP

#pragma once
#include "interpreter.hpp"
#include "nbl_instance.hpp"

class NblInstance;

struct NblReturn
{
    const std::any value;
};

class NblFunction : public NblCallable
{
    private:
        std::string name;
        std::shared_ptr<FunctionExpr> declaration;
        std::shared_ptr<Environment> closure;
        bool is_initializer;

    public:
        NblFunction(std::string name, std::shared_ptr<FunctionExpr> declaration, std::shared_ptr<Environment> closure, bool is_initializer);
        std::shared_ptr<NblFunction> bind(std::shared_ptr<NblInstance> instance);
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
        std::string to_string() override;
};

#endif
