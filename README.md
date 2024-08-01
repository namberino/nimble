# The NIMBLE Programming Language

![logo](doc/img/logo.png)

NIMBLE (Nam's Interpreted Modular Basic Language Engine) is a dynamically typed, interpreted programming language with C-like syntax. This repository contains the source code for the NIMBLE programming language's interpreter.

I started this project at the beginning of June 2024 with the goal of learning more about programming language design and practicing my C++ skills. It then gradually became one of my biggest projects yet. 

The interpreter is not very optimized and there may be some bugs that I might have overlooked, but I'm quite happy with how it turned out.

## Syntax

```nimble
import "core:math";

// collatz conjecture function
fun collatz_conjecture(n)
{
    if (n % 2 == 0)
    {
        n = floordiv(n, 2);
        return n;
    }
    else if (n % 2 == 1)
    {
        n = 3 * n + 1;
        return n;
    }
}

mut n = input("Enter number: ");
mut i = 0;
mut li = [];

// main loop
while (n != 1)
{
    n = collatz_conjecture(n);
    li[i] = n;
    i += 1;
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
| [lib](lib/) | Core library |
| [src](src/) | Source code |
| [tests](tests/) | Test cases |
| [tools](tools/) | Useful tools and scripts |

## Compiling and Running

If you're using `make`:
- `make compile` to compile the program into the `bin/` directory
- `make run` to run the program
- `make clean` to clean up the object and binary files
- `make test` to run test cases
- `make bench` to run benchmarks

You can run the interpreter with `make run` or `./bin/nimble <filename>.nbl`

## Benchmark

Elapsed time of computationally intensive programs:

| Program | Mininum | Average | Maximum |
| --- | :---: | :---: | :---: |
| bintree.nbl | 1.13 | 1.19 | 1.30 |
| count.nbl | 0.57 | 0.61 | 0.64 |
| fibonacci.nbl | 1.01 | 1.04 | 1.10 |
| prime.nbl | 0.34 | 0.34 | 0.35 |

## Documentation

You can find the documentation in the [doc](doc/) directory. You can also learn about the syntax of the language through example programs found in the [examples](examples/) directory.

## Plans for this project

For now, most of the features of the language and the interpreter is complete. Currently, I'm working on the documentation for this interpreter.

Hopefully I could integrate this project with [LLVM](https://llvm.org/) and create a compiler for this language for improved performance and speed in the future.

## VSCode syntax support

I've created a VSCode extension for the language. You can checkout the extension [here](https://github.com/namberino/nimble-vscode)

## References

- [Making a Programming Language & Interpreter in under 10 minutes](https://www.youtube.com/watch?v=A3gTw1ZkeK0)
- [Writing An Interpreter In Go](https://www.amazon.com/Writing-Interpreter-Go-Thorsten-Ball/dp/3982016118)
- [Crafting Interpreters](https://craftinginterpreters.com/)
- [Make YOUR OWN Programming Language in Python](https://youtube.com/playlist?list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD&si=cr5N8m9Ybknj0Pvr)
- [Let's Write an Interpreter (in 168 Lines of Python)](https://www.youtube.com/watch?v=LgsW0eGk-6U)
- [My First Language Frontend with LLVM Tutorial](https://www.llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)
