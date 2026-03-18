#include <stdlib.h>
#include <string.h>

#include "WASMUtils.h"


// Imported Functions

[[clang::import_name("WASMUtils_consolelog")]] void consolelog(const char* str);
void WASMUtils::log(char *str)
{
	consolelog(str);
}

[[clang::import_name("WASMUtils_consolelogHexInt")]] void consolelogHexInt(int val);
void WASMUtils::logHexInt(int val)
{
	consolelogHexInt(val);
}

[[clang::import_name("WASMUtils_updateElement")]] void updateElement(const char *elId, const char *attribute, const char *value);
void WASMUtils::updateDOMElement(const char *elId, const char *attribute, const char *value)
{
	updateElement(elId, attribute, value);
}


// Exported Memory Functions

[[clang::export_name("WASMUtils_wmalloc")]]
void *WASMUtils::wmalloc(int size)
{
	return (void *)malloc(size);
}

[[clang::export_name("WASMUtils_wfree")]]
void WASMUtils::wfree(void *ptr)
{
	if (ptr)
		free(ptr);
}


// Utility functions for use by C/C++ code
// Since this is not strictly WASM related, this may be moved to a different class later.

/// @brief Combines two strings in memory into a new string. It is the responsibility of the caller to free the memory. The caller must also free the source strings.
/// @param str1 First string
/// @param str2 Second string, placed immediately following first
/// @return Pointer to a new string that contains the combined strings. Caller must free this memory when done.
char *WASMUtils::combineStrings(char *str1, char *str2)
{
	if (!str1 || !str2) return NULL;

	char *strPtr1 = str1;
	char *strPtr2 = str2;
	int len1 = strlen(strPtr1);
	int len2 = strlen(strPtr2);

	// Dynamically allocate memory for the combined string, remembering to add 1 for terminating 0.
	char *strPtr3 = (char *)malloc(len1 + len2 + 1);

	strcpy(strPtr3, strPtr1);
	strcpy(strPtr3 + len1, strPtr2);

	return strPtr3;
}
