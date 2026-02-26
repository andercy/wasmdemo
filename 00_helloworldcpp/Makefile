# Makefile for helloworldcpp
#
# This is the most basic of Makefiles.
CC = clang++

all: helloworldcpp
	${CC} helloworldcpp.o -o helloworldcpp

helloworldcpp.o: helloworldcpp.cpp
	${CC} -c helloworldcpp.cpp -o helloworldcpp.o

clean:
	rm -f helloworldcpp
	rm -f *.o
