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

We check for the presence of a superclass first and whether the superclass is an actual superclass or not. Then we can define the class, the superclass, and create a map for storing the methods inside the function. There's also this funky `NblClass` type variable named `superklass` for storing the type casted superclass. We then create a new class object `klass` with the name, the superclass, and the methods the we just defined and created. We'll also make sure to create an enclosing environment if there's a superclass and we make sure to assign the name of the class with the created class. using the `assign()` method of the environment data structure.

```cpp
class NblClass : public NblCallable, public std::enable_shared_from_this<NblClass>
{
    friend class NblInstance;
    
    private:
        std::string name;
        std::shared_ptr<NblClass> superclass;
        std::map<std::string, std::shared_ptr<NblFunction>> methods;

    public:
        NblClass(std::string name, std::shared_ptr<NblClass> superclass, std::map<std::string, std::shared_ptr<NblFunction>> methods);
        std::shared_ptr<NblFunction> find_method(const std::string& name);
        int arity() override;
        std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
        std::string to_string() override;
};
```

This is what the class object looks like. We'll have to hold the name, the superclass and the methods mapping. We also have the arity function kinda like the function object. The reason why is because if there's an `init()` method in the class, that means the class can also be called kinda like a function when an instance is created. So we need to take the arity of the `init()` method. The class object also inherit from from the callable object, allowing us to create an instance by calling it like a function.

```cpp
std::shared_ptr<NblFunction> NblClass::find_method(const std::string& name)
{
    auto element = methods.find(name);
    
    if (element != methods.end())
        return element->second;

    if (superclass != nullptr)
        return superclass->find_method(name);

    return nullptr;
}

int NblClass::arity()
{
    std::shared_ptr<NblFunction> initializer = find_method("init");
    
    if (initializer == nullptr)
        return 0;

    return initializer->arity();
}

std::any NblClass::call(Interpreter& interpreter, std::vector<std::any> arguments)
{
    auto instance = std::make_shared<NblInstance>(shared_from_this());
    std::shared_ptr<NblFunction> initializer = find_method("init");

    if (initializer != nullptr)
        initializer->bind(instance)->call(interpreter, std::move(arguments));

    return instance;
}

std::string NblClass::to_string()
{
    return name;
}
```

The `find_method()` method will find a specific method based on the name. If the method is not in the class's method map, we recursively find it in the superclass. The `arity()` method just calls the init method's arity function. The `call()` method will first create an instance object. then we try to find the init method and if there's no initializer then we just bind the instance with the arguments passed into the initializer. Otherwise, we return the instance.

```cpp
std::shared_ptr<NblClass> klass;
std::map<std::string, std::any> fields;

NblInstance::NblInstance(std::shared_ptr<NblClass> klass)
    : klass(std::move(klass)) {}

std::any NblInstance::get(const Token& name)
{
    auto element = fields.find(name.lexeme);

    if (element != fields.end())
        return element->second;

    std::shared_ptr<NblFunction> method = klass->find_method(name.lexeme);

    if (method != nullptr)
        return method->bind(shared_from_this());

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'");
}

void NblInstance::set(const Token& name, std::any value)
{
    fields[name.lexeme] = std::move(value);
}

std::string NblInstance::to_string()
{
    return klass->name + " instance";
}
```

This instance object is a runtime representation of an instance. We also have the `get()` and `set()` methods which is mainly used for getting and setting fields within the created instance. Since all class fields in NIMBLE is public, this makes implementing the get and set methods for the instance object a lot easier. We just find the method and get the value or set the value of it without having to worry about checking for access modifiers.

## Get expression

```cpp
GetExpr::GetExpr(std::shared_ptr<Expr> object, Token name)
    : object(std::move(object)), name(std::move(name)) {}

std::any GetExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitGetExpr(shared_from_this());
}
```

Get expressions are used for accessing fields within a class's instance.

