# README for 06_UsingMallocInWASI

This demo demonstrates how to use `malloc` to allocate memory in WebAssembly code.
Note that `malloc` is part of the standard C library, which we have specifically excluded.
To get `malloc` for WebAssembly, we must use a special library, wasi-libc, which was developed to provide most of the same functionality in libc.
This will require some extra setup, shown below, but first let's talk about memory in WASM.

## WASM Memory
In WASM, the memory is divided into 4 main components: Code, Data, Stack and Heap.

Code is where your program instructions exist.
This typically starts at the very bottom of memory and is loaded into memory from the binary file.
You should not touch this memory, because modifying it will result in bad instructions or other corruption.
In WebAssembly, the Code section starts at address 0x0000 and builds up.
Note that address 0x0000 is normally not considered valid, so usually Code actually starts at address 0x0001.

Data is memory that holds things like global data and is placed right after the Code.
It includes data that is loaded from the program image, such as static strings.
For example, when you make your Hello World program, the string `"Hello, world!"` is saved in the program file then copied into the Data section for use by the program when it is loaded.

The Stack is the storage location for local memory.
This is where function variables and parameters passed to functions are stored.
The stack in WASM starts somewhere in the middle and grows downward.
So the deeper you get into the call stack, the lower the addresses of your local data will be.

The Heap is where dynamic data is stored.
This memory has to be allocated with something like `malloc` to be able to use it.
In WASM, the Heap starts in the middle, usually somewhere near the start of the Stack, but it grows upward.
Newly allocated memory will typically have a higher address than older allocations.
However, as memory is freed and reallocated, objects can be placed all over the Heap, so objects are not necessarily in any order.

Here is a simple diagram of WASM memory
```
Top of Memory (Highest Addresses)
------
|
|
| /\
|  |
|  | Grows upward
| Heap
------
| Stack
|  |
|  | Grows downward
| \/
|
|
------
|
|
| /\
|  |
|  | Grows upward
| Data
|
|
| Code
------
Bottom of Memory (Lowest Addresses)
```

To WASM code, this memory map is its whole world.
Everything here is accessed like in a regular C/C++ program with pointers in a linear address space.
If you want to share data with JavaScript, we must make this memory available to JavaScript.
Since WASM only passes and returns `int`s to JavaScript, to pass more complex data, we must do this through pointers into shared memory.

In JavaScript, we can set up a WebAssembly module so that it uses a block of memory, which it has already allocated, for the WASM code to use as its "world".
We do this by creating a new `WebAssembly.Memory` object in JavaScript, then passing that memory object to `instantiate` through `importObject`.
This makes the memory object available for the WASM code to use when linked, but it does not use it by default.
Normally, `instantiate` will create its own address space and use that for the WASM code.

To override this, we must pass the `--import-memory` flag to the linker. (See Makefile for details.)
Note that if you do not use this flag, `instantiate` will still create its own new addres space and not use the one you passed through `importObject`.
This can be seen in this demo by the fact that if you remove the flag and look at `instance.exports.memory`, it will be a 128KB buffer.
However, the memory we created in JavaScript was 256KB, so without the flag, it is using the wrong memory.

Once this is done, the address space used by the WASM code will be the same as the memory object created in JavaScript.
The WASM code accesses the memory with regular pointers.
The JavaScript code accesses the memory through the `memory.buffer` object.
You can inspect this from the console and see all the data there, but the Code might show all 0's, because it is protected.

In the next demo, we will move on to actually using the shared memory to pass a value from JavaScript to WASM.
Just introducing the memory model for WASM and adding wasi-libc is enough for one demo.

## Setup
To install wasi-libc, you must use CMake.
CMake is a tool that is used to make a build environment for a program.
It makes it possible to easily port programs to different platforms, including Windows and Linux.
For this demo, we will just use CMake to create the build environment that uses `make` on Linux to build wasi-libc.
```sh
	git clone https://github.com/WebAssembly/wasi-libc
	cd wasi-libc
	cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr/local/wasi -DCMAKE_C_COMPILER=clang -DTARGET_TRIPLE=wasm32-wasi
	cd build
	make
	sudo make install
```
The first two steps just get the source of the wasi-libc repository on Github and changes into its directory.
You can put this anywhere, but we just put it in `~/src`.

The next command runs `cmake`. Here is an explanation of the parameters passed to `cmake`
- `-S .` This flag specifies where to get the source that will be built into the target. Note that CMake is not actually compiling the code, it is just putting the files somewhere that they are ready to be built using whatever system is intended. The `.` basically just says use the source in the current directory.
- `-B build` This flag tells where to put the build target. This directory will contain all the files needed to compile and build the wasi-libc library using `make`.
- `-DCMAKE_INSTALL_PREFIX=/usr/local/wasi` This tells where to place the wasi-libc files after building them. We put it in `/usr/local` because that is a standard location to put code installed for just this machine. You can put it anywhere you want, as long as you use the same location when you link to the library and refer to its include files.
- `-DCMAKE_C_COMPILER=clang` As we have said before, `gcc` can not target WASM, so this flag ensures the build uses `clang`.
- `-DTARGET_TRIPLE=wasm32-wasi` This tells the build process what targets to make. Without this flag, it creates `wasm32-wasip1` in the `lib` and `include` directories but not `wasm32-wasi`.

After running `cmake`, you just change into the directory you specified for the target and build the project like any other project you would build on Linux with make.
If you run it with the same options as above, you should now have the directory `/usr/local/wasi`.
It will have two directories, `include` and `lib`, each with the subdirectories `wasm32-wasi` and `wasm32-wasip1`.
We will need to tell the compiler to find the include (`.h`) files in the `/usr/local/wasi/include/wasm32-wasi` directory, and
tell the linker to link to the files in `/usr/local/wasi/lib/wasm32-wasi`.
Please see the Makefile to see exactly how this is done.

Last, we run `make` with the `install` target.
This is the step that actually copies the libraries and other files to `/usr/local/wasi`.
We must use `sudo` for this, because `/usr/local` usually does not have write permission for anyone except root.

## Building
Be sure you have followed the additional setup instructions above, then do the usual:
```sh
cd ~/src/wasmdemo/06_UsingMallocInWASI
make
```

## Running
Open `index.html` in a web browser. Typically this will be done with a URL like `localhost/wasmdemo06/index.html`.

The `index.html` file will load the WASM and call the main function.
Output will be in the JavaScript Console, so you need to open the debugger to see the output.
If you wish to inspect memory, you must put a breakpoint in `startup.js` somewhere inside the onload function.
Once this function ends, the `memory` object will be out of scope, so you will not be able to see it.
You can see the memory by looking at `memory.buffer` in the console.
Expand the `Uint8Array` object and drill down to the location you want with the ranges provided.
Keep in mind that this demo prints addresses in Hex, so you will need to convert those to decimal for the appropriate array index.
Or you can just change the logging function to print decimal.

Try inspecting the memory before the module is loaded, after it is loaded and after main has completed.
You should see the Data section get populated after load and values appearing in the Stack after main finishes.
Note that after main finishes, not all the Stack data you might expect to see will be there.
This is because some Stack data is overwritten between the time the functions are called and main returns.
If you dig in deeper and step into the actual WebAssembly code, you should see this data appearing on the Stack.
