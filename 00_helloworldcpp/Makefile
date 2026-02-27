# Makefile for helloworldcpp
#
# This is the most basic of Makefiles.
CC = clang++

# Link object file into an executable
all: helloworldcpp
	${CC} helloworldcpp.o -o helloworldcpp

# Compile C++ code into an object file
helloworldcpp.o: helloworldcpp.cpp
	${CC} -c helloworldcpp.cpp -o helloworldcpp.o

clean:
	rm -f helloworldcpp
	rm -f *.o
