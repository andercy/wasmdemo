// greeting.c
//
// A simple library that can be linked to a C program.
// Since this is meant to build to WASM, we can not include stdio.h.
#include "greeting.h"

int getNumber(void)
{
	return 27;
}

int add(int x)
{
	return x + 27;
}

int getGreetingLength()
{
	return 25;
}

// The functions below are not used by the JavaScript code.
// WASM functions called by JavaScript can actually only return ints.
// We will look at passing more complex data later.
// The functions below can be used from C code but not from JavaScript (yet).
char *getGreeting(void)
{
	return (char *)"Hello world, from clang!\n";
}

int copyGreeting(char *src, char *dest)
{
	// Note that since we can not include stdio.h, NULL is undefined, so we use (char *)0.
	if (src == (char *)0 || dest == (char *)0)
		return 0;
	return 1;
}
