# Parser

The parser will take the list of tokens generated from the lexer and create an abstract syntax tree (AST) out of those tokens.

## Defining the AST

A way to implement the precedence of operations is by using a tree. We can use post-order traversal to work our way from the bottom of the tree up, evaluating every operations in the correct order.

![ast](img/ast.jpg)

The tree representation needs to match the grammatical structure of the language. So we need to get precise with the language's syntactic grammatical structure.

NIMBLE's syntactic grammar is going to be utilizing the [Chomsky hierarchy](https://en.wikipedia.org/wiki/Chomsky_hierarchy). You can check out the grammar rule of the language [here](grammar.md).

## Parser

Parsing technique: [Recursive descent](https://en.wikipedia.org/wiki/Recursive_descent_parser).
