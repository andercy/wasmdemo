# README for 09_ModifyingDOMInWASM

In this demo we show how to manipulate the Document Object Model (DOM) from WASM to make visual changes to a web page.
We also do more work with objects and make things even more programmer friendly.

Just to summarize, here are the changes this demo has done:
* It now uses JavaScript modules, which is are a more modern and clean way of handling multiple file JavaScript.
* Changed `WASMUtils.js` to `WASMInstance.js` to relect the new class name.
* Put the code that loads and instantiates the WASM file in a `try` block. This made debugging easier when unexpected imports were happening.
* Added a JavaScript function that is imported into C/C++ that allows C/C++ code to modify DOM elements.
* Renamed `Utils.cpp` to `WASMUtils.cpp` to reflect that it contains WASM utility code.

## Modifying DOM from WASM

WASM code is unable to modify DOM elements directly.
To modify the DOM, WASM must call a JavaScript function, which will then do the update.
We created a function called `WASMUtils_updateElement` and added it to the imports in `loadWASM`.
Code in C/C++ can access this function through the static function `WASMUtils::updateDOMElement`.
To use this, the WASM code will pass the element id, attribute name and text to assign to the attribute to the function.
The function converts the C strings, finds the element and sets the given attribute.
The function includes special processing for the attribute name `innerHTML`.
This is not technically an attribute of an element, but when the function sees this, it will set the `innerHTML` property of the element.
Calling JavaScript every time we want to modify the DOM might not be the most efficient way to handle changing elements, but it is how it must be done with WASM for now.
Maybe in the future more direct methods will be made to do this directly from WASM.

## Importing Functions into WASM Code

In this demo, we added a new function, `WASMUtils_updateElement` to the imports.
In the process of making this demo, other functions were also automatically added.
Using the `snprintf` function in `Person.displayPersonTable` causes WASI to add JavaScript functions it needs to import to an object named wasi_snapshot_preview1.
If this is not added to `importObject`, we will get an error when we try to instantiate WASM, saying something like this:
```
WASMInstance.js:51 Error loading WASM file: TypeError: WebAssembly.instantiate(): Import #1 "wasi_snapshot_preview1": module is not an object or function".
```

If you want to inspect a WASM file to see what it requires, stop JavaScript execution after `initiate` is called.
In the demo, we added a `try catch` block to prevent termination if `initiate` fails.
In the Sources tab of DevTools, click on the .wasm file in the left panel then scroll to the top of the WASM code.
You should see lines showing what imports the WASM file expects that look like this:
```
(func $consolelog(char const*) (;3;) (import "env" "WASMUtils_consolelog") (param i32))
```

The first part, `func $consolelog(char const*)` shows the function signature, including the name, that the WASM code will see the function as.
The number (3) is just a sequence number, used to uniquely identify all the imports.
Next `import` identifies that this is a function WASM expects to see provided externally.
The string `"env"` identifies what object in `importObject` the function should be placed in.
(Note how the functions added by WASI are in a different object.)
The next string `"WASMUtils_consolelog"` is the function name it expects to exist in that object.
The last part is the list of parameters.
The `consolelog` function just passes an int which is used as a pointer to a C string to print in the console.
Some functions may also show a return value, indicated by the text `result`.
See the functions added by WASI for an example of this.

## Building
```sh
cd ~/src/wasmdemo/09_ModifyingDOMInWASM
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/wasmdemo09/index.html`.

The `index.html` file will load `startup.js`, which will load the WASM using the class WASMInstance.
It will then create a new Person object, display it, modify it, display it again and create and display a new Person.
