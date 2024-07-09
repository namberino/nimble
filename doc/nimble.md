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

Associativity:

| Name | Operators | Associates|
| --- | --- | --- |
|Equality | ==, != | Left |
| Comparison | >, >=, <, <= | Left |
| Term | -, + | Left |
| Factor | /, * | Left |
| Unary | !, - | Right |

Grammar rule:
```
expression -> equality
equality -> comparison ( ( "!=" | "==" ) comparison )*
comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
term -> factor ( ( "-" | "+" ) factor )*
factor -> unary ( ( "/" | "*" ) unary )*
unary -> ( "!" | "-" ) unary | primary
primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
```

> Note: `*` means it can be repeated for 0 or more times

Old grammar rule:
```
expression -> literal | unary | binary | grouping
literal -> NUMBER | STRING | "true" | "false" | "nil"
grouping -> "(" expression ")"
unary -> ("-" | "!") expression
binary -> expression operator expression
operator -> "==" | "!=" | "<" | "<=" | ">" | ">=" | "+"  | "-"  | "*" | "/"
```

Parsing technique: **recursive descent**

Statement grammar rule:
```
program -> statement* EOF
statement -> expr_statement | print_statement
expr_statement -> expression ";"
print_statement -> "print" expression ";"
```
