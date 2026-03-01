#include "Utils.h"

// This file exists just to hold the main function, because it does not belong in Greeting or Utils.
[[clang::export_name("main")]]
int main()
{
	// Utils::log is actually calling an imported JavaScript function.
	Utils::log((char *)"Hello, Browser from C++!");
	return 0;
}
