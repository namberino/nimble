# Statements

Statements allow us to produce output or modify some state in the interpreter that can be utilized later, this is called *side effect*. This also makes them great for defining other variables and other named entities.

The first grammar rule for statement is `program`, which is the starting point for grammar and represents a complete NIMBLE script or REPL entry. It's a list of statements followed by an end of file token.

You can checkout the statement grammar [here](grammar.md). Let's checkout some statement for example on how they work.

## Expression statement

```cpp
ExpressionStmt::ExpressionStmt(std::shared_ptr<Expr> expression) 
    : expression(std::move(expression)) {}

std::any ExpressionStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitExpressionStmt(shared_from_this());
}
```

This lets you place an expression where a statement is expected. They're here to evaluate expressions that have side effects. Any time you see a function or method call followed with a `;`, that's an expression statement.

## Print statement

```cpp
PrintStmt::PrintStmt(std::shared_ptr<Expr> expression) 
    : expression(std::move(expression)) {}

std::any PrintStmt::accept(StmtVisitor& visitor)
{
    return visitor.visitPrintStmt(shared_from_this());
}
```

This lets you evaluate an expression and print the result out into the terminal.