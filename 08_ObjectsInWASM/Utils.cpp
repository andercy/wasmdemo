#include <stdlib.h>
#include <string.h>

#include "Utils.h"


[[clang::import_name("Utils_consolelog")]] void consolelog(const char* str, int len);
void Utils::log(char *str)
{
	// Modified to use wasi-libc strlen function and deleted the one that was in Utils.
	consolelog(str, strlen(str));
}

[[clang::import_name("Utils_consolelogHexInt")]] void consolelogHexInt(int val);
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
	if (ptr)
		free((void *)ptr);
}

/// @brief Combines two strings in memory into a new string. It is the responsibility of the caller to free the memory. The caller must also free the source strings.
/// @param str1 First string
/// @param str2 Second string, placed immediately following first
/// @return Pointer to a new string that contains the combined strings. Caller must free this memory when done.
[[clang::export_name("Utils_combineStrings")]]
char *Utils::combineStrings(char *str1, char *str2)
{
	if (!str1 || !str2) return NULL;

	char *strPtr1 = str1;
	char *strPtr2 = str2;
	int len1 = strlen(strPtr1);
	int len2 = strlen(strPtr2);

	// Dynamically allocate memory for the combined string, remembering to add 1 for terminating 0.
	char *strPtr3 = (char *)malloc(len1 + len2 + 1);
	Utils::logHexInt((int)strPtr3);

	strcpy(strPtr3, strPtr1);
	strcpy(strPtr3 + len1, strPtr2);

	return strPtr3;
}
