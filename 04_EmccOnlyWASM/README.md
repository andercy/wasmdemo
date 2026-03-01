# README for 04_EmccOnlyWASM

## IMPORTANT!
Note that this demo is very different from the others, because it uses emscripten.
My main purpose for this was testing to see if emscripten performs C++ name mangling, (It does.) and to just see if I could get emcc to generate WASM only without the extra HTML and JavaScript.
The test was successful, but I have not uploaded the source yet.
Using emscripten requires additional setup, beyond what is needed for clang, so I need to verify I have everything needed and nothing more for building the demo.
Once this is done, I will remove this message. Until then, it is safe to skip ahead to wasmdemo05.

---

This demo shows how to build a WASM library using `emcc`.
This builds only the WASM file, without making all the extra HTML and JavaScript `emcc` usually makes.
This might be necessary in the future if we need to use `libc`, and WASI does not work.
To do this, `--no-entry` is added to the linker flags.
Alternatively, you can add a dummy `main` function to the source file, but this be not recommended.

One vital change to this demo is the addition of a `.vscode` directory with the file `c_cpp_properties.json`.
This file adds the include path for `emcc` to the include paths used by VSCode.
This path needs to be changed, based on where emsdk is located on your system.
Alternatively, you can add this config file by going to "C:/C++: Edit Configurations" in the Command Pallette. (Ctrl+Shift+P)
The process will be different for environments other than VSCode.
Note that without this config, it will still build with `emcc`, but VSCode will complain.
`emcc` knows where its libraries and includes are, probably based on its current path when running.
Also note that although includes are in `~/src/emsdk/upstream/emscripten/system/include/`,
the actual path used by `emcc` is `~/src/emsdk/upstream/emscripten/cache/sysroot/include/`.
This can be seen by running `emcc` with the `-v` flag.

Looking in the browser debugger, you will notice that C++ functions compiled with `emcc` are still mangled.
Mangling happens even if you do not use any classes or actual C++ features, as long as you use `em++`.

## Building
If using VSCode, change the path in `c_cpp_properties.json` to be the path to the emscripten includes.
Go to shell and run:
```sh
cd ~/src/wasmdemo/04_EmccOnlyWASM
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/wasmdemo04/index.html`.
The `index.html` file will load the WASM and call a few funtions from `Greeting` and `Utils` in the library, `Library.wasm`.
Output will be in the JavaScript Console, so you need to open the debugger to see the output.
