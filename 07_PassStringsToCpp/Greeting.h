class Greeting
{
private:
	char *theGreeting;

public:
	Greeting();
	~Greeting();

	const char * getGreeting();
	void setGreeting(char *newGreeting);
	int getLength();

	static int getDefaultLength(void);
};
