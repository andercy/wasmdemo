# README for 05_CallJavascriptFromCpp

This demo is for multiple experiments, but its first intent was to place the JS code into a separate file.
JS code in the browser just seems to debug better when running in a separate `.js` file instead of from HTML.
We discovered that Linux Chrome does not like to stop at breakpoints in async functions.
They do seem to work in Windows Chrome or if the function is not async in Linux.

One important change here is that `.cpp` files now use the `clang::export_name` decorator.
This decorator allows us to specify what name we wish to use for the C/C++ function being exported.
Since we manually specify which functions to export, the flag `--export-all` no longer needs to be passed to the linker, and C++ names are no longer mangled.
This also allows us to choose which functions NOT to export, making them effectively local to the C++ code.

The most crucial change for this demo is that it adds the ability to call JavaScript functions from C++ code.
It does this by passing an object to `instantiateStreaming` that holds the function name and an anonymous function to call in JavaScript.
In the C/C++ code, the decorator `clang::import_name` is used to make the function available for use from C/C++.
Naturally, if this decorator is used, and the object passed to `instantiate` does not have that function, `instantiate` will fail.
Note that we have already been calling C/C++ from JavaScript, but this demo adds calling a `main` function.
The `main` function has been placed in its own file, `Main.cpp`, since it does not belong in Greeting or Utils.
The main function passes a C/C++ string to JavaScript.
The next demo will work on passing string data from JavaScript to C/C++.
WebAssembly actually only passes ints, so passing more complex data must be done with pointers.

The JavaScript function being called from C++ is `consolelog`.
This function is vital, because it allows us to print messages from C/C++ to the debugging console in the browser.

The last change for this demo is that it uses `instantiateStreaming`, instead of `instantiate`.
Both functions should work basically the same and have the same inputs, but `instantiateStreaming` is the standard used these days and is more efficient,
since it can begin processing the WASM code before it is all received.

Note that the layout used in this demo is slightly different from most examples on the Web.
Instead of putting all the logic for the JavaScript function being called in an anonymous function, this example calls a named function.
This was done just to make the code more organized and easier to understand.

## Building
```sh
cd ~/src/wasmdemo/05_CallJavascriptFromCpp
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/wasmdemo05/index.html`.
The `index.html` file will load the JavaScript file `startup.js`, which will load the WASM.
It will then call a function from `Utils` then call the function `main`.
The main function then calls JavaScript and prints the string passed to it to the console.

Note that output from JavaScript functions that call C++ functions is now written to a `<div>`, so it is immediately visible.
Output to the console is now actually coming from C++ code, so you need to open the debugger to see that.
This is very important for debugging C++ code going forward.
