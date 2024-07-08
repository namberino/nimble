# The NIMBLE Programming Language

## Abstract Syntax Tree

![ast](img/ast.jpg)

We use post-order traversal to figure out which node to visit first, which operation to compute first.

## Grammar

| Terminology | Lexical Grammar | Syntactic Grammar |
| --- | --- | --- |
| Alphabet | Characters | Tokens |
| String | Lexeme / Token | Expression |
| Implemented by | Scanner | Parser |

- Literals: numbers, strings, booleans, nil
- Unary expressions: a prefix '!' to perform a logical not, a '-' to negate a number
- Binary expressions: The arithmetic operations (+, -, *, /) and logic operators (==, !=, <, <=, >, >=)
- Parentheses: A pair of '(' and ')' wrapped around an expression

Example:
```
1 - (2 * 3) < 4 == false
```

Grammar:
```
expression -> literal | unary | binary | grouping
literal -> NUMBER | STRING | "true" | "false" | "nil"
grouping -> "(" expression ")"
unary -> ("-" | "!") expression
binary -> expression operator expression
operator -> "==" | "!=" | "<" | "<=" | ">" | ">=" | "+"  | "-"  | "*" | "/"
```
