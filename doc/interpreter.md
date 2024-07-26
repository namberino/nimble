# The interpreter

This is where the magic happens. Where our tokens, expressions, and AST that we created from the lexer and the parser gets interpreted and executed.

## Value representation

Values are created by literals, computed by expression, and stored in variables. A variable can store any type in this language, and can even store values of different types at different points in time.
