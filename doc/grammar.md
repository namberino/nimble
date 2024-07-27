# Grammar

Grammar is extremely important as it helps us define exactly how our programming language will work and how its syntax is shaped.

Programming languages relies heavily on *formal grammar*. Formal grammar takes a set of atomic pieces that it calls the *alphabet*, and defines a set of strings that are in the grammar. Each string is a sequence of letters in the alphabet.

## Context-free grammars

| Terminology | Lexical Grammar | Syntactic Grammar |
| --- | --- | --- |
| Alphabet | Characters | Tokens |
| String | Lexeme / Token | Expression |
| Implemented by | Scanner | Parser |

In lexical grammar, the alphabet consists of individual characters and "strings" that are valid lexemes ("words").

In syntactic grammar, the alphabet is actually an entire token and a "string" is a sequence of tokens (an expression).

A formal grammar's job is to specify which strings are valid and which aren't. A grammar rule will allow us to derive strings from the grammar rules.

Each rule in a grammar has a head (name) and a body (what it generates, which is a list of symbols).

2 types of symbols:
- Terminal: A letter from the grammar's alphabet. It's the individual lexemes (Eg. `if`, `"hello"`, etc).
- Nonterminal: A named reference to another rule in the grammar rules.

These grammar rules are the building block for a programming language's syntax and AST.

To avoid ambiguity in operation order, we need to determine the precedence and associativity. Precedence means which operator to evaluate first in an expression. Associativity means which operator to evaluate first in a series of the same operator.

Example:
- Left associative: `4 - 2 - 1` is equivalent to `(4 - 2) - 1`.
- Right associative: `a = b = c` is equivalent to `a = (b = c)`.

## NIMBLE's Grammar rule

- Literals: *numbers*, *strings*, *booleans*, *nil*.
- Unary expressions: A prefix `'!'` to perform a logical not, a prefix `'-'` to negate a number.
- Binary expressions: The arithmetic operations (`+`, `-`, `*`, `/`) and logic operators (`==`, `!=`, `<`, `<=`, `>`, `>=`).
- Parentheses: A pair of `'('` and `')'` wrapped around an expression.

Example:
```
1 - (2 * 3) < 4 == false
```

Associativity:

| Name | Operators | Associates|
| --- | --- | --- |
| Equality | ==, != | Left |
| Comparison | >, >=, <, <= | Left |
| Term | -, + | Left |
| Factor | /, * | Left |
| Unary | !, - | Right |

Note on the notation: For defining the grammar rules, the notation that I use is based on the [Backus–Naur form](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form)

Each rule has a name, followed by a `:=`, then a sequence of symbols, and the terminals are quoted. `*` means the rule can be repeated for 0 or more times. `|` means or. Parentheses can be used for grouping. `?` means optional, the rule doesn't necessarily need to be complied. And the symbols here represent tokens. Capitalized terminals are single lexemes whose text representation may vary.

Grammar rule:
```
expression := assignment
assignment := ( call "." )? IDENTIFIER "=" assignment | logic_or
logic_or := logic_and ( "or" logic_and )*
logic_and := equality ( "and" equality )*
equality := comparison ( ( "!=" | "==" ) comparison )*
comparison := exponent ( ( ">" | ">=" | "<" | "<=" ) exponent )*
exponent := term ( "**" term )*
term := factor ( ( "-" | "+" ) factor )*
factor := unary ( ( "/" | "*" | "%" ) unary )*
unary := ( "!" | "-" ) unary | call
call := subscript ( "(" arguments? ")" | "." IDENTIFIER )*
subscript := primary ( "[" arguments? "]" )*
arguments := expression ( "," expression )*
primary := NUMBER | STRING | IDENTIFIER | "true" | "false" | "nil" | "this" | "(" expression ")" | "super" "." IDENTIFIER | "[" (expression)* "]"
```

Statement grammar rule:
```
program := declaration* EOF
declaration := mut_declaration | function_declaration | class_declaration | statement
statement := expression_statement | print_statement | if_statement | for_statement | while_statement | break_statement | return_statement | import_statement | block
expr_statement := expression ";"
print_statement := "print" expression ";"
mut_declaration := "mut" IDENTIFIER ( "=" expression )? ";"
function_declaration := "fun" function
class_declaration := "class" IDENTIFIER ( ":" IDENTIFIER )? "{" function* "}"
function := IDENTIFIER "(" parameters? ")" block
parameters := IDENTIFIER ( "," IDENTIFIER )*
block := "{" declaration* "}"
if_statement := "if" "(" expression ")" statement ( "else" statement )?
for_statement := "for" "(" ( mut_declaration | expression_statement | ";" ) expression? ";" expression? ")" statement
while_statement := "while" "(" expression ")" statement
break_statement := "break" ";"
return_statement := "return" expression? ";"
import_statement := "import" STRING ";"
```
