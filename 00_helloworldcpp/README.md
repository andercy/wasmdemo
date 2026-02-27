# README for 00_helloworldcpp

This example is the most basic of demos.
Its purpose is primarily just to test to make sure your build environment is setup correctly and will work.
This program and most of these demos use C++.
This demo ensures a C++ program can be compiled and run.
Since we will be using clang to build WASM later, the Makefile uses `clang`, instead of `g++`, but the result is the same.
Note that to use C++ libraries, like iostream, you must link with `clang++`, not `clang`, just like with `gcc` and `g++`.
If you encounter any errors, make sure to follow the setup instructions in the README file in the main wasmdemo folder.

The Makefile shows simple variable use for the compiler name and statically defined dependencies.
It also introduces a basic `clean` target to remove object and executable files.
The `clean` target will remove final target files and intermediate files, such as `.o` files.
To clean the source directory, just run the command
```sh
make clean
```

This project also demonstrates separate compile and link phases with `clang`, which also work the same as with `gcc`.
Most tutorials will show build as a single step, but this separates them for learning purposes.

## Building
Note that these instructions assume you put the demos in the `src/wasmdemo` directory under your home folder.
All future demos do the same.
Please modify this to whatever directory you are using if different.
```sh
cd ~/src/wasmdemo/00_helloworldcpp
make
```

## Running
```sh
./helloworldcpp
```
