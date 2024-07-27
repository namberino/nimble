# Environment

The bindings that associate variables to values need to be stored somewhere. This is where *environment* comes into play. Environment is a data structure kinda like a map. The keys are variable names, and the values are variables' value.

```cpp
class Environment : public std::enable_shared_from_this<Environment>
{
    friend class Interpreter;

    std::shared_ptr<Environment> enclosing;
    std::map<std::string, std::any> values;

    public:
        Environment();
        Environment(std::shared_ptr<Environment> enclosing);

        std::any get(const Token& name);
        void assign(const Token& name, std::any value);
        void define(const std::string& name, std::any value);
        std::shared_ptr<Environment> ancestor(int distance);
        std::any get_at(int distance, const std::string& name);
        void assign_at(int distance, const Token& name, std::any value);
};
```

We use `std::map` to store the bindings. A token represents a unit of code at a specific place in the source text, but when it comes to looking up variables, all identifier tokens with the same name should refer to the same variable. Using the raw strings as keys ensures all of those tokens refer to the same map key.

For variable definition and redefinition, we need to bind a value to a new name:

```cpp
void Environment::define(const std::string& name, std::any value)
{
    values[name] = std::move(value);
}
```

For looking up an existing variable:

```cpp
std::any Environment::get(const Token& name)
{
    auto element = values.find(name.lexeme);

    // key found
    if (element != values.end())
        return element->second;

    // enclosing environment
    if (enclosing != nullptr)
        return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable: '" + name.lexeme + "'");
}
```

For assigning variables:

```cpp
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

```
