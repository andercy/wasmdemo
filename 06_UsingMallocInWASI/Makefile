# Makefile for 06_UsingMallocInWASI
#
# This Makefile changes the build to use memory imported from JavaScript and to link with wasi-libc.

CC = clang++
# Note that the build target has changed to wasm32-wasi.
# This tells the compiler and linker to build binaries for wasm32-wasi, which we just installed, instead of regular plain wasm.
# The --sysroot flag tells where the wasm32-wasi binaries are located. Change the WASIROOT variable to where you installed them if you did not use /usr/local/wasi.
# The -I flag tells the compiler the path to find the headers for the source files.
# These headers have the same names as standard header files, but are slightly different for use with WASM.
WASIROOT = /usr/local/wasi
CFLAGS = --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -I${WASIROOT}/include/wasm32-wasi -c
# Note that --no-standard-libraries is still needed so it does not try to link to the regular C/C++ libraries and uses the WASI ones instead.
# The flag -Wl,-lc tells the linker to link to the libc library, but this is the library in /usr/local/wasi, not the standard libc.
# If you add the -v flag for linking, it will show that it uses the flag -L/usr/local/wasi/lib/wasm32-wasi, which means to look in that directory for libraries.
# Last, we added the flag --import-memory. This flag causes the instance of the WASM module to use the WebAssembly.Memory object imported in the importObject instead of creating its own.
LFLAGS = --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -Wl,--no-entry -Wl,--import-memory -Wl,-lc
WASMTARGET = Library.wasm
WASMOBJECTS = Greeting.o Utils.o Main.o
TARGETOBJECTS = ${WASMTARGET} index.html startup.js
WEBROOT = /var/www/html/wasmdemo06

.PHONY: demo
demo: ${TARGETOBJECTS}
	rm -rf ${WEBROOT}
	mkdir ${WEBROOT}
	cp -f index.html ${WEBROOT}
	cp -f startup.js ${WEBROOT}
	cp -f *.wasm ${WEBROOT}

%.wasm: ${WASMOBJECTS}
	${CC} ${LFLAGS} ${WASMOBJECTS} -o $@

.PRECIOUS: %.o
%.o: %.cpp
	${CC} ${CFLAGS} $< -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.wasm
	rm -rf ${WEBROOT}
