// These are not the regular includes. It should be using the ones for wasi-libc, which we put in /usr/local/wasi/include/wasm32-wasi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Person.h"
#include "WASMUtils.h"

// Constructors / Destructors
// Note that Constructors and destructors actually call separate init and delete functions.
// This is because compiling WASM with clang does not fully support all C++ features.
// This includes the new and delete operators and exception handling.
// So we must put the initialization and cleanup logic in separate functions, so they can be called on memory allocated with malloc.

Person::Person()
{
	init(NULL, NULL, 0, 0, 0);
}

/// @brief Constructor
/// @param firstName First Name
/// @param lastName Last Name
/// @param ageYears Age in Years
/// @param heightCM Height in Centimeters
/// @param weightG Weight in Grams
Person::Person(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG)
{
	init(firstName, lastName, ageYears, heightCM, weightG);
}

void Person::init(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG)
{
	// Note how we make a copy of the strings passed to this function with strdup. (Rule 2)
	// strdup uses malloc and returns a pointer to allocated memory.
	this->firstName = firstName ? strdup(firstName) : NULL;
	this->lastName = lastName ? strdup(lastName) : NULL;
	this->ageYears = ageYears;
	this->heightCM = heightCM;
	this->weightG = weightG;
}

Person::~Person()
{
	// The destructor is typically called when a Person on the stack goes out of context.
	// We pass false here, because free should not be called on stack data.
	deletePerson(this, false);
}

/// @brief This just calls init. It exists to be exported so you can create a new Person from JavaScript.
/// @param firstName First Name, Given Name...
/// @param lastName Last Name, Surname, Family Name...
/// @param ageYears Age in years, rounded down
/// @param heightCM Height in centimeters
/// @param weightG Weight in grams (remember 1kg = 1000g)
/// @return Pointer to a new Person object
[[clang::export_name("Person_newPerson")]]
Person *Person::newPerson(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG)
{
	// Note that since we can not use new, we must allocate data then initialize it with init.
	Person *person = (Person *)malloc(sizeof(Person));
	person->init(firstName, lastName, ageYears, heightCM, weightG);
	return person;
}

/// @brief This will free all the resources for a Person.
/// @param person Pointer to the Person object to delete
/// @param freePerson Whether to free the Person object itself. If the Person is allocated on the stack, this should not be done.
void Person::deletePerson(Person *person, bool freePerson)
{
	if (!person) return;
	if (person->firstName)
	{
		free(person->firstName);
		person->firstName = NULL;
	}
	if (person->lastName)
	{
		free(person->lastName);
		person->lastName = NULL;
	}
	if (freePerson)
		free(person);
}
// When deleting a Person from JavaScript, it will always be located on the heap,
// so for the exported version of DeletePerson, we always free the actual Person object.
// We could make the caller in JavaScript call wfree, but this just makes it a little cleaner.
[[clang::export_name("Person_deletePerson")]]
void Person::deletePerson(Person *person)
{
	deletePerson(person, true);
}



/// @brief Returns a pointer to the First Name String. Do NOT call wfree on this!
/// @return First Name
[[clang::export_name("Person_getRefFirstName")]]
const char *Person::getRefFirstName(Person *p)
{
	return p->firstName;
}
const char *Person::getRefFirstName()
{
	return firstName;
}

/// @brief Returns a pointer to the Last Name String. Do NOT call wfree on this!
/// @return Last Name
[[clang::export_name("Person_getRefLastName")]]
const char *Person::getRefLastName(Person *p)
{
	return p->lastName;
}
const char *Person::getRefLastName()
{
	return lastName;
}

// Note for the getFullName function, the function name does not start with getRef.
// Therefore, by Rule 7, the caller is responsible for freeing the returned string.
// The function documentation reminds the programmer of this.

/// @brief Returns a pointer to the full name string. Caller must call wfree on the returned pointer!
/// @param p Person to get name for
/// @return Pointer to full name string. Must be freed by caller.
[[clang::export_name("Person_getFullName")]]
const char *Person::getFullName(Person *p)
{
	return p->getFullName();
}
const char *Person::getFullName()
{
	if (!firstName || !lastName) return NULL;

	int firstNameLen = strlen(firstName);
	int lastNameLen = strlen(lastName);
	char *fullName = (char *)malloc(firstNameLen + lastNameLen + 2);	// Room for both strings, plus space for separator and null terminator.
	strcpy(fullName, firstName);
	fullName[firstNameLen] = ' ';
	strcpy(fullName + firstNameLen + 1, lastName);
	return fullName;
}

/// @brief Returns the Person's age in years as an int
/// @return Age in years
[[clang::export_name("Person_getAgeYears")]]
int Person::getAgeYears(Person *p)
{
	return p->ageYears;
}
int Person::getAgeYears()
{
	return ageYears;
}

