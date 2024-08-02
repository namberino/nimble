# Syntax of NIMBLE

You can learn about the syntax of the NIMBLE programming language here.

## Data type

- Booleans: `true` and `false`
- Doubles: The only numeric type is double ($1.0$, $5.2$, etc)
- Strings: String literals enclosed by double quotes ("Hello world")
- Nil: nil type (nil)

## Expression

> If built-in data types are atoms then expressions must be molecules.

### Arithmetic

```nimble
add1 + add2;
sub1 - sub2;
mul1 * mul2;
div1 / div2;
mod1 % mod2;
pow1 ** pow2;
```

These are called binary operators (because there're 2 of them). The expression on either side of the operator are called operands

The `-` operator can also be used to negate a number

```nimble
-negative_num;
```

All these operator works on numbers only. Except for the `+` operator, which can work on 2 strings, 2 numbers, or 1 string and 1 number.

## Compound operations

NIMBLE supports compound operation for 4 different arithmetic symbols:

```nimble
add1 += 1;
sub1 -= 1;
mul1 *= 1;
div1 /= 1;
```

### Comparison

These operators return boolean. They can be used to compare values

```nimble
lt1 < lt2;
le1 <= le2;
gt1 > gt2;
ge1 >= ge2;
```

```nimble
1 == 2; // false
"hello" == "hello"; // true
1 == "1"; // false
"not" != "Not"; // true
```

### Logical operators

The `!` is the *not* operator

```nimble
!true; // false
!false; // true
```

There's also the `and` and `or` operator.

The `and` operator will return the left operand if it is false, and the `or` operator will return the left operand if it is true.

```nimble
false and true; // false
true or false; // true
```

### Precedence

Basic mathematical precedence is here, you can also use parentheses to group stuff

```nimble
mut avg = (n1 + n2 + n3) / 3;
```

## Statement

> If an expression's job is to produce a value, then a statement's job is to produce an effect.

Statements don't evaluate to a value. They usually modify some state, read input, or produce output.

An example is the `print` statement

```nimble
print("Hello world");
```

Another example

```nimble
"expression here";
```

An expression followed by a semicolon will get promoted to an expression statement.

We can use a *block* to pack a series of statements

```nimble
{
    print("hello");
    print("world");
}
```

> Blocks also affect scoping

## Variables

Declare variables with the `mut` keyword. By default, variables evaulate to `nil`.

```nimble
mut v1 = "variable";
mut v2; // nil
```

Variables also support reassignation

```nimble
mut v1 = "variable";
print(v1);
v1 = "reassigned";
print(v1);
```

## Control flow

Use `if` and `else` statements for control flow

```nimble
if (condition)
{
    print("yes");
}
else if (condition2)
{
    print("maybe");
}
else
{
    print("no");
}
```

Use `for` and `while` loops for iteration

```nimble
for (mut i = 0; i < 10; i = i + 1)
{
    print(i);
}

mut i = 0;
while (i < 10)
{
    print(i);
    i = i + 1;
}
```

## List

There's support for list expressions

```nimble
mut li = [1, 2, 3];
print(li); // 1, 2, 3
print(li[0]); // 1

li[0] = 4;
li[4] = 5; // appending 5
print(li); // 4, 2, 3, 5
```

## Function

Function call is pretty much the same as most other languages.

```nimble
func1(a1, a2, a3);
func2();
```

Parentheses are mandatory for function call. Leaving the parentheses off will just refer to the function, not calling it.

Function definition syntax is quite similar to C, but it's a lot more "*fun*"

```nimble
fun add(a, b)
{
    print("Adding...");
    return a + b;
}

print(add(1, 2)); // 3
```

You can pass in arguments to the function, the function will have a parameter list used to store those arguments.

If execution reaches the end of the block without hitting a `return`, it implicitly returns `nil`.

Since function declarations are statements, you can declare local functions inside another function.

```nimble
fun func1()
{
    fun func2()
    {
        print("Local function");
    }

    func2();
}
```

```nimble
fun return_func()
{
    mut outside = "outside";

    fun inner()
    {
        print(outside);
    }

    return inner;
}

mut f = return_func();
f();
```

`inner()` holds on to references to any surrounding variables that it uses so that they can stay around even after the outer function has returned. Functions like this are called *closures*.

## Classes

You can declare a class and methods in the class's body.

```nimble
class GordonRamsay
{
    cook()
    {
        print("I'm cooking");
    }

    yell(who)
    {
        print(who + " is an idiot sandwich");
    }
}
```

When class declaration is executed, the language creates a class object and stores that in a variable named after the class.

```nimble
mut variable = GordonRamsay; // store it in variables
func(GordonRamsay); // pass it to function
```

The class itself is a factory function for instances. Calling a class will produce a new instance of itself.

```nimble
mut chef = GordonRamsay();
print(chef); // GordonRamsay instance
```

You can also freely add properties to the object. Assigning to a field will create it if it doesn't already exist.

```nimble
GordonRamsay.line = "Oh dear";
GordonRamsay.insult = "You donkey";
```

Accessing the field or method on a current object can be done through the use of the `this` keyword.

```nimble
class GordonRamsay
{
    yell(who)
    {
        print(this.line + ". What have you done, " + insult);
    }
    
    // ...
}
```

You can also use the `init()` method to correctly initialize a class. This is bascially the constructor method.

```nimble
class GordonRamsay
{
    init(line, insult)
    {
        this.line = line;
        this.insult = insult;
    }

    // ...
}

mut chef = GordonRamsay("Oh dear", "You donkey");
chef.yell("you");
```

You can also use `:` for inheritance, this mimics the C++ inheritance syntax. Here, `GordonRamsay` is the subclass and `Chef` is the superclass

```nimble
class Brunch : Breakfast
{
    drink()
    {
        print("I want vodka");
    }
}

mut t = Brunch("ham", "muffin");
t.serve("me");
```

Every method defined in the superclass is available to the subclasses.

There's support for the `super` keyword too.

```nimble
class Brunch : Breakfast
{
    init(meat, bread, drink)
    {
        super.init(meat, bread);
        this.drink = drink;
    }
}
```

## Built-in functions

Here's a list of built-in functions:

- `clock()`: The main purpose of this is for benchmarking, returns the number of seconds since the program started (takes in no argument)
- `time()`: Return the current time (takes in no argument)
- `input()`: Prompt input from the user (takes in 1 argument: the prompt)
- `exit()`: Exit the interpreter (optionally takes in 1 argument: the exit code, if there's no argument then it will exit with the code *0* by default)
