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

## Scope

A scope defines a region where a variable is mapped to a certain object. Multiple scopes enable the same name to refer to different things in different context.

Lexical scope is a style of scoping where the text of the program shows where a scope begins and ends. We use braces (`{` and `}`) to indicate scopes.

Example (2 blocks):

```nimble
{
    var test = "first";
    print(test); // first
}

{
    var test = "second";
    print(test); // second
}

{
    var test = "third";
}
print(test); // error, no test variable outside of scope
```

## Nesting

As we visit each statement inside the block, keep track of any variables declared. After the last statement in the block scope is executed, tell the environment to delete all of those variables. However, for global variables with the same name as the variables declared inside the block, we can't just delete those globla variables as those are different variables.

When a local variable has the same name as a variable in an enclosing scope, it *covers* the outer one. The code inside the block can’t see it any more, but it’s still there.

When we enter a new block scope, we need to save variables defined in outer scopes so that they will still exist when we exit the inner block. We do that by defining an environment for each of the block containing only variables in that scope. We also need to handle variables that are not covered.

If we need to access a variable in an outer block inside of an inner scope, the interpreter needs to search the current inner environment and any enclosing ones. We can do this by chaining environments. Each environment has a reference to the environment of the enclosing scope. When we look up a variable, we go up the chain (starting from the innermost scope) until we find the variable.

This is the reference to the enclosing scope:

```cpp
std::shared_ptr<Environment> enclosing;
```

We initialize this field by using 2 different constructors, 1 for the global scope, 1 for a new inner scope:

```cpp
Environment::Environment()
    : enclosing(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(std::move(enclosing)) {}
```

If the variable isn't in the current environment, we'll check the outer environment recursively.