/// @brief Returns the Person's height in centimeters as an int
/// @return Height in centimeters
[[clang::export_name("Person_getHeightCM")]]
int Person::getHeightCM(Person *p)
{
	return p->heightCM;
}
int Person::getHeightCM()
{
	return heightCM;
}

/// @brief Returns the Person's weight in grams as an int
/// @return Weight in grams
[[clang::export_name("Person_getWeightG")]]
int Person::getWeightG(Person *p)
{
	return p->weightG;
}
int Person::getWeightG()
{
	return weightG;
}

[[clang::export_name("Person_setFirstName")]]
void Person::setFirstName(Person *p, const char *newFirstName)
{
	p->setFirstName(newFirstName);
}
void Person::setFirstName(const char *newFirstName)
{
	if (newFirstName)
	{
		if (this->firstName)
		{
			free(this->firstName);
			this->firstName = NULL;
		}
		this->firstName = (char *)malloc(strlen(newFirstName) + 1);
		strcpy(this->firstName, newFirstName);
	}
}

[[clang::export_name("Person_setLastName")]]
void Person::setLastName(Person *p, const char *lastName)
{
	p->setLastName(lastName);
}
void Person::setLastName(const char *newLastName)
{
	if (newLastName)
	{
		if (this->lastName)
		{
			free(this->lastName);
			this->lastName = NULL;
		}
		this->lastName = (char *)malloc(strlen(newLastName) + 1);
		strcpy(this->lastName, newLastName);
	}
}

[[clang::export_name("Person_setAgeYears")]]
void Person::setAgeYears(Person *p, int ageYears)
{
	p->setAgeYears(ageYears);
}
void Person::setAgeYears(int newAgeYears)
{
	ageYears = newAgeYears;
}

/// @brief Displays the person's information in an HTML table in the element with the id provided
/// @param elId The id of the HTML element to display the table in
[[clang::export_name("Person_displayPersonTable")]]
void Person::displayPersonTable(Person *person, const char *elId)
{
	person->displayPersonTable(elId);
}
void Person::displayPersonTable(const char *elId)
{
	// This is old style string manipulation, how it was done in old C, before C++.
	// We could implement a String class to make string operations easier, but this is just for demonstration purposes.
	// The C++ String class is not available, because we do not have access to all the C++ libraries.
	char *result, *resultBak;

	const char *fullName = getFullName();
	result = resultBak = WASMUtils::combineStrings((char *)"<table><tr><td colspan=\"2\">", (char *)fullName);
	free((void *)fullName);

	result = WASMUtils::combineStrings(result, (char *)"</td></tr><tr><td>First Name:</td><td>");
	free(resultBak);
	resultBak = result;

	result = WASMUtils::combineStrings(result, firstName);
	free(resultBak);
	resultBak = result;

	result = WASMUtils::combineStrings(result, (char *)"</td></tr><tr><td>Last Name:</td><td>");
	free(resultBak);
	resultBak = result;

	result = WASMUtils::combineStrings(result, lastName);
	free(resultBak);
	resultBak = result;

	result = WASMUtils::combineStrings(result, (char *)"</td></tr><tr><td>Age:</td><td>");
	free(resultBak);
	resultBak = result;

	// Use snprintf to convert number to a string by printing into a buffer (ageStr).
	// Note that 12 is the maximum length for a 32bit signed integer: 10 decimal digits, 1 negative sign, 1 null terminator
	// Using the snprintf function causes WASI to add the wasi_snapshot_preview1 imports. (See loadWASM in WASMInstance.js)
	char ageStr[12];
	snprintf(ageStr, 12, "%d", ageYears);
	result = WASMUtils::combineStrings(result, ageStr);
	free(resultBak);
	resultBak = result;
	result = WASMUtils::combineStrings(result, (char *)"</td></tr><tr><td>Height:</td><td>");
	free(resultBak);
	resultBak = result;

	char heightStr[12];
	snprintf(heightStr, 12, "%d", heightCM);
	result = WASMUtils::combineStrings(result, heightStr);
	free(resultBak);
	resultBak = result;
	result = WASMUtils::combineStrings(result, (char *)" cm</td></tr><tr><td>Weight:</td><td>");
	free(resultBak);
	resultBak = result;

	char weightStr[12];
	snprintf(weightStr, 12, "%d", weightG);
	result = WASMUtils::combineStrings(result, weightStr);
	free(resultBak);
	resultBak = result;
	result = WASMUtils::combineStrings(result, (char *)" g</td></tr></table>");
	free(resultBak);

	WASMUtils::updateDOMElement(elId, "innerHTML", result);
	free(result);
}
