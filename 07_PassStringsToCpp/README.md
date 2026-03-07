# README for 07_PassStringsToCpp

This demo demonstrates passing strings to and from WebAssembly code.
WebAssembly actually only passes integers, so passing anything other than ints must be done with pointers.
In a previous demo, we passed a string to JavaScript and converted it from a buffer of bytes to a string.
In this demo, we will pass a string from JavaScript to WASM.

That sounds like it should be straightforward, but to pass strings (and other objects) from JavaScript, we have to allocate memory in the WASM address space from JavaScript.
Then we store the value being passed in the memory and call the C/C++ function.
To reserve memory, we must use `malloc`, which we made available in the last demo with wasi-libc.
Please see the Setup instructions in the previous demo to see how to install wasi-libc.

Although `malloc` was made available in the last demo, it was not visible to JavaScript.
To make it usable from JavaScript, we add new functions `Utils_wmalloc` and `Utils_wfree`.
The `w` was added to the function names to avoid name conflicts. (See Utils.cpp for more information.)

Once memory is allocated, JavaScript copies its string data into the WASM memory at the location it got from `malloc`.
It then calls the C/C++ function, passing pointers into the WASM memory for the strings as ints.

Two different functions, `printStrings` and `combineStrings` are used.
Each takes 2 strings as parameters and combines them.
`printStrings` then prints the combined strings in memory it allocates and returns.
`printStrings` intentionally contains a memory leak.
This is done to illustrate how we must be careful about allocating memory and freeing it when done.
`combineStrings` returns a pointer to the combined strings, so the calling function can free it, and there is no leak.
We then show how the returned value can be used by converting it to a JavaScript string and printing it.

Please see the actual code, especially in `startup.js` and `Main.cpp` to see how this is all done.

Theoretically we can now pass anything we want.
Since we can pass strings, we can pass an object with JSON, but that would be inefficient.
In the next demo, we will look more at objects, how to handle them and clean some things up.

## Building
```sh
cd ~/src/wasmdemo/07_PassStringsToCpp
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/wasmdemo07/index.html`.

The `index.html` file will load the WASM and call a few funtions from Greeting and Utils in the library, Library.wasm.
Output will be in the JavaScript Console, so you need to open the debugger to see the output.
