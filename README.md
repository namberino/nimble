# The NIMBLE Programming Language

![logo](doc/img/logo.png)

NIMBLE (Nam's Interpreted Modular Basic Language Engine) is a dynamically typed, interpreted programming language with C-like syntax. This repository contains the source code for the NIMBLE programming language's interpreter.

I started this project at the beginning of June 2024 with the goal of learning more about programming language design and practicing my C++ skills. It then gradually became one of my biggest projects yet. 

This is a gigantic learning project for me. This project was inspired by the [Lox](https://github.com/munificent/craftinginterpreters) programming language. The interpreter is not very optimized and there may be some bugs that I overlooked, but I'm quite happy with how it turned out. I'll definitely continute working on when I have the time.

## Syntax

```nimble
import "core:math"

fun fib(n)
{
    if (n <= 1)
    {
        return n;
    }

    return fib(n - 2) + fib(n - 1);
}

var li = [];

for (var i = 0; i < 20; i = i + 1)
{
    li[i] = fib(i);
}

print(li);
```

NIMBLE is a dynamically typed programming language, featuring C-like syntax with class support and C++ inheritance syntax. It has a very "*fun*" syntax (because the function keyword is *fun*)

## Requirements

Before compilation, you'll need to have these tools installed first:
- [Clang](https://clang.llvm.org/) or [GCC](https://gcc.gnu.org/) (I built this with Clang but GCC works just fine)
- [GNU Make](https://www.gnu.org/software/make/)

## Project structure

| Directory | Description |
| --- | --- |
| [benchmark](benchmark/) | Benchmark programs |
| [doc](doc/) | Documentation |
| [examples](examples/) | Example programs |
| [include](include/) | Header files |
| [src](src/) | Source code |
| [tests](tests/) | Test cases |
| [tools](tools/) | Useful tools and scripts |

## Compiling

If you're using `make`:
- `make compile` to compile the program into the `bin/` directory
- `make run` to run the program
- `make clean` to clean up the object and binary files
- `make test` to run test cases
- `make bench` to run benchmarks

## Benchmark

Average elapsed time of computationally intensive programs:

| Program | AET |
| --- | :---: |
| bintree.nbl | 82s |
| count.nbl | 57s |
| fibonacci.nbl | 77s |
| prime.nbl | 35s |

## Documentation

You can find the documentation in the [doc](doc/) directory. You can also learn about the syntax of the language through example programs found in the [examples](examples/) directory.

## Plans for this project

For now, most of the features of the language and the interpreter is complete. Currently, I'm working on the documentation for this interpreter.

The long term plan is to integrate with [LLVM](https://llvm.org/) and create a compiler for this language for improved performance and speed.

## VSCode syntax support

I've created a VSCode extension for the language. You can checkout the extension [here](https://github.com/namberino/nimble-vscode)

## References

- [Making a Programming Language & Interpreter in under 10 minutes](https://www.youtube.com/watch?v=A3gTw1ZkeK0)
- [Writing An Interpreter In Go](https://www.amazon.com/Writing-Interpreter-Go-Thorsten-Ball/dp/3982016118)
- [Crafting Interpreters](https://craftinginterpreters.com/)
- [Make YOUR OWN Programming Language in Python](https://youtube.com/playlist?list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD&si=cr5N8m9Ybknj0Pvr)
- [Let's Write an Interpreter (in 168 Lines of Python)](https://www.youtube.com/watch?v=LgsW0eGk-6U)
- [My First Language Frontend with LLVM Tutorial](https://www.llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)