```cpp
std::any Interpreter::visitGetExpr(std::shared_ptr<GetExpr> expr)
{
    std::any object = evaluate(expr->object);

    if (object.type() == typeid(std::shared_ptr<NblInstance>))
        return std::any_cast<std::shared_ptr<NblInstance>>(object)->get(expr->name);

    throw RuntimeError(expr->name, "Only instances have properties");
}
```

We try to evaluate the expression's object whose property is being accessed. Then we check if the object is an instance of a class. Because only instances can have properties, if we can't cast the object to an instance, we throw a runtime error. If it is an instance, we call the `get()` method from `NblInstance` to access the property. The `get()` method will throw an error if the field being accessed is undefined.

## Set expression

```cpp
SetExpr::SetExpr(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
    : object(std::move(object)), name(std::move(name)), value(std::move(value)) {}

std::any SetExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitSetExpr(shared_from_this());
}
```

Set expressions are used for setting fields' values within a class's instance.

```cpp
std::any Interpreter::visitSetExpr(std::shared_ptr<SetExpr> expr)
{
    std::any object = evaluate(expr->object);

    if (object.type() != typeid(std::shared_ptr<NblInstance>))
        throw RuntimeError(expr->name, "Only instances have fields");

    std::any value = evaluate(expr->value);
    std::any_cast<std::shared_ptr<NblInstance>>(object)->set(expr->name, value);

    return value;
}
```

We try to evaluate the expression's object whose field is being accessed. Then we check if the object is an instance of a class. Because only instances can have properties, if we can't cast the object to an instance, we throw a runtime error. If it is an instance, we call the `set()` method from `NblInstance` to access the property. The `set()` method will throw an error if the field being modified is undefined.

## The "this" keyword

The "this" keyword allows us to access the fields and methods inside a class inside the class. It will be bound to the instance instead of the class.

```cpp
ThisExpr::ThisExpr(Token keyword)
    : keyword(std::move(keyword)) {}

std::any ThisExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitThisExpr(shared_from_this());
}
```

The this expression just needs to hold a keyword, which the parser can recognized. We declare "this" as a variable in that environment and bind it to the instance that the method is being accessed from.

```cpp
std::any Interpreter::visitThisExpr(std::shared_ptr<ThisExpr> expr)
{
    return lookup_mut(expr->keyword, expr);
}
```

To interpret the this expression, we use the `lookup_mut()` function to grab the variable associated with the keyword.

## Constructor

The constructor does 2 things:
- Allocate memory required for a new instance.
- Initialize the instance with the user's parameters.

When a new instance is created, we find the `init()` method. If we find one, we immediately bind and invoke it just like a normal method call. The argument list is forwarded along. Since we bind the init method before we call it, it has access to the "this".

If a function is an initilizer, we return "this" if it is called. If the user try to return a value from the init method, we'll throw an error. But if the user use just the return keyword with no return value, we'll allow that and return "this" instead of `nil`.

## Super expression

```cpp
SuperExpr::SuperExpr(Token keyword, Token method)
    : keyword(std::move(keyword)), method(std::move(method)) {}

std::any SuperExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitSuperExpr(shared_from_this());
}
```

The super expression is pretty similar to the this expression. It's a "magic" variable that allows us to access methods of the superclass. The super expression just contain the super keyword and the name of the method being looked up.

```cpp
std::any Interpreter::visitSuperExpr(std::shared_ptr<SuperExpr> expr)
{
    int distance = locals[expr];
    auto superclass = std::any_cast<std::shared_ptr<NblClass>>(environment->get_at(distance, "super"));
    auto obj = std::any_cast<std::shared_ptr<NblInstance>>(environment->get_at(distance - 1, "this"));
    std::shared_ptr<NblFunction> method = superclass->find_method(expr->method.lexeme);

    if (method == nullptr) // can't find method
        throw RuntimeError(expr->method, "Undefined property '" + expr->method.lexeme + "'");

    return method->bind(obj);
}
```

We're basically get the distance of the expression in the local environment. We also create a superclass object (for finding the methods) and an object object (for binding the method). If we can't find the method, we throw a runtime error because that means the property is undefined.
