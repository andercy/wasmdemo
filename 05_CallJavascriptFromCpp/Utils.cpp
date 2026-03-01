#include "Utils.h"

Utils::Utils()
{
}

[[clang::export_name("Utils_getCount")]]
int Utils::getCount()
{
	return 1024;
}

[[clang::export_name("Utils_strlen")]]
int Utils::strlen(char *str)
{
	int i = 0;
	while (str[i] != '\0') i++;
	return i;
}

[[clang::export_name("Utils_add")]]
int Utils::add(int a, int b)
{
	return a + b;
}

// The import_name decorator tells clang that this code expects an external function to be made available.
// This must be provided as a function in the importObject passed to instantiateStreaming.
// From within C++ code, we can call Utils::log, which calls consolelog, which calls logFromWASM, which actually writes the text to the console.
// Now we can log text to the browser console from any C++ code, just by using Utils::log!
[[clang::import_name("consolelog")]] void consolelog(const char* str, int len);
void Utils::log(char *str)
{
	// Note that since JavaScript does not use zero terminated strings like C/C++,
	// we need to pass the length, so JavaScript can create the correct length string.
	consolelog(str, Utils::strlen(str));
}
