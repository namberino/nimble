# Functions

Programming is basically applied mathematics. And when it comes to math, functions are vital. So we'll need functions support in this language.

## Function call

```cpp
CallExpr::CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
    : callee{std::move(callee)}, paren{std::move(paren)}, arguments{std::move(arguments)} {}

std::any CallExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitCallExpr(shared_from_this());
}
```

We store the callee (any expression that represents a function), a token for closing parenthesis (for runtime error), and a list of expressions for the arguments.

```cpp
std::shared_ptr<Expr> Parser::call()
{
    std::shared_ptr<Expr> expr = subscript();

    while (true)
    {
        if (match(LEFT_PAREN))
        {
            expr = finish_call(expr);
        }
        else if (match(DOT))
        {
            Token name = consume(IDENTIFIER, "Expected property name after '.'");
            expr = std::make_shared<GetExpr>(expr, std::move(name));
        }
        else if (match(LEFT_BRACKET)) // handle subscript
        {
            expr = finish_subscript(expr);
        }
        else
        {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Expr> Parser::finish_call(std::shared_ptr<Expr> callee)
{
    std::vector<std::shared_ptr<Expr>> arguments;

    if (!check(RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 255)
                error(peek(), "Can't have more than 255 arguments");

            arguments.push_back(expression());
        } while (match(COMMA));
    }

    Token paren = consume(RIGHT_PAREN, "Expected ')' after arguments");

    return std::make_shared<CallExpr>(callee, std::move(paren), std::move(arguments));
}
```

The `call()` function will handle the function calls, class get expressions as well as subscripts. We parse the left operand to the call, then each time we see a left parenthesis, we call `finish_call()` to parse the expression. The `finish_call()` function checks for right parenthesis and handle the arguments accordingly. If there are no arguments, we just consume the right parenthesis and make a call expression. If there are arguments, make sure the number of arguments is under 255 (because wny would you want a function with more than 10 arguments, like seriously). The commas are the separators for the arguments, if there's not comma left, we stop parsing the arguments.

```cpp


std::any Interpreter::visitCallExpr(std::shared_ptr<CallExpr> expr)
{
    std::any callee = evaluate(expr->callee);
    std::vector<std::any> arguments;

    for (const std::shared_ptr<Expr>& argument : expr->arguments)
        arguments.push_back(evaluate(argument));

    // pointers in a std::any wrapper must be unwrapped before they can be cast
    std::shared_ptr<NblCallable> function;

    if (callee.type() == typeid(std::shared_ptr<NblFunction>))
    {
        function = std::any_cast<std::shared_ptr<NblFunction>>(callee);
    }
    else if (callee.type() == typeid(std::shared_ptr<NblClass>))
    {
        function = std::any_cast<std::shared_ptr<NblClass>>(callee);
    }
    else if (callee.type() == typeid(std::shared_ptr<NativeClock>))
    {
        function = std::any_cast<std::shared_ptr<NativeClock>>(callee);
    }
    else if (callee.type() == typeid(std::shared_ptr<NativeTime>))
    {
        function = std::any_cast<std::shared_ptr<NativeTime>>(callee);
    }
    else if (callee.type() == typeid(std::shared_ptr<NativeInput>))
    {
        function = std::any_cast<std::shared_ptr<NativeInput>>(callee);
    }
    else if (callee.type() == typeid(std::shared_ptr<NativeExit>))
    {
        std::shared_ptr<NativeExit> func = std::any_cast<std::shared_ptr<NativeExit>>(callee);
        func->param_count = arguments.size() > 0 ? 1 : 0;
        function = func;
    }
    else if (callee.type() == typeid(std::shared_ptr<NativeFloorDiv>))
    {
        function = std::any_cast<std::shared_ptr<NativeFloorDiv>>(callee);
    }
    else if (callee.type() == typeid(std::shared_ptr<NativeArrayLen>))
    {
        function = std::any_cast<std::shared_ptr<NativeArrayLen>>(callee);
    }
    else
    {
        throw RuntimeError(expr->paren, "Can only call functions");
    }

    if (arguments.size() != function->arity())
        throw RuntimeError(expr->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()));

    return function->call(*this, std::move(arguments));
}
```

For interpreting function calls, this is a long one. We evaluate the expression for the callee, which is just an identifier for looking up functions. We also evaluate each arguments. Then we do the call by casting the callee to an virtual `NblCallable` class and calling the `call()` method on it. If we encounter an expression that doesn't match any of the classes that we specified here, then it can't be a function or a class, so we throw a runtime error. We also check for the arity of the callee (arity means the expected number of arguments) by calling the `arity()` function. If the number of arguments doesn't match with the arity, we'll also throw a runtime error. If it passes all of those error check, we call the function with the `call()` method and pass into that method the arguments.

```cpp
class NblCallable
{
    public:
        virtual ~NblCallable() = default;
        virtual int arity() = 0;
        virtual std::any call(Interpreter& interpreter, std::vector<std::any> arguments) = 0;
        virtual std::string to_string() = 0;
};
```

With this callable class, we can implement a function class for our language, you can check the implementation of it [here](../src/function.cpp).
