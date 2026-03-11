#include <stdlib.h>
#include <string.h>

#include "Utils.h"

// Globals
const char *HELLOMAIN = "Hello, from main!";

// Main
// For this demo, main does not do much.
// Most functions are called from JavaScript.
[[clang::export_name("main")]]
int main()
{
	Utils::log((char *)HELLOMAIN);

	return 0;
}
