# Control flow

Control flow is the most important part about a programming language. Let's see how control flows are implemented in NIMBLE.

## Logical operations

There are 2 logical operations in this language: `and` and `or`. We can express these operations with a logical expression struct (we can evaluate them kinda like the arithmetic operations):

```cpp
LogicalExpr::LogicalExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

std::any LogicalExpr::accept(ExprVisitor& visitor)
{
    return visitor.visitLogicalExpr(shared_from_this());
}
```

This is pretty similar to the binary expression. We can parse the `and` and `or` expressions like this:

```cpp
std::shared_ptr<Expr> Parser::or_expression()
{
    std::shared_ptr<Expr> expression = and_expression();

    while (match(OR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = and_expression();
        expression = std::make_shared<LogicalExpr>(expression, std::move(op), right);
    }

    return expression;
}

std::shared_ptr<Expr> Parser::and_expression()
{
    std::shared_ptr<Expr> expression = equality();

    while (match(AND))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expression = std::make_shared<LogicalExpr>(expression, std::move(op), right);
    }

    return expression;
}
```

Finally, we can interpret these 2 operations like this:

```cpp
std::any Interpreter::visitLogicalExpr(std::shared_ptr<LogicalExpr> expr)
{
    std::any left = evaluate(expr->left);

    if (expr->op.type == OR)
    {
        if (is_truthy(left))
            return left;
    }
    else
    {
        if (!is_truthy(left))
            return left;
    }

    return evaluate(expr->right);
}
```

We evaluate the left operand first, check the value to see if we should return or not based on the left operand, then we evaluate the right value. This also allows us perform logical operations on operands that aren't the same type.

```nimble
// this is a valid line in nimble
print("string" or 2); // prints the left side operand
```

## Conditionals

```cpp
IfStmt::IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch)
    : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

std::any IfStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitIfStmt(shared_from_this());
}
```

An `if` statement has an expression for the condition, then a statement to execute if the condition is truthy. It might also have an `else` keyword and a statement to execute if the condition is falsey.

If the parser found an `if` keyword, it will start parsing the statement:

```cpp
std::shared_ptr<Stmt> Parser::if_statement()
{
    consume(LEFT_PAREN, "Expected '(' after 'if' statement");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after 'if' condition");

    std::shared_ptr<Stmt> then_branch = statement();
    std::shared_ptr<Stmt> else_branch = nullptr;

    if (match(ELSE))
        else_branch = statement();

    return std::make_shared<IfStmt>(condition, then_branch, else_branch);
}
```

The else branch is initialized to a `nullptr` so we can chain if statements if we want to. We only parse else statements if we encounter them after we encountered an if statement.

When it comes to binding else statements to if statements, an else statement is always bound to the nearest if statement that precedes it. The `if_statement()` functions does that, we wait for the else branch to finish parsing before we return.

```cpp
std::any Interpreter::visitIfStmt(std::shared_ptr<IfStmt> stmt)
{
    if (is_truthy(evaluate(stmt->condition)))
        execute(stmt->then_branch);
    else if (stmt->else_branch != nullptr)
        execute(stmt->else_branch);
    
    return {};
}
```

The code is pretty self-explanatory. If the condition is true, execute it. If it's not true and there's an else statement, execute the else statement.

## Loopings

There are 2 supported looping statements: `for` loops and `while` loops.

### While loop

```cpp
WhileStmt::WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
    : condition(std::move(condition)), body(std::move(body)) {}

std::any WhileStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitWhileStmt(shared_from_this());
}
```

The while statement node will store the condition and the body.

```cpp
std::shared_ptr<Stmt> Parser::while_statement()
{
    try
    {
        loop_depth++;

        consume(LEFT_PAREN, "Expected '(' after 'while' statement");
        std::shared_ptr<Expr> condition = expression();
        consume(RIGHT_PAREN, "Expected ')' after 'while' condition");
        std::shared_ptr<Stmt> body = statement();

        return std::make_shared<WhileStmt>(condition, body);
    }
    catch (...) // throw error for trying to use break outside a loop
    {
        loop_depth--;
        throw;
    }

    loop_depth--;
}
```

The parsing for this is pretty simple. There's a new variable called `loop_depth`, which allows us to keep track of how deep we are in the loop stack, allows us to track enclosing loops. The `while_statement()` function will also catch when a break is used outside of a loop. If it does catch that, it will throw a syntax error.

```cpp
std::any Interpreter::visitWhileStmt(std::shared_ptr<WhileStmt> stmt)
{
    try
    {
        while (is_truthy(evaluate(stmt->condition)))
            execute(stmt->body);
    }
    catch (BreakException* bex)
    {
        // pass
    }

    return {};
}
```

For interpreting, it just use a C++ while loop by evaluating the condition and execute the body. We also try to catch the break exception to break out of the loop. The break exception will occur when we hit a `break`.

### For loop

For `for` loops, the syntax is similar to C's for loops. There are 3 clauses in the condition expression:
- Initializer: Executed once, is an expression or a variable declaration. If it is a variable declaration, the variable will last for the scope of the for loop
- Condition: This dictates when to end the loop. It's evaluated at the beginning of each iteration.
- Increment/Decrement: Operate on the variable at the end of each iteration. It's usually used to increment or decrement the variable.

Any one of those clauses can be omitted. Example valid expression:

```nimble
for (;;)
{
    // block
}
```
