#include <stdlib.h>
#include <string.h>

#include "Utils.h"
#include "Greeting.h"

// Function Prototypes

// Globals
const char *HELLOMAIN = "Hello, from main!";

// Main
[[clang::export_name("main")]]
int main()
{
	Utils::log((char *)HELLOMAIN);

	return 0;
}

[[clang::export_name("makeGreeting")]]
int makeGreeting()
{
	// This function creates a new Greeting object, but the greeting is just the default, so the address should be a low value in the Data section.
	Greeting g;
	return (int)g.getGreeting();

	// Since g is local to this function, it goes out of scope and its destructor is called.
	// Since the string for the greeting text is in the Data section, it remains there, and can be used, but this is a bad practice.
	// You should never refer to data for an object that has been released.
	// In the next demo, we will discuss how to handle this more elegantly.
}

// Takes 2 string pointers and combines them into a new string, leaving the originals intact.
// Then it prints the combined strings to the console.
// It intentionally leaves memory unfreed for demonstration purposes.
[[clang::export_name("printStrings")]]
void printStrings(int str1, int str2)
{
	char *strPtr1 = (char *)str1;
	char *strPtr2 = (char *)str2;
	int len1 = strlen(strPtr1);
	int len2 = strlen(strPtr2);

	Utils::log((char *)"printStrings()");

	// Dynamically allocate memory for the combined string, remembering to add 1 for terminating 0.
	char *strPtr3 = (char *)malloc(len1 + len2 + 1);
	Utils::logHexInt((int)strPtr3);

	int i = 0;
	while (i < len1)
	{
		strPtr3[i] = strPtr1[i];
		i++;
	}
	i = 0;
	while (i < len2)
	{
		strPtr3[i + len1] = strPtr2[i];
		i++;
	}
	strPtr3[len1 + len2] = '\0';
	Utils::log(strPtr3);

	// Here we intentionally leave a memory leak.
	// We should free strPtr3 when we are done with it.
	// But we can't once the function ends, because the local variable strPtr3 goes out of scope.
}

// Unlike the printStrings function, this function combines the strings and returns the pointer to it.
// It is the responsibility of the caller to free the memory.
/// @brief Combines two strings in memory into a new string
/// @param str1 First string
/// @param str2 Second string, placed following first
/// @return Pointer to a new string that contains the combined strings. Caller must free this memory when done.
[[clang::export_name("combineStrings")]]
int combineStrings(int str1, int str2)
{
	char *strPtr1 = (char *)str1;
	char *strPtr2 = (char *)str2;
	int len1 = strlen(strPtr1);
	int len2 = strlen(strPtr2);

	Utils::log((char *)"combineStrings()");

	// Dynamically allocate memory for the combined string, remembering to add 1 for terminating 0.
	char *strPtr3 = (char *)malloc(len1 + len2 + 1);
	Utils::logHexInt((int)strPtr3);

	int i = 0;
	while (i < len1)
	{
		strPtr3[i] = strPtr1[i];
		i++;
	}
	i = 0;
	while (i < len2)
	{
		strPtr3[i + len1] = strPtr2[i];
		i++;
	}
	strPtr3[len1 + len2] = '\0';

	return (int)strPtr3;
}
