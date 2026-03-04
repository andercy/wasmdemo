#include "Greeting.h"

// Nothing has changed in wasidemo06 for the Greeting class.
// In the next demo, we will add some actual functionality.
Greeting::Greeting()
{
	theGreeting = (char *)"Hello!\n";
}

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
