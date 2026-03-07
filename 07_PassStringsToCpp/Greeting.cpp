// These are not the regular includes. It should be using the ones for wasi-libc, which we put in /usr/local/wasi/include/wasm32-wasi
#include <stdlib.h>
#include <string.h>

#include "Greeting.h"
#include "Utils.h"

// Globals
const char *DEFAULTGREETING = "Hello!";

Greeting::Greeting()
{
	Utils::log((char *)"Greeting::Greeting()");
	theGreeting = (char *)DEFAULTGREETING;
}

Greeting::~Greeting()
{
	Utils::log((char *)"Greeting::~Greeting()");
	theGreeting = (char *)DEFAULTGREETING;
}

// Non static Greeting member functions have been removed from export, since they need a Greeting object to work.
const char *Greeting::getGreeting()
{
	return theGreeting;
}

void Greeting::setGreeting(char *newGreeting)
{
	theGreeting = newGreeting;
}

int Greeting::getLength()
{
	// Changed to just use libc
	return strlen(theGreeting);
}

[[clang::export_name("Greeting_getDefaultLength")]]
int Greeting::getDefaultLength(void)
{
	return strlen(DEFAULTGREETING);
}
