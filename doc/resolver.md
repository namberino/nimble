# Resolver

A resolver will track down which declaration a variable refers to everytime an expression is evaluated.

> If that variable is swaddled inside a loop that runs a thousand times, that variable gets re-resolved a thousand times.

## Mutable environments

Environments are dynamic representations of static scopes. A new environment is created when we enter a new scope, and will be discarded when we leave the scope. Environments is also where we bind variables.

Closures are different. When a function is declared, closures capture the current environment as it existed when the function was declared. 

Example:

```nimble
mut n = 5;
{
    fun print_n()
    {
        print(n);
    }

    print(n); // should print 5
    print_n(); // should print 5

    mut n = 6; // declaring a new variable

    print_n(); // should print 5
    print(n); // should print 6

    n = 6; // reassigning the n variable

    print_n(); // should print 6
}
```

This is because the closures are immutable. So when we use `mut n`, we're declaring a new variable outside of the immutable closure. So the `print_n()` function will not register the change. It we were to reassign the `n` variable, the second `print_n()` function call will print 6, the variable n inside the closure was reassigned, no new variable added.

A closure will retain a reference to the environment instance in play when the function was declared. Since any later declarations in that block would produce new environment objects, the closure wouldn’t see the new variables.

We'll need to inspect the user's program and figure out which declaration each refers to. We store the binding between a variable and its declaration by utilizing the environment class. This is resolution. To resolve a variable usage, we calculate how far away the declared variable is in the environment chain. We do the resolution once, so it's not dynamic (which is kinda good since dynamic analysis is a whole 'nother can of worm).

After the AST is produced, we walk through the tree and resolve all of the variables. The resolver will walk through the tree, visit each node, and perform analysis on each of the node.

```cpp
Resolver::Resolver(Interpreter& interpreter, std::string& executed_path)
    : interpreter(interpreter), executed_path(executed_path) {}
```

Since the resolver needs to visit every node in the AST, it needs to inherit from `ExprVisitor` and `StmtVisitor`. The resolver will check these nodes:
- Block statements: Since it introduces a new scope for the statement it contains.
- Function declaration: Since it introduces a new scope for its body and binds its parameters in that scope.
- Variable declaration: Since it adds a new variable to the current scope.
- Variable and assignment expressions: Since they need to have their variables resolved.

## Resolving nodes

Let's start with block statements:

```cpp
void Resolver::begin_scope()
{
    scopes.push_back(std::map<std::string, bool>{});
}

void Resolver::end_scope()
{
    scopes.pop_back();
}

void Resolver::resolve(const std::vector<std::shared_ptr<Stmt>>& statements)
{
    for (const std::shared_ptr<Stmt>& statement : statements)
        resolve(statement);
}

void Resolver::resolve(std::shared_ptr<Stmt> stmt)
{
    stmt->accept(*this);
}

void Resolver::resolve(std::shared_ptr<Expr> expr)
{
    expr->accept(*this);
}

std::any Resolver::visitBlockStmt(std::shared_ptr<BlockStmt> stmt)
{
    begin_scope();
    resolve(stmt->statements);
    end_scope();
    return {};
}
```

This begins a new scope, traverses the statements in the block, then discard the scope. The `resolve()` function will walk through the list of statements and resolve each one of them. Resolving means applying the visitor function on those nodes.

For the chain environment, we do that by implementing a stack using `std::vector`.

```cpp
std::vector<std::map<std::string, bool>> scopes;
```

This field helps keep track of the stack of scopes currently in scope. Each element in the stack is a map representing a block scope. The keys (as in environment) are variable names, and the values are booleans.

The scope stack is used for local block scopes. Variables declared in the global scope is not tracked by the resolver (because they are more dynamic). When resolving variables, if we can't find it in the stack of local scopes, then it must be global. And we can exit a scope by just popping it from the stack.

For variable statements:

```cpp
std::any Resolver::visitMutStmt(std::shared_ptr<MutStmt> stmt)
{
    declare(stmt->name);

    if (stmt->initializer != nullptr)
        resolve(stmt->initializer);
    define(stmt->name);

    return {};
}
```

We add a new entry to the current innermost scope's map. We do this by splitting the binding process into 2: declaring and defining. As we visit an expression, we need to know if we're inside the initializer for some variable. We do that by first declaring it:

```cpp
void Resolver::declare(const Token& name)
{
    if (scopes.empty())
        return;
    
    std::map<std::string, bool>& scope = scopes.back();

    if (scope.find(name.lexeme) != scope.end())
        Error::error(name, "Already a variable with this name in this scope");
    scope[name.lexeme] = false;
}
```

This adds the variable to the innermost scope so it covers the outer one so we know the variable exists. We mark it as “not ready yet” by binding its name to `false` in the scope map. The value associated with a key in the scope map represents whether or not we have finished resolving that variable’s initializer. We also check whether the scope already has a variable of the same name already defined and throw an error if there is. Then we try to resolve the initializer value if it does exist. Finally, we can continue resolving it by defining it:

