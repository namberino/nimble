# Grammar

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

Parsing technique: **recursive descent**

Grammar rule:
```
expression -> assignment
assignment -> IDENTIFIER "=" assignment | logic_or
logic_or -> logic_and ( "or" logic_and )*
logic_and -> equality ( "and" equality )*
equality -> comparison ( ( "!=" | "==" ) comparison )*
comparison -> exponent ( ( ">" | ">=" | "<" | "<=" ) exponent )*
exponent -> term ( "**" term )*
term -> factor ( ( "-" | "+" ) factor )*
factor -> unary ( ( "/" | "*" | "%" ) unary )*
unary -> ( "!" | "-" ) unary | call
call -> primary ( "(" arguments? ")" )*
arguments -> expression ( "," expression )*
primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
```

> Note: `*` means it can be repeated for 0 or more times

Statement grammar rule:
```
program -> declaration* EOF
declaration -> var_declaration | function_declaration | statement
statement -> expression_statement | print_statement | if_statement | for_statement | while_statement | break_statement | return_statement | block
expr_statement -> expression ";"
print_statement -> "print" expression ";"
var_declaration -> "var" IDENTIFIER ( "=" expression )? ";"
function_declaration -> "fun" function
function -> IDENTIFIER "(" parameters? ")" block
parameters -> IDENTIFIER ( "," IDENTIFIER )*
block -> "{" declaration* "}"
if_statement -> "if" "(" expression ")" statement ( "else" statement )?
for_statement -> "for" "(" ( var_declaration | expression_statement | ";" ) expression? ";" expression? ")" statement
while_statement -> "while" "(" expression ")" statement
break_statement -> "break" ";"
return_statement -> "return" expression? ";"
```

Variable rules:
```
primary -> "true" | "false" | "nil" | NUMBER | STRING | "(" expression ")" | IDENTIFIER 
expression -> assignment
assignment -> IDENTIFIER "=" assignment | equality
```
