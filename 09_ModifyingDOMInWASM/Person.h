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
	void init(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG);
	~Person();
	// Note that thes functions are static so JavaScript can pass a pointer to the Person.
	static Person *newPerson(const char *firstName, const char *lastName, int ageYears, int heightCM, int weightG);
	static void deletePerson(Person *person, bool freePerson);
	static void deletePerson(Person *person);

	// Static getters for use from JavaScript
	static const char *getRefFirstName(Person *p);
	static const char *getRefLastName(Person *p);
	static const char *getFullName(Person *p);
	static int getAgeYears(Person *p);
	static int getHeightCM(Person *p);
	static int getWeightG(Person *p);

	// Instance getters for use from C++
	const char *getRefFirstName();
	const char *getRefLastName();
	const char *getFullName();
	int getAgeYears();
	int getHeightCM();
	int getWeightG();

	// Static setters for use from JavaScript
	static void setFirstName(Person *p, const char *firstName);
	static void setLastName(Person *p, const char *lastName);
	static void setAgeYears(Person *p, int ageYears);

	// Instance setters for use from C++
	void setFirstName(const char *firstName);
	void setLastName(const char *lastName);
	void setAgeYears(int ageYears);


	// Functions for displaying data
	static void displayPersonTable(Person *person, const char *elId);
	void displayPersonTable(const char *elId);
};
