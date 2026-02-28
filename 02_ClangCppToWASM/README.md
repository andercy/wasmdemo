# README.txt for 02_ClangCppToWASM

All this demo does is show how to build a WASM file from C++ code, instead of C.
Not much has changed from the previous demo, except it is using `clang++`.

Note that in `index.html`, the function called from JavaScript is `"_ZN8Greeting16getDefaultLengthEv"`.
This is due to name mangling in C++, which must be done to keep function names unique.
For more information, see the Running instructions below.

Since not much else new is being done, the Makefile introduces using variables for compiler flags and object files.

## Building
```sh
cd ~/src/wasmdemo/02_ClangCppToWASM
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/demo02/index.html`.
The `index.html` file will load the WASM and call a few funtions from the `Greeting.wasm` library.
Output will be in the JavaScript Console, so you need to open the debugger to see the output.

Once you have opened the page and browser debugger, inspect the source code for `index.html`.
Put a breakpoint on the line with `console.log`, reload the page and inspect the `instance.exports` object.
This object should contain all the functions exported from Greeting.cpp, but the names are mangled, because they came from C++.
In the future, we will see how to work around this, but for now you may need to change the name of the function used in `index.html`, if your compiler uses different name mangling rules.
