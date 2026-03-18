# Makefile for 09_ModifyingDOMInWASM
#
# For this Makefile, the files being used have been changed slightly.

CC = clang++
WASIROOT = /usr/local/wasi
CFLAGS = -g -fno-limit-debug-info --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -I${WASIROOT}/include/wasm32-wasi -c
LFLAGS = -g -fno-limit-debug-info --target=wasm32-wasi --sysroot ${WASIROOT} --no-standard-libraries -Wl,--no-entry -Wl,--import-memory -Wl,-lc
WASMTARGET = Library.wasm
WASMOBJECTS = Person.o WASMUtils.o Main.o
TARGETOBJECTS = ${WASMTARGET} index.html startup.js WASMInstance.js
WEBROOT = /var/www/html/wasmdemo09

.PHONY: demo
demo: ${TARGETOBJECTS}
	rm -rf ${WEBROOT}
	mkdir ${WEBROOT}
	cp -f index.html ${WEBROOT}
	cp -f startup.js ${WEBROOT}
	cp -f WASMInstance.js ${WEBROOT}
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
