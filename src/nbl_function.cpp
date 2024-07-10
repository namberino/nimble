#include <iostream>

#include "nbl_function.hpp"

NblFunction::NblFunction(std::shared_ptr<FunctionStmt> declaration, std::shared_ptr<Environment> closure)
    : declaration(declaration), closure(closure) {}

int NblFunction::arity()
{
    return declaration->params.size();
}

std::any NblFunction::call(Interpreter& interpreter, std::vector<std::any> arguments)
{
    auto environment = std::make_shared<Environment>(closure);

    for (int i = 0; i < declaration->params.size(); i++)
        environment->define(declaration->params[i].lexeme, arguments[i]);

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
    return "<func " + declaration->name.lexeme + ">";
}
