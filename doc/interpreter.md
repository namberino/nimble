# The interpreter

This is where the magic happens. Where our tokens, expressions, and AST that we created from the lexer and the parser gets interpreted and executed.

## Value representation

Values are created by literals, computed by expression, and stored in variables. A variable can store any type in this language, and can even store values of different types at different points in time.

The type of the variables in our programming language will need to be determined at runtime. So `std::any` was employed to help do this. Then we can use `std::any_cast<>()` to help cast the `std::any` object into other types for operation. We can determine the type using the `.type()` function of `std::any` along with the `typeid()` function

| NIMBLE type | C++ type |
| --- | --- |
| Any nbl type | std::any |
| nil | nullptr |
| boolean | bool |
| number | double |
| string | std::string |

Example (Checking for booleans):

```cpp
if (obj1.type() == typeid(bool) && obj2.type() == typeid(bool))
    return std::any_cast<bool>(obj1) == std::any_cast<bool>(obj2);
```

## Expression evaluation

We'll use the [Visitor patter](https://en.wikipedia.org/wiki/Visitor_pattern) for the interpreter. It takes in an AST, and it recursively traverses it, computes the values, which will eventually get returned.

```cpp
class Interpreter : public ExprVisitor, public StmtVisitor
```

The class inherits from the visitor classes. It will implement these visitor methods, and each expression visitor method will return an `std::any`.

### Literals

```cpp
std::any Interpreter::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr)
{
    // evaluating literals
    return expr->value;
}
```

The leaves of the AST will be literals. The literals are syntaxes that produces a value, **not** values computed at runtime. That distinction needs to be clear. The values are a part of the runtime domain. So when we evaluate a literal node, we need to convert the literal into runtime values.

## Groupings

```cpp
std::any Interpreter::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr)
{
    // evaluating parentheses
    return evaluate(expr->expression);
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    // send expression back into interpreter's visitor methods for evaluation
    return expr->accept(*this);
}
```

Groupings happens as a result of expressions inside of parentheses. The grouping node will have a reference to the inner node for the expression contained inside the parentheses. So all we need to do is to evaluate the subexpression and return it.

### Unaries

```cpp
std::any Interpreter::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr)
{
    // evaluate unary expressions
    std::any right = evaluate(expr->right);

    switch (expr->op.type)
    {
        case BANG:
            return !is_truthy(right);

        case MINUS:
            check_num_operand(expr->op, right);
            return -std::any_cast<double>(right);
        
        default:
            break;
    }

    return {}; // unreachable, here to make the compiler happy
}

bool Interpreter::is_truthy(const std::any& obj)
{
    if (obj.type() == typeid(nullptr))
        return false;
    
    if (obj.type() == typeid(bool))
        return std::any_cast<bool>(obj);
    
    return true;
}

void Interpreter::check_num_operand(const Token& op, const std::any& operand)
{
    if (operand.type() == typeid(double))
        return;
    
    throw RuntimeError{op, "Operand must be a number"};
}
```

Unary expressions also have a subexpression to evaluate first. Then we evaluate the operator with the evaluated expression. Firstly, we need someway to check if the type of the variables matches what needs to be evaluated.

For the bang sign (NOT operation), we need to check for 2 cases first, if it's a `nil` type, return false, if it's a `boolean` type, return the boolean value, else, for everything other than those 2 will be evaluated to true. Then the bang operation will handle 'not-ing' the boolean cases.

For the dash sign (negate operation), since we can only negate numbers, we need to check for whether the operand is a number or not. If not then we throw a runtime error.

### Binaries