```cpp
void Resolver::define(const Token& name)
{
    if (scopes.empty())
        return;
    scopes.back()[name.lexeme] = true;
}
```

The variable's value will be marked to `true` once it is fully initialized and ready to be use.

```cpp
std::any Resolver::visitMutExpr(std::shared_ptr<MutExpr> expr)
{
    if (!scopes.empty())
    {
        auto& current_scope = scopes.back();
        auto element = current_scope.find(expr->name.lexeme);

        if (element != current_scope.end() && element->second == false)
            Error::error(expr->name, "Can't read local variable in its initializer");

    }
    resolve_local(expr, expr->name);
    return {};
}
```

The variable expressions will try to read the map when they are being resolved. We check if the variable is being accessed inside its own initializer by checking the value associated with the variable. If it is false, then the variable has been declared but not defined yet. Then we resolve the variable using the `resolve_local()` function:

```cpp
void Resolver::resolve_local(std::shared_ptr<Expr> expr, const Token& name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        if (scopes[i].find(name.lexeme) != scopes[i].end())
        {
            interpreter.resolve(expr, scopes.size() - i - 1);
        }
    }
}
```

Starting with the innermost scope, we look at each map for a matching name. If we find a variable, we resolve it, passing in the number of scopes between the current innermost scope and the scope where the variable was found. So, if the variable was found in the current scope, we pass in 0. If it’s in the enclosing scope, 1. If we never find the variable, we leave it unresolved and making it a global variable. 

```cpp
std::any Resolver::visitAssignExpr(std::shared_ptr<AssignExpr> expr)
{
    resolve(expr->value);
    resolve_local(expr, expr->name);
    return {};
}
```

We can resolve expression that assign to variables like this. Resolve the expression's assigned value (in case it also contains references to other variables), and use `resolve_local()` to resolve the variable being assigned to.

```cpp
std::any Resolver::visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt)
{
    declare(stmt->name);
    define(stmt->name);

    resolve_function(stmt->fn, FunctionType::FUNCTION);
    return {};
}

void Resolver::resolve_function(std::shared_ptr<FunctionExpr> fn, FunctionType type)
{
    FunctionType enclosing_func = current_func;
    current_func = type;

    begin_scope();
    for (const Token& param : fn->parameters)
    {
        declare(param);
        define(param);
    }
    resolve(fn->body);
    end_scope();

    current_func = enclosing_func;
}
```

Functions will need to bind names and create a new scope. The name of the function is bound to the surrounding scope where the function is declared. We also bind the function's parameters into the function's scope. We handle the function statement by declaring and defining the function's name in the current scope before resolving. This lets a function recursively refer to itself inside its own body. We will resolve the function with `resolve_function()`. We store the enclosing function and the function type to keep track of how many function enclosings we're in. When we're done resolving the function body, we restore the original field to that value. This allows us to declare inner functions inside another function. We then create a new scope and bind the variables for each of the function parameters. After the parameters are done, we resolve the function's body and end scope. 

We'll also need a visit function for other nodes in the AST. You can checkout the implementation for each of those nodes [here](../src/resolver.cpp).

## Resolution interpretation

Each time the resolver visits a variable, it tells the interpreter how many scopes there are between the current scope and the scope where the variable is defined. This is basically  the number of environments between the current one and the enclosing one where the interpreter can find the variable's value. The resolver pass that number to the interpreter through the interpreter's `resolve()` function.

```cpp
std::map<std::shared_ptr<Expr>, int> locals;

void Interpreter::resolve(std::shared_ptr<Expr> expr, int depth)
{
    locals[expr] = depth;
}
```

We store that number on a map called `locals`. This allows us to access the resolution information when the variable expression or assignment expression is executed.

```cpp
std::any Interpreter::visitMutExpr(std::shared_ptr<MutExpr> expr)
{
    return lookup_mut(expr->name, expr);
}

std::any Interpreter::lookup_mut(const Token& name, std::shared_ptr<Expr> expr)
{
    auto element = locals.find(expr);

    if (element != locals.end())
    {
        int distance = element->second;
        return environment->get_at(distance, name.lexeme);
    }
    else
    {
        return globals->get(name);
    }
}
```

So we can lookup the variables like this using the `lookup_mut()` function. Because we only resolved local variables, if we don't find the element in in the map, it must be a global. If we do find it, then it's a local and we can call `get_at()` to evaluate it.

```cpp
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
```

The `get()` function of environment dynamically walks the chain of enclosing environments, searching for the variable. Since we know exactly which environment in the chain will the variable through the `distance` variable, we can call `ancestor()` to walk through the chain a fixed number of times and return the environment. Then `get_at()` returns the value of the variable in the environment's map.

```cpp
void Environment::assign_at(int distance, const Token& name, std::any value)
{
    ancestor(distance)->values[name.lexeme] = std::move(value);
}
```

We can do the same thing for assignment expressions with the `assign_at()` function. 
