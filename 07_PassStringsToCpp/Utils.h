class Utils
{
public:
	Utils();
	static void log(char *str);
	static void logHexInt(int val);
	static int wmalloc(int size);
	static void wfree(int ptr);
};