```cpp
std::any Interpreter::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr)
{
    // evaluate binary operators
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type)
    {
        // comparisors
        case BANG_EQUAL: return !is_equal(left, right);
        case EQUAL_EQUAL: return is_equal(left, right);
        case GREATER:
            check_num_operands(expr->op, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case GREATER_EQUAL:
            check_num_operands(expr->op, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case LESS:
            check_num_operands(expr->op, left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case LESS_EQUAL:
            check_num_operands(expr->op, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        case STAR_STAR:
            check_num_operands(expr->op, left, right);
            return pow(std::any_cast<double>(left), std::any_cast<double>(right));

        // arithmetics
        case PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) + std::any_cast<double>(right);

            if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);

            if (left.type() == typeid(std::string) && right.type() == typeid(double))
                return std::any_cast<std::string>(left) + std::to_string(std::any_cast<double>(right));

            if (left.type() == typeid(double) && right.type() == typeid(std::string))
                return std::to_string(std::any_cast<double>(left)) + std::any_cast<std::string>(right);

            throw RuntimeError{expr->op, "Operands must be 2 numbers, 2 strings, or 1 number and 1 string"};
        case MINUS:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) - std::any_cast<double>(right);
        case STAR:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) * std::any_cast<double>(right);
        case SLASH:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) / std::any_cast<double>(right);
        case PERCENT:
            if (left.type() == typeid(double) && right.type() == typeid(double))
                return fmod(std::any_cast<double>(left), std::any_cast<double>(right));
    
        default: break;
    }

    return {}; // unreachable, here to make the compiler happy
}

void Interpreter::check_num_operands(const Token& op, const std::any& left, const std::any& right)
{
    if (left.type() == typeid(double) && right.type() == typeid(double))
        return;
    
    throw RuntimeError{op, "Operands must be numbers"};
}

bool Interpreter::is_equal(const std::any& obj1, const std::any& obj2)
{
    if (obj1.type() == typeid(nullptr) && obj2.type() == typeid(nullptr))
        return true;

    if (obj1.type() == typeid(nullptr))
        return false;

    if (obj1.type() == typeid(std::string) && obj2.type() == typeid(std::string))
        return std::any_cast<std::string>(obj1) == std::any_cast<std::string>(obj2);

    if (obj1.type() == typeid(double) && obj2.type() == typeid(double))
        return std::any_cast<double>(obj1) == std::any_cast<double>(obj2);

    if (obj1.type() == typeid(bool) && obj2.type() == typeid(bool))
        return std::any_cast<bool>(obj1) == std::any_cast<bool>(obj2);

    return false;
}
```

This is a long one.

The arithmetic ones are pretty self explanatory. The "plus" one operates a bit differently, it operates kinda like python's plus operation. it can add up 2 doubles, it can concatenate 2 strings, it can also concatenate a string and a double.

For the comparisors, we need to check whether the 2 operands are actual numbers as we can only compare numbers. If they are numbers then we perform comparison on the 2 of them. There are 2 exceptions though, `!=` and `==`. For these 2 comparisors, we can compare strings, bools, numbers, and nils with each other.

## Interpreting

```cpp
std::string Interpreter::interpret(const std::shared_ptr<Expr>& expr)
{
    try
    {
        std::any value = evaluate(expr);
        return stringify(value);
    }
    catch(RuntimeError error)
    {
        Error::runtime_error(error);
        return "";
    }
}

std::string Interpreter::stringify(const std::any& obj)
{
    if (obj.type() == typeid(nullptr))
        return "nil";

    if (obj.type() == typeid(double))
    {
        std::string text = std::to_string(std::any_cast<double>(obj));

        if (text[text.length() - 2] == '.' && text[text.length() - 1] == '0')
            text = text.substr(0, text.length() - 2);

        return text;
    }

    if (obj.type() == typeid(std::string))
        return std::any_cast<std::string>(obj);

    if (obj.type() == typeid(bool))
        return std::any_cast<bool>(obj) ? "true" : "false";
    
    if (obj.type() == typeid(std::shared_ptr<NblFunction>))
        return std::any_cast<std::shared_ptr<NblFunction>>(obj)->to_string();

    if (obj.type() == typeid(std::shared_ptr<NblClass>))
        return std::any_cast<std::shared_ptr<NblClass>>(obj)->to_string();

    if (obj.type() == typeid(std::shared_ptr<NblInstance>))
        return std::any_cast<std::shared_ptr<NblInstance>>(obj)->to_string();
    
    if (obj.type() == typeid(std::shared_ptr<NativeClock>))
        return std::any_cast<std::shared_ptr<NativeClock>>(obj)->to_string();

    if (obj.type() == typeid(std::shared_ptr<NativeTime>))
        return std::any_cast<std::shared_ptr<NativeTime>>(obj)->to_string();
    
    if (obj.type() == typeid(std::shared_ptr<NativeInput>))
        return std::any_cast<std::shared_ptr<NativeInput>>(obj)->to_string();

    if (obj.type() == typeid(std::shared_ptr<NativeExit>))
        return std::any_cast<std::shared_ptr<NativeExit>>(obj)->to_string();

    if (obj.type() == typeid(std::shared_ptr<NativeFloorDiv>))
        return std::any_cast<std::shared_ptr<NativeFloorDiv>>(obj)->to_string();

    if (obj.type() == typeid(std::shared_ptr<ListType>))
    {
        std::string result = "[";
        std::shared_ptr<ListType> list = std::any_cast<std::shared_ptr<ListType>>(obj);
        std::vector<std::any> elements = list->elements;

        for (auto i = elements.begin(); i != elements.end(); i++)
        {
            auto next = i + 1;

            result.append(stringify(*i));

            if (next != elements.end())
                result.append(", ");
        }

        result.append("]");
        return result;
    }

    return "Error in stringify: Invalid object type";
}
```

