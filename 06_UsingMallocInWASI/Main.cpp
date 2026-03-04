#include "Utils.h"
#include "Greeting.h"

// Note the use of stdlib.h. It is needed for malloc.
// This is not the regular stdlib.h. It should be using the one for wasi-libc, which we put in /usr/local/wasi/include/wasm32-wasi
#include <stdlib.h>

// Function Prototypes
void helloStack(int num);
void helloAgainStack(int num);

// Main
[[clang::export_name("main")]]
int main()
{
	// These logging statements illustrate the memory structure of WASM.
	// This first log shows that the address of program code (loaded from the program image) is stored in the lower part of memory.
	// This usually will show that the address of the main function is at 0x0001.
	char *helloCode = (char *)"Hello, from Code!";
	Utils::log(helloCode);
	Utils::logHexInt((int)&main);

	// This log shows that the address of program data (data loaded from the program image) is stored in the lower part of memory, after the Code.
	// For example, one time this was run, the pointer to helloData from logHexInt showed a value of 0x047C.
	char *helloData = (char *)"Hello, from Data!";
	Utils::log(helloData);
	Utils::logHexInt((int)helloData);

	// This shows where stack memory is located. (See function definition below.)
	helloStack(27);

	// Here we allocate memory with malloc, which gives us memory from the heap.
	// In our sample run, the address for the allocation for helloHeap was 0x10680.
	char *helloHeap = (char *)malloc(1024);
	Utils::log((char *)"Hello, from Heap!");
	Utils::logHexInt((int)helloHeap);
	// The address for the second allocation, for helloAgainHeap, gave us 0x10A90
	// That is 0x0400 + 0x0010 bytes above the first allocation.
	// Notice how the heap grows upward. The 0x0400 bytes are for the first allocation of memory (0x0400 = 1024 decimal),
	// and the 0x0010 (16 decimal) extra bytes are for internal use by malloc.
	char *helloAgainHeap = (char *)malloc(1024);
	Utils::log((char *)"Hello again, from Heap!");
	Utils::logHexInt((int)helloAgainHeap);
	// As always, remember to free any memory allocated with malloc.
	free(helloHeap);
	free(helloAgainHeap);

	// This shows how malloc reuses memory after it is freed.
	// On our test run, this gave the address 0x10680 again, which was previously in use by helloHeap.
	char *goodbyeHeap = (char *)malloc(1024);
	Utils::log((char *)"Goodbye, from Heap!");
	Utils::logHexInt((int)goodbyeHeap);
	free(goodbyeHeap);

	return 0;
}

void helloStack(int num)
{
	// This shows the first allocation of Stack memory.
	// We allocate Stack data when a parameter is passed to this function.
	// In this case it is an int, but it is a dummy value.
	// We are just using it to see where the Stack is located.
	// In our sample run, we got 0x1066C for the address of num.
	// Notice how the Stack addresses and Heap addresses are close to each other. (0x10680 - 0x1066C = 0x14 = 20 decimal)
	// This small difference can easily be accounted for by other data already on the Stack and Heap.
	Utils::log((char *)"Hello, from Stack!");
	Utils::logHexInt((int)(&num));

	// Next we call another function with a dummy argument.
	helloAgainStack(42);
}

void helloAgainStack(int num)
{
	// Since the parameter for the previous function is still on the stack, new space is allocated for the parameter for this function.
	// In our sample run, we got 0x1065C, which is 16 bytes lower than the parameter for the first function, so we see the stack grows downward.
	Utils::log((char *)"Hello again, from Stack!");
	Utils::logHexInt((int)(&num));
}
