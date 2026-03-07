// Now that since we have wasi-libc, we can use standard functions, like strlen, instead of having to make our own.
#include <stdlib.h>
#include <string.h>

#include "Utils.h"


Utils::Utils()
{
}

// For this demo, we removed functions that did not serve a useful purpose (add and getCount)

[[clang::import_name("consolelog")]] void consolelog(const char* str, int len);
void Utils::log(char *str)
{
	// Modified to use wasi-libc strlen function and deleted the one that was in Utils.
	consolelog(str, strlen(str));
}

[[clang::import_name("consolelogHexInt")]] void consolelogHexInt(int val);
void Utils::logHexInt(int val)
{
	consolelogHexInt(val);
}

// For JavaScript to pass objects to C/C++, it needs to allocate memory to put objects in, so we created this function to expose malloc to JS.
// Note that in some WASM tutorials, malloc is automatically exposed, but this was not happening for these demos.
// Note also how we add "w" to the function names for malloc and free.
// This is to prevent name conflicts with the wasi-libc functions.
[[clang::export_name("Utils_wmalloc")]]
int Utils::wmalloc(int size)
{
	return (int)malloc(size);
}

// If we expose malloc, we need to expose free also.
[[clang::export_name("Utils_wfree")]]
void Utils::wfree(int ptr)
{
	free((void *)ptr);
}
