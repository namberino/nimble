//------------------------------------//
// Copyright 2024 Nam Nguyen
// Licensed under Apache License v2.0
//------------------------------------//

#include "environment.hpp"

Environment::Environment()
    : enclosing(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(std::move(enclosing)) {}


std::any Environment::get(const Token& name)
{
    auto element = values.find(name.lexeme);

    if (element != values.end())
        return element->second;

    if (enclosing != nullptr)
        return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable: '" + name.lexeme + "'");
}

void Environment::assign(const Token& name, std::any value)
{
    auto element = values.find(name.lexeme);

    if (element != values.end())
    {
        element->second = std::move(value);
        return;
    }

    if (enclosing != nullptr)
    {
        enclosing->assign(name, std::move(value));
        return;
    }

    throw RuntimeError(name, "Undefined variable: '" + name.lexeme + "'");
}

void Environment::define(const std::string& name, std::any value)
{
    values[name] = std::move(value);
}

 std::shared_ptr<Environment> Environment::ancestor(int distance)
 {
    std::shared_ptr<Environment> environment = shared_from_this();

    for (int i = 0; i < distance; i++)
        environment = environment->enclosing;

    return environment;
 }

std::any Environment::get_at(int distance, const std::string& name)
{
    return ancestor(distance)->values[name];
}

void Environment::assign_at(int distance, const Token& name, std::any value)
{
    ancestor(distance)->values[name.lexeme] = std::move(value);
}