This will take in an AST for an expression and evaluates it. If it works, `evaluate()` will return an `std::any`. Then we can convert this object into strings to display those values.

The `stringify()` function basically let's us turn an `std::any` object into strings for printing. For primitive types like strings, double, boolean, they're pretty simple, but for more complex objects, we need to do more. For each of the built-in functions and objects that the language supports, we need to implement a `to_string()` function for them. For lists specifically, we need to format them a bit differently, we can access each of the elements that the `ListType` object holds and recursively call the `stringify()` function on each elements, then storing it in a result string.

## Statements execution

Our interpreter class will also implements the statement visitor virtual struct:

```cpp
struct StmtVisitor
{
    virtual ~StmtVisitor() = default;
    virtual std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) = 0;
    virtual std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) = 0;
    virtual std::any visitMutStmt(std::shared_ptr<MutStmt> stmt) = 0;
    virtual std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) = 0;
    virtual std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) = 0;
    virtual std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) = 0;
    virtual std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) = 0;
    virtual std::any visitBreakStmt(std::shared_ptr<BreakStmt> stmt) = 0;
    virtual std::any visitClassStmt(std::shared_ptr<ClassStmt> stmt) = 0;
    virtual std::any visitImportStmt(std::shared_ptr<ImportStmt> stmt) = 0;
};
```

So the interpreter class will also need to implement a visitor function for each of the statement types. Let's checkout 2 as an example:

### Expression statement

```cpp
std::any Interpreter::visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt)
{
    evaluate(stmt->expression);
    return {};
}
```

Since statements produces no values (they only modify the states), we just return nothing. The expression statement just needs to be evaluated using the `evaluate()` function.

### Print statement

```cpp
std::any Interpreter::visitPrintStmt(std::shared_ptr<PrintStmt> stmt)
{
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) + "\n";
    return {};
}
```

The print statement will need to evaluate the inner expression to derive a value from it then stringify it for printing.

## Interpreting the statements from parser

We can interpret the statements outputed by the parser by making another function to do it.

```cpp
void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements)
{
    try
    {
        for (const std::shared_ptr<Stmt>& statement : statements)
            execute(statement);
    }
    catch (RuntimeError error)
    {
        Error::runtime_error(error);
    }
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt)
{
    stmt->accept(*this);
}
```

This just execute each of the statements in the statement list by calling the accept function of the statement, which calls the statement's visitor function based on its type.

## Evaluating variables in interpreter

Refer to [environment.md](environment.md) to learn more about the environment data structure.

We can initialize an environment object in the interpreter class so that the variables stay in memory as long as the interpreter is running.

```cpp
std::any Interpreter::visitMutStmt(std::shared_ptr<MutStmt> stmt)
{
    std::any value = nullptr;

    if (stmt->initializer != nullptr)
        value = evaluate(stmt->initializer);

    environment->define(stmt->name.lexeme, std::move(value));
    
    return {};
}
```

We'll need to evaluate the statement's initializer if it does have one, if not then we just assign the variable's value to a `nullptr`. Then add the variable to the environment data structure.


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

This allows us to find the expression at a local level first, then find it in the global level. This will allow us to access local and global variables.
