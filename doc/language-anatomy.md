# The anatomy of the programming language

There's a couple of components of this programming language that plays a vital role. I'll explain each one of those components here

## Lexical analysis

A lexer takes in a stream of characters (from a file or from the command-line) and turn them into tokens. It should ignore stuff like whitespaces.

```
[mut] [avg] [=] [(] [n1] [+] [n2] [+] [n3] [)] [/] [3] [;]
```

> Each word in the brackets represents a token.

## Parsing

This is where the grammar analysis happen. This allows us to build large expressions and statements from small parts.

The parser takes in a list of tokens and constructs an abstract syntax tree (AST) which describe which expression and statement needs to be processed first. It mirrors the nested nature of grammar. It also has the job of reporting errors for when the program doesn't follow the grammar.

```
         mut statement  [avg]
                          |
      binary expression  [/]
                          |
                       -------
                       |     |
    binary expression [+]   [3] literal expression
                       |
                 -------------
                 |     |     |
                [n1]  [n2]  [n3] mut expression
```

## Static analysis

NIMBLE is dynamically typed so the type checking will be done at runtime. If those type don't support being operated with each other, report a type error

Firstly, the language will do something called **binding**/**resolution**. For each identifier, we find where it is defined and wire the 2 together. Scope also comes into play here. It is the region of code where a certain name can be used to refer to a certain declaration

All of these information will get stored into the tree. We do this by transforming the tree into a new data structure that expresses the semantic of the code

## Interpreter

For our language, we begin executing code right after parsing it into an AST and applying static analysis. The interpreter will traverse the tree and evaluate each nodes as it goes.
