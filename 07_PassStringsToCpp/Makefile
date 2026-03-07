# Makefile for 07_PassStringsToCpp
#
# No changes from last demo.
# Last time, we got the demo to build with wasi-libc.
# This time, we will get it to actually use wasi-libc to pass parameters with malloc.

CC = clang++
WASIROOT = /usr/local/wasi
CFLAGS = --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -I${WASIROOT}/include/wasm32-wasi -c
LFLAGS = --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -Wl,--no-entry -Wl,--import-memory -Wl,-lc
WASMTARGET = Library.wasm
WASMOBJECTS = Greeting.o Utils.o Main.o
TARGETOBJECTS = ${WASMTARGET} index.html startup.js
WEBROOT = /var/www/html/wasmdemo07

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
