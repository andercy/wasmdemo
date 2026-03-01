# README for 03_ClangMultiFileCppToWASM

This demo shows how to make multiple objects into a single WASM library.
Since not much else new is being done, the Makefile introduces using wildcards to setup dependencies for object files.

## Building
```sh
cd ~/src/wasmdemo/03_ClangMultiFileCppToWASM
make
```

## Running:
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/wasmdemo03/index.html`.
The `index.html` file will load the WASM and call a few funtions from `Greeting` and `Utils` in the library, `Library.wasm`.
Output will be in the JavaScript Console, so you need to open the debugger to see the output.
