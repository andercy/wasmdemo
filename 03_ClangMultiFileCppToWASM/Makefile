# Makefile for 03_ClangMultiFileCppToWASM
#
# This Makefile demonstrates the use of wildcards and automatic variables to make a more dynamic Makefile.
# Note how now individual source files do not each have to have their own separate recipe listed.
# Everything in WASMOBJECTS will be built according to the rule for %.o.
# Note the addition of the .PRECIOUS target, which is needed to prevent make from deleting object files.
# Without this special target, it will delete the dependencies for the %.wasm target.
# If this target was specified without a wildcard, ("Library.wasm") it would not delete the .o files, so it would not need .PRECIOUS.
# Note also the use of the .PHONY target. This just tells make the target is not an actual file, so do not consider its date if it exists.

CC = clang++
CFLAGS = --target=wasm32 --no-standard-libraries -c
LFLAGS = --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry
WASMTARGET = Library.wasm
WASMOBJECTS = Greeting.o Utils.o
TARGETOBJECTS = ${WASMTARGET} index.html
WEBROOT = /var/www/html/wasmdemo03

# TARGETOBJECTS includes Library.wasm and index.html, so it will go to the rule for %.wasm to build Library.wasm.
# index.html must already exist, and make will fail if it does not find it, which is fine, because we custom created it.
.PHONY: demo
demo: ${TARGETOBJECTS}
	rm -rf ${WEBROOT}
	mkdir ${WEBROOT}
	cp -f index.html ${WEBROOT}
	cp -f *.wasm ${WEBROOT}

# Any time a .wasm file is required, it will use this rule to build it.
%.wasm: ${WASMOBJECTS}
	${CC} ${LFLAGS} ${WASMOBJECTS} -o $@

# Any time a .o file is required, it will use this rule to build it.
# Note that for this project, we are only building .o files from .cpp files.
# If other languages were being used, a more complex rule might be needed.
.PRECIOUS: %.o
%.o: %.cpp
	${CC} ${CFLAGS} $< -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.wasm
	rm -rf ${WEBROOT}
