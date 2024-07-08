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
