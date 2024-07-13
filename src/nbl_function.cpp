#include <iostream>

#include "nbl_function.hpp"

NblFunction::NblFunction(std::string name, std::shared_ptr<FunctionExpr> declaration, std::shared_ptr<Environment> closure, bool is_initializer)
    : name(name), declaration(declaration), closure(closure), is_initializer(is_initializer) {}

std::shared_ptr<NblFunction> NblFunction::bind(std::shared_ptr<NblInstance> instance)
{
    auto environment = std::make_shared<Environment>(closure);
    environment->define("this", instance);
    return std::make_shared<NblFunction>(name, declaration, environment, is_initializer);
}

int NblFunction::arity()
{
    return declaration->parameters.size();
}

std::any NblFunction::call(Interpreter& interpreter, std::vector<std::any> arguments)
{
    auto environment = std::make_shared<Environment>(closure);

    for (int i = 0; i < declaration->parameters.size(); i++)
        environment->define(declaration->parameters[i].lexeme, arguments[i]);

    try
    {
        interpreter.execute_block(declaration->body, environment);
    }
    catch(NblReturn r)
    {
        if (is_initializer)
            return closure->get_at(0, "this");
        return r.value;
    }
    
    if (is_initializer)
        return closure->get_at(0, "this");
    return nullptr;
}

std::string NblFunction::to_string()
{
    return name != "" ? "<func " + name + ">" : "<func lambda>";
}
