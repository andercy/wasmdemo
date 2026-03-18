class WASMUtils
{
public:
	static void log(char *str);
	static void logHexInt(int val);
	static void updateDOMElement(const char *elId, const char *attribute, const char *value);
	static void *wmalloc(int size);
	static void wfree(void *ptr);
	static char *combineStrings(char *str1, char *str2);
};
