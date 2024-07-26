# Error handling

This is an important feature of the programming language. Being able to catch and throw errors correctly and indicate where the error is at is something every programming language's interpreters need.

## The error class

```cpp
class Error
{
    public:
        static bool has_error; // general error
        static bool has_runtime_error; // runtime error

        static void report(int line, const std::string& where, const std::string& msg);
        static void error(int line, const std::string& msg);
        static void error(const Token& token, std::string msg);
        static void runtime_error(const RuntimeError& error);
};
```

The error class will report the error, where the error is occuring, which line the error is at, and the error message. It's pretty bare bone though, there's more we could do to the error handling like showing the user the actual line and where the error is at with an arrow and stuff.

The error class has the `has_error` variable, which indicates if there's an error or not. If it is true, the program exits with a non-zero value to indicate error.

The flag will be reset in an interactive loop, so that it doesn't kill the user's session if they make a mistake.

The error class has a `report()` and `error()` functions. These 2 functions, as their name suggests, report and generate errors.

## Syntax error

If there's an invalid list of tokens, the parser will throw a syntax error. Good syntax error handling is critical to a programming language. The parser needs to be able to detect and report those errors, and it also needs to be able to avoid crashing. It needs to be able to detect as many errors as possible, but not detect cascaded errors (errors that are side effects of previous ones).

As soon as the parser detects an error, it enters panic mode (it knows at least 1 token is invalid). Before it gets back to parsing, it needs to get its state and sequence of tokens aligned such that the next token match the rule being parsed. This is called *synchronization*.

We do this by selecting some rule in the grammar to be marked as synchronization points, which are **between statements**. The parser fixes its parsing state by jumping out of any nested productions until it gets back to that rule. Then it synchronizes the token stream by discarding tokens until it reaches one that can appear at that point in the rule. Any additional syntax errors hiding in those discarded tokens aren't reported, so no more cascaded errors.

## Runtime error

The interpreter has to do a lot, casting, executing, etc. And there's no guarantee of that all of those will work 100% of the time. If the user write bad code, then the interpreter will fail. It's our job to handle those error gracefully.

Runtime errors are errors that happen while the program is running. We need to make it so that the interpreter could handle and report those runtime errors and also alert the user about those errors. When a runtime error occurs deep inside an expression, we need to break out of that.

These runtime errors need to stop executing the program and stop evaluating the expression, and not kill the interpreter program.

```cpp
class RuntimeError : public std::runtime_error
{
    public:
        const Token& token;

        RuntimeError(const Token& token, std::string msg);
};
```

The `RuntimeError` class will track the token where the runtime error occurs by holding a reference to that token. This helps the user know where the error occured.

We can throw this error deep inside the evaluator to break out. Then we can catch those error and properly handle them.
