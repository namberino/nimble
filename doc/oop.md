# OOP in NIMBLE

This is a big topic. Nowadays many modern programming languages have support for OOP. NIMBLE also has support for OOP, but it's not forced onto the user (like how Java does it), so if you don't like OOP, you can just not use it (kinda like languages like Python). This tool gives the user options to use it when they write big programs.

## Classes

A class bundle together data and operations. A class needs to be able to do 3 things:
1. Have a constructor that will help initialize new instances of the class.
2. Provide a way to store and access fields.
3. Define methods that can operate on instances.

We'll also throw in inheritance because that's pretty useful for refactoring and reusing.

```cpp
ClassStmt::ClassStmt(Token name, std::shared_ptr<MutExpr> superclass, std::vector<std::shared_ptr<FunctionStmt>> methods)
    : name(std::move(name)), superclass(std::move(superclass)), methods(std::move(methods)) {}

std::any ClassStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitClassStmt(shared_from_this());
}
```

The class statements (used for class declaration) will hold 3 things: the name of the current class being declared, the name of the superclass that this class inherits from (if it does inherit from another superclass), and the methods inside this class's body. The methods will be represented by the `FunctionStmt` struct. The class will be initialized with the `class` keyword.

```cpp
std::shared_ptr<Stmt> Parser::class_declaration()
{
    Token name = consume(IDENTIFIER, "Expected class name");

    std::shared_ptr<MutExpr> superclass = nullptr;
    if (match(COLON))
    {
        consume(IDENTIFIER, "Expected superclass name");
        superclass = std::make_shared<MutExpr>(previous());
    }

    consume(LEFT_BRACE, "Expected '{' before class body");
    std::vector<std::shared_ptr<FunctionStmt>> methods;

    while (!check(RIGHT_BRACE) && !is_at_end())
        methods.push_back(function("method"));

    consume(RIGHT_BRACE, "Expected '}' after class body");

    return std::make_shared<ClassStmt>(std::move(name), std::move(superclass), std::move(methods));
}
```

We consume the class name, then we check for curly braces. When we're inside the class body, we parse the method declarations until we hit the closing curly brace. Methods are handled by `function()`. And finally, the data is wrapped into a class statement. 

```cpp
std::any Interpreter::visitClassStmt(std::shared_ptr<ClassStmt> stmt)
{
    std::any superclass;
    if (stmt->superclass != nullptr)
    {
        superclass = evaluate(stmt->superclass);

        if (superclass.type() != typeid(std::shared_ptr<NblClass>))
            throw RuntimeError(stmt->superclass->name, "Superclass must be a class");
    }

    environment->define(stmt->name.lexeme, nullptr);

    if (stmt->superclass != nullptr)
    {
        environment = std::make_shared<Environment>(environment);
        environment->define("super", superclass);
    }

    std::map<std::string, std::shared_ptr<NblFunction>> methods;
    for (std::shared_ptr<FunctionStmt> method : stmt->methods)
    {
        bool is_method_init = method->name.lexeme == "init";
        auto function = std::make_shared<NblFunction>(stmt->name.lexeme, method->fn, environment, is_method_init);
        methods[method->name.lexeme] = function;
    }

    std::shared_ptr<NblClass> superklass = nullptr;
    if (superclass.type() == typeid(std::shared_ptr<NblClass>))
        superklass = std::any_cast<std::shared_ptr<NblClass>>(superclass);

    auto klass = std::make_shared<NblClass>(stmt->name.lexeme, superklass, methods);

    if (superklass != nullptr)
        environment = environment->enclosing;

    environment->assign(stmt->name, std::move(klass));

    return {};
}
```
