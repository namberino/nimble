# Error handling

This is an important feature of the programming language. Being able to catch and throw errors correctly and indicate where the error is at is something every programming language's interpreters need.

## The error class

The error class will report the error, where the error is occuring, which line the error is at, and the error message. It's pretty bare bone though, there's more we could do to the error handling like showing the user the actual line and where the error is at with an arrow and stuff.

The error class has the `has_error` variable, which indicates if there's an error or not. If it is true, the program exits with a non-zero value to indicate error.

The flag will be reset in an interactive loop, so that it doesn't kill the user's session if they make a mistake.

The error class has a `report()` and `error()` functions. These 2 functions, as their name suggests, report and generate errors.

## Syntax error

If there's an invalid list of tokens, the parser will throw a syntax error. Good syntax error handling is critical to a programming language. The parser needs to be able to detect and report those errors, and it also needs to be able to avoid crashing. It needs to be able to detect as many errors as possible, but not detect cascaded errors (errors that are side effects of previous ones).

As soon as the parser detects an error, it enters panic mode (it knows at least 1 token is invalid). Before it gets back to parsing, it needs to get its state and sequence of tokens aligned such that the next token match the rule being parsed. This is called *synchronization*.

We do this by selecting some rule in the grammar to be marked as synchronization points, which are **between statements**. The parser fixes its parsing state by jumping out of any nested productions until it gets back to that rule. Then it synchronizes the token stream by discarding tokens until it reaches one that can appear at that point in the rule. Any additional syntax errors hiding in those discarded tokens aren't reported, so no more cascaded errors.
