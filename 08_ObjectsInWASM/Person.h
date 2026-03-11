class Person
{
private:
	char *firstName;
	char *lastName;
	int ageYears;
	int heightCM;
	int weightG;

public:
	Person();
	Person(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG);
	void Init(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG);
	~Person();
	// These functions exist just to use the constructor or destructor for JavaScript.
	// Note that they are static so JavaScript can pass a pointer to the Person.
	static Person *NewPerson(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG);
	static void DeletePerson(Person *person, bool freePerson);
	static void DeletePerson(Person *person);

	// Note that each member function has a static version to be called from JavaScript.
	// This is so we can pass in the pointer to a Person from JavaScript, which then returns the member data.
	// Functions you do not intend to call from JavaScript do not need a static version.
	static const char *getRefFirstName(Person *p);
	static const char *getRefLastName(Person *p);
	static const char *getFullName(Person *p);
	static int getAgeYears(Person *p);
	static int getHeightCM(Person *p);
	static int getWeightG(Person *p);

	const char *getRefFirstName();
	const char *getRefLastName();
	const char *getFullName();
	int getAgeYears();
	int getHeightCM();
	int getWeightG();
};
