# README for 08_ObjectsInWASM

In the last demo, we showed how to pass strings to C/C++ from JavaScript.
Now that we can pass strings to and from JavaScript and C/C++, we can theoretically pass anything that can be serialized, but that requires parsing the object every time it is passed.
Rebuilding an object from a string every time you need to call a function for it is inefficient, however.
The best solution, in my opinion, is to not pass objects at all.
Instead, what we can do is keep all objects inside the WASM memory space.
Then when JavaScript needs to use the object, it calls a function, passing a reference to the object where needed.

The other problem we needed to deal with was memory leaks.
We still need to pass strings, even if we avoid passing objects.
So we need to devise a method to make sure strings are freed when not used, not freed more than once and never freed if they are in the Data section.
Other languages, like Rust, use the idea of ownership of data to prevent memory leakage.
We will use a similar system, but much simpler.
These will be our Rules:
1. The function that allocates memory is responsible for freeing it.
2. Strings passed to an object must make a copy of that string if they intend to keep it.
3. Objects must free any strings they own in their takedown code.
4. The function that dynamically allocated an object must call the takedown code on the object when done with it.
5. Objects should be passed by reference, especially for calls from JavaScript. Passing a reference to an object does not transfer ownership.
6. Any time a function returns a pointer, the function receiving the pointer is responsible for freeing it.
7. (Exception to 6) If a function wants to return a pointer but not transfer ownership to the caller, the function name should start with `getRef`.
8. As much as possible, treat strings as immutable. (Unless building a large string, of course, then treat as immutable once done building it, if not too large.)

Note that we can not use `new` and `delete` operators in this demo, so they are not mentioned in the rules above.
WASM with plain clang does not fully support all features of C++.
This includes the `new` and `delete` operators and Exception handling.
See the file `Person.cpp` to see how we work around this.

Similarly, the words "constructor" and "destructor" are also not used.
Since we cannot use  `delete`, we must put the the deallocation code somewhere it can be accessed outside the destructor.
For instances where the destructor is called, we just have it call this same deallocation code.

You do not have to follow these rules for your own code, but you do have to make sure you do not have any memory leaks.
This demo demonstrates how to apply these rules and does some cleanup over the last demo.
Not much new is done as far as WASM technology, but this demo has several key points.
Here are some of the things this demo has done:
* Replaced the Greeting class with the Person class. This just tends towards more interesting data.
* Created a Person instance in C++ and returns it to JavaScript.
* Moved WASM code out of `startup.js` and into a new file, `WASMUtils.js`.
* Moved support functions out of `Main.cpp` into `Utils.cpp`.
* Added support functions to `WASMUtils.js` that make it easier to call WASM functions.
* Enabled debugging of C/C++ source code in Chrome DevTools (See note below.)
* Added function documentation for most public methods, so you should see help pop up when using a function.
* Added some basic null checking.
* Moved main output to HTML elements. Now only debugging output should be in the console.
* Changed parameter types and return types of exported C/C++ functions to pointer types instead of ints, where appropriate. (To JavaScript, they look the same.)

Most of these items are self explanatory, but here is bit more information about calling WASM functions.
You probably noticed how ugly it is to call a WASM function.
In the last demo, it would take several lines to setup a call to a WASM function and then release resources when done.
This makes code difficult to follow, so we made a function in `WASMUtils.js` named `callWASM`.
This function allocates memory for string parameters using `malloc`.
It then takes the parameters out of the array using the span operator (`...`), calls the function, using `call` (with `null` passed for the `this` parameter) and finally deallocates the strings when done with `free`.
It does not currently do anything with the value returned other than return it to the caller.
So if a function returns a string pointer, the caller must turn it into a JavaScript string to use it and `free` it when done.
Number parameters are passed as is to the function and need no allocation or freeing of memory.
This function does not support object parameters and probably will not need to.
Since Rule 5 says to pass objects as references, they will really just be passed as ints anyway.

Some of what me must do for WebAssembly might feel a bit "old school".
It is a bit primitive not being able to use `new` or `delete` and having to watch for memory leaks and manually allocate and free memory.
Just remember that WebAssembly is still in its early stages.
This is the Wild West.
Earlier languages, like C, did not have automatic garbage collection or quick memory handling with `new` and `delete`.
In the future, such things should become more common.

## Debugging C/C++ Source Code in Browser DevTools
Our C++ source files are still fairly simple, but if you look at the raw WebAssembly in the browser DevTools, it has already started to get long and difficult to find your way around in.
It is possible to debug within the browser DevTools, using the original C/C++ source code, but it takes a bit of setup.

First, you must go [here](https://chromewebstore.google.com/detail/pdcpmagijalfljmkmjngeonclgbbannb?utm_source=item-share-cb) and install the C/C++ DevTools Support extension for Chrome.
If you are using a different browser, it might have a similar extension or add on.
Next, add the `-g` flag to the compile options. This enables inclusion of debugging information in the binary file using a system called [DWARF](https://dwarfstd.org/).
Then, add the `-fno-limit-debug-info` flag to compile options. This just makes it more likely that the compiler will not drop debugging information.

Once you build your program with the options above and refresh the page, you should see a new folder labeled `file://` in the Source tab of the Chrome DevTools.
(You might need to click on your WASM file first to get it to populate.)
You can expand this and see all your C++ source files.
You can even set breakpoints and step through the code, just like JavaScript code in the browser.
This should make debugging much easier from now on.

IMPORTANT: DWARF does not actually include the source code in the binary files.
It just stores references to line numbers in source files and what WASM code they correspond to.
In order to be able to see the source files, DevTools must have filesystem access to the files.
This means to debug C/C++ code in DevTools, you must be running the browser on the same machine running the web server.
If you try debugging from a different client machine, you will get an error like this when you click on one of the files: `Could not load content for file:///home/[username]/src/wasmdemo/08_ObjectsInWASM/Main.cpp (System error: net::ERR_FILE_NOT_FOUND)`.


## Building
```sh
cd ~/src/wasmdemo/08_ObjectsInWASM
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/wasmdemo08/index.html`.

The `index.html` file will load the WASM using WASMUtils and create a new Person object.
It will then generate an HTML table containing the user data and display it in the web page.
From now on, main output will be in the web page, and debugging information will be in the JavaScript Console.
