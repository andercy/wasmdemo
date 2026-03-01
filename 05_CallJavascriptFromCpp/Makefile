# Makefile for 05_CallJavascriptFromCpp
#
# This Makefile does little more from previous demos other than copying the JS file to WEBROOT.

CC = clang++
CFLAGS = --target=wasm32 --no-standard-libraries -c
LFLAGS = --target=wasm32 --no-standard-libraries -Wl,--no-entry
WASMTARGET = Library.wasm
WASMOBJECTS = Greeting.o Utils.o Main.o
TARGETOBJECTS = ${WASMTARGET} index.html startup.js
WEBROOT = /var/www/html/wasmdemo05

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
