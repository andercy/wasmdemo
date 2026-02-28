# README for 01_ClangCToWASM

This demo shows how to build a WASM file from C code.
Normally, emscripten is used to build WASM, but it has a lot of overhead.
My goal was to make a custom system running in its own environment, so the extra libaries provided by emscripten should not be needed.
To use the WASM, a custom `index.html` file is used, which loads the WASM code on its own.
The WASM is built as a library from the `greeting.c` module.

The Makefile introduces a step that copies needed files, including the WASM and `index.html` into the `WEBROOT` directory.
Set the `WEBROOT` variable to whatever directory you setup the webserver to serve files from for this demo.
By default this setup will place the WASM and HTML into a subdirectory of `WEBROOT` named `demo01`.
Please see the main README file for more information about the `WEBROOT`.

The Makefile also demonstrates how you can build a module with no `main` function with the `--no-entry` linker flag.
Without this flag, the module would need to include a dummy `main` function to prevent errors.
Note that to compile to WASM, the flags `--target=wasm32` and `--no-standard-libraries` must be used in both compiling and linking phases.

Note also that to get `clang` to use `wasm32`, you may have to install `lld-18`:
```sh
sudo apt install lld-18
```
To check if lld-18 is installed, use `apt list lld-18`.
If it is installed, there should be a line of text that ends with `[installed]`.
If not, when you try to build, you will get a message saying `unable to execute command: Executable "wasm-ld-18" doesn't exist!`.

## Building
```sh
cd ~/src/wasmdemo/01_ClangCToWASM
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `http://localhost/demo01/index.html`.
If you are using a server on the public internet, replace `localhost` with the IP address or hostname of your server, and include `http://` at the beginning of the URL.
Most modern browsers will try to access websites on the internet with HTTPS by default.
Since we have not set up HTTPS, you will get a page not found error otherwise.
This applies to all future demos as well.

The `index.html` file will load the WASM and call a few funtions from `Greeting` and `Utils` in the library, `Library.wasm`.
Output will be in the JavaScript Console, so you need to open the debugger to see the output.
