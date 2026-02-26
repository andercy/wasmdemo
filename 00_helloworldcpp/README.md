# README for 00_helloworldcpp

This example is the most basic of demos.
It just shows how a C++ program can be compiled and run.
The Makefile uses `clang`, but the result is the same as with `g++`.
Note that to use C++ libraries, like iostream, you must link with `clang++`, not `clang`, just like with `gcc` and `g++`.

The Makefile shows simple variable use for the compiler name and statically defined dependencies.
It also introduces a basic `clean` target to remove object and executable files.
It also demonstrates separate compile and link phases with `clang`, which also work the same as with `gcc`.

## Building
```sh
cd 00_helloworldcpp
make
```

## Running
```sh
./helloworldcpp
```
