#include "Greeting.h"

Greeting::Greeting()
{
	theGreeting = (char *)"Hello!\n";
}

// Using this decorator gives you control over what name is exported for the function.
// It also gives us finer control over what gets exported or not, since we do not use --export-all.
// Note for example that instance.exports does not include the constructor above,
// because it lacks the decorator, so it is not exported, unlike previous demos.
[[clang::export_name("Greeting_getGreeting")]]
char* Greeting::getGreeting()
{
	return theGreeting;
}

[[clang::export_name("Greeting_setGreeting")]]
void Greeting::setGreeting(char *newGreeting)
{
	theGreeting = newGreeting;
}

[[clang::export_name("Greeting_getLength")]]
int Greeting::getLength()
{
	int i = 0;
	while (theGreeting[i] != '\0')
		i++;
	return i;
}

[[clang::export_name("Greeting_getDefaultLength")]]
int Greeting::getDefaultLength(void)
{
	return 7;
}
