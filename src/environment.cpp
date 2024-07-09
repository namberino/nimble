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
