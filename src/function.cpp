//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#include <iostream>

#include "function.hpp"

// get the name, declaration, closure and verify if it is an initializer
NblFunction::NblFunction(std::string name, std::shared_ptr<FunctionExpr> declaration, std::shared_ptr<Environment> closure, bool is_initializer)
    : name(name), declaration(declaration), closure(closure), is_initializer(is_initializer) {}

std::shared_ptr<NblFunction> NblFunction::bind(std::shared_ptr<NblInstance> instance)
{
    // bind the function to a closure
    auto environment = std::make_shared<Environment>(closure);
    environment->define("this", instance);
    return std::make_shared<NblFunction>(name, declaration, environment, is_initializer);
}

int NblFunction::arity()
{
    // get the size of the parameters list
    return declaration->parameters.size();
}

std::any NblFunction::call(Interpreter& interpreter, std::vector<std::any> arguments)
{
    // create a new environment at each function call
    auto environment = std::make_shared<Environment>(closure);

    // add each parameter to the environment
    for (int i = 0; i < declaration->parameters.size(); i++)
        environment->define(declaration->parameters[i].lexeme, arguments[i]);

    try
    {
        // execute the body
        interpreter.execute_block(declaration->body, environment);
    }
    catch(NblReturn r) // catch return exception
    {
        // for classes
        if (is_initializer)
            return closure->get_at(0, "this");
        return r.value; // return the value
    }
    
    // no return exception
    if (is_initializer)
        return closure->get_at(0, "this");
    return nullptr;
}

std::string NblFunction::to_string()
{
    // for printing the function itself (and the lambda functions)
    return name != "" ? "<func " + name + ">" : "<func lambda>";
}
