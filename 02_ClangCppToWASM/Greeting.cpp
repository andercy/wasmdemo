#include "Greeting.h"

Greeting::Greeting()
{
	theGreeting = (char *)"Hello!\n";
}

char *Greeting::getGreeting()
{
	return theGreeting;
}

void Greeting::setGreeting(char *newGreeting)
{
	theGreeting = newGreeting;
}

int Greeting::getLength()
{
	int i = 0;
	while (theGreeting[i] != '\0')
		i++;
	return i;
}

int Greeting::getDefaultLength(void)
{
	return 7;
}
