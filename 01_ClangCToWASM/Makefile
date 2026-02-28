# Makefile for 01_ClangCToWASM

# Note that unlike the previous demo, we are just compiling C code, not C++, so we just use clang, not clang++.
CC = clang
WEBROOT = /var/www/html/demo01

demo: greeting.wasm index.html
	rm -rf ${WEBROOT}
	mkdir ${WEBROOT}
	cp -f index.html ${WEBROOT}
	cp -f greeting.wasm ${WEBROOT}

# To get clang to use wasm32, had to install lld-18: sudo apt install lld-18.
# The flag --target=wasm32 is what makes clang produce WASM for its output.
# Flags starting with "-Wl," are flags that are passed to the linker.
greeting.wasm: greeting.o
	${CC} --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry greeting.o -o greeting.wasm

greeting.o: greeting.c
	${CC} --target=wasm32 --no-standard-libraries -c greeting.c -o greeting.o

clean:
	rm -f *.o
	rm -f *.wasm
	rm -rf ${WEBROOT}
