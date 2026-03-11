# Makefile for 08_ObjectsInWASM
#
# For this Makefile, the files being used have been changed slightly.
# Also, the flags "-g -fno-limit-debug-info" have been added.
# This tells the compiler to include debugging information (DWARF) in the WASM file.
# This makes it possible to step through the C/C++ source code, instead of the raw WebAssembly.
# You must have the Chrome DevTools WebAssembly Extension to do this.
# See the README for more information.

CC = clang++
WASIROOT = /usr/local/wasi
CFLAGS = -g -fno-limit-debug-info --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -I${WASIROOT}/include/wasm32-wasi -c
LFLAGS = -g -fno-limit-debug-info --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -Wl,--no-entry -Wl,--import-memory -Wl,-lc
WASMTARGET = Library.wasm
WASMOBJECTS = Person.o Utils.o Main.o
TARGETOBJECTS = ${WASMTARGET} index.html startup.js WASMUtils.js
WEBROOT = /var/www/html/wasmdemo08

.PHONY: demo
demo: ${TARGETOBJECTS}
	rm -rf ${WEBROOT}
	mkdir ${WEBROOT}
	cp -f index.html ${WEBROOT}
	cp -f startup.js ${WEBROOT}
	cp -f WASMUtils.js ${WEBROOT}
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
