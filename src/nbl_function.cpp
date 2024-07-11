#include <iostream>

#include "nbl_function.hpp"

NblFunction::NblFunction(std::string name, std::shared_ptr<FunctionExpr> declaration, std::shared_ptr<Environment> closure)
    : name(name), declaration(declaration), closure(closure) {}

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
        return r.value;
    }
    
    return nullptr;
}

std::string NblFunction::to_string()
{
    return name != "" ? "<func " + name + ">" : "<func lambda>";
}
