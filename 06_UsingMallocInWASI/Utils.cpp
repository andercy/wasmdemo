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

[[clang::import_name("consolelog")]] void consolelog(const char* str, int len);
void Utils::log(char *str)
{
	consolelog(str, Utils::strlen(str));
}

// Logs an integer in hexadecimal format, mostly so we can display memory addresses.
[[clang::import_name("consolelogHexInt")]] void consolelogHexInt(int val);
void Utils::logHexInt(int val)
{
	consolelogHexInt(val);
}
