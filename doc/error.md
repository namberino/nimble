# Error handling

The error class will report the error, where the error is occuring, which line the error is at, and the error message. It's pretty bare bone though, there's more we could do to the error handling like showing the user the actual line and where the error is at with an arrow and stuff.

The error class has the `has_error` variable, which indicates if there's an error or not. If it is true, the program exits with a non-zero value to indicate error.

The flag will be reset in an interactive loop, so that it doesn't kill the user's session if they make a mistake.

The error class has a `report()` and `error()` functions. These 2 functions, as their name suggests, report and generate errors.
