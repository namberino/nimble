# Benchmarking NIMBLE

Benchmarking allows us to measure the performance of the language and figure out how optimized is our programming language.

Currently, NIMBLE's benchmark is not very good. This project wasn't built with optimization in mind. Hopefully in the future, we can integrate [LLVM](https://llvm.org/) into this project and improve the performance.

## Benchmark test files

The programs used for benchmarking are all very computationally intensive. Usually these would take C around 2 seconds max to run and Python around half a minute. So if NIMBLE could actually go under 30, it'd be a huge achievement.

YOu can find all the benchmark programs in the [benchmark](../benchmark/) directory. I encourage you to create your own benchmark programs to test the limits of NIMBLE.

## Running the benchmark 

The benchmarks can be ran by running the interpreter on the programs, or if you want to test all of the benchmarks and get results from all of them, you can run `./tools/bench.sh` or `make bench`.
