# Makefile for 02_ClangCppToWASM
#
# This Makefile just demonstrates the use of more variables for compiler and linker flags and the target objects.
# Also, it uses clang++, since we are using C++ instead of C.

CC = clang++
# Compiler Flags
CFLAGS = --target=wasm32 --no-standard-libraries -c
# Linker Flags
LFLAGS = --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry
# Objects that need to be copied to website
TARGETOBJECTS = Greeting.wasm index.html
WEBROOT = /var/www/html/demo02

demo: ${TARGETOBJECTS}
	rm -rf ${WEBROOT}
	mkdir ${WEBROOT}
	cp -f index.html ${WEBROOT}
	cp -f Greeting.wasm ${WEBROOT}

Greeting.wasm: Greeting.o
	${CC} ${LFLAGS} Greeting.o -o Greeting.wasm

Greeting.o: Greeting.cpp
	${CC} ${CFLAGS} Greeting.cpp -o Greeting.o

clean:
	rm -f *.o
	rm -f *.wasm
	rm -rf ${WEBROOT}
