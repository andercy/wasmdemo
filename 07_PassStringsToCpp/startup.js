window.onload = async () =>
{
	let el = document.getElementById("output");

	let memory = new WebAssembly.Memory({initial: 4});

	const importObject = { env:
			{
				consolelog: (str, len) => logFromWASM(memory.buffer, str, len),
				consolelogHexInt: (val) => logHexIntFromWASM(val),
				memory: memory
			}};

	const response = fetch("Library.wasm");
	const { instance } = await WebAssembly.instantiateStreaming(response, importObject);

	// Create a byte buffer to use for all references to WASM memory going forward.
	const wasmMemory = new Uint8Array(memory.buffer);

	instance.exports.main();

	// This is just a simple demo showing how the new CstrToJS function works.
	const greetingPtr = instance.exports.makeGreeting();
	const greeting = CstrToJS(wasmMemory, greetingPtr);
	appendInnerHTML(el, greeting + "<br />");		// Note this goes to an HTML element, not the console.

	// This is a simple test showing that the greeting string created previously is not affected when we change the string stored in the buffer.
	// If we create another string from the modified buffer, it does show a change, as expected.
	// This means that when the string is created, it is actually making a copy of all its characters for its own use.
	// Note that it is also a bad practice to modify data in the Data section. You should generally treat this as constant data.
	wasmMemory[greetingPtr] = 73;
	const greeting2 = CstrToJS(wasmMemory, greetingPtr);
	console.log(greeting + "\n" + greeting2 + "\n");

	// Now we get to the true point of this demo, passing strings to WASM.
	// First, we allocate 2 blocks of memory with the exported malloc function.
	const mem1 = instance.exports.Utils_wmalloc(256);
	const mem2 = instance.exports.Utils_wmalloc(256);
	console.log("mem1:");
	logHexIntFromWASM(mem1);
	console.log("mem2:");
	logHexIntFromWASM(mem2);
	copyStrToBuffer(wasmMemory, mem1, "One.\n");
	copyStrToBuffer(wasmMemory, mem2, "Two.\n");

	// See comments for this function in Main.cpp. It creates a memory leak for demonstration purposes.
	instance.exports.printStrings(mem1, mem2);

	// Now we perform an allocation to see what happens after the memory leak.
	// Notice that the memory is allocated after the memory allocated inside printStrings.
	// The total string length is 11 (2x 5 bytes strings + 1 null terminator).
	// This gives more than 4 bytes, reserved for use by malloc, before the next 16 byte boundary.
	// Note that in our test run, the value for mem3 was exactly 16 greater than mem2.
	// This means that memory is still reserved and is therefore lost. (leaked)
	const mem3 = instance.exports.Utils_wmalloc(256);
	console.log("mem3:");
	logHexIntFromWASM(mem3);

	// Now, we allocate 2 more blocks of memory with the exported malloc function.
	const mem4 = instance.exports.Utils_wmalloc(256);
	const mem5 = instance.exports.Utils_wmalloc(256);
	console.log("mem4:");
	logHexIntFromWASM(mem4);
	console.log("mem5:");
	logHexIntFromWASM(mem5);
	copyStrToBuffer(wasmMemory, mem4, "Four.\n");
	copyStrToBuffer(wasmMemory, mem5, "Five.\n");

	// This time, we call the combineStrings function. This has no leak, since it returns the pointer.
	// We can just free the memory allocated by combineStrings with the pointer stored in mem6.
	const mem6 = instance.exports.combineStrings(mem4, mem5);
	console.log("mem6:");
	logHexIntFromWASM(mem6);
	const combined = CstrToJS(wasmMemory, mem6);
	console.log(combined);

	// Let's allocate memory after the combined string, just to see where memory gets allocated next.
	// Note that in our test run, the value for mem7 was exactly 32 greater than mem6.
	// This is because the combined string length just barely does not fit in a single 16 byte block.
	// (2x 6 byte strings + 1 null terminator + 4 bytes for malloc = 17 bytes)
	const mem7 = instance.exports.Utils_wmalloc(256);
	console.log("mem7:");
	logHexIntFromWASM(mem7);

	// You can try inspecting the memory before and after free is called.
	// malloc uses the 4 bytes before the pointer to track what memory is used or available.
	// How exactly it does this is not standardized, so you should not depend on any specific behavior or touch these bytes.
	// It is just interesting to watch.
	// When stepping through WebAssembly code in the browser Dev Tools, you can inspect the memory by creating a buffer in the console, using the memories object, like this:
	// m = new Uint8Array(memories[0].buffer)
	// In JavaScript, you can just inspect the memory by looking at wasmMemory.
	// WebAssembly code can get very long and complex.
	// When stepping through, it helps to look for function calls, like those to malloc, to know where you are.
	instance.exports.Utils_wfree(mem1);
	instance.exports.Utils_wfree(mem2);
	instance.exports.Utils_wfree(mem3);
	instance.exports.Utils_wfree(mem4);
	instance.exports.Utils_wfree(mem5);
	instance.exports.Utils_wfree(mem6);
	instance.exports.Utils_wfree(mem7);
};

function logFromWASM(buffer, str, len)
{
		const bytes = new Uint8Array(buffer, str, len);
		const text = new TextDecoder("utf8").decode(bytes);
		console.log(text);
}

function logHexIntFromWASM(val)
{
	console.log("0x" +val.toString(16));
}

function appendInnerHTML(el, text)
{
	if (el && el.innerHTML != undefined)
	{
		if (el.innerHTML == null)
			el.innerHTML = text;
		else
			el.innerHTML = el.innerHTML + text;
	}
}

/**
 * Converts a C string to a JS string
 * @param {Uint8Array} memory - A Uint8Array representing a block of memory
 * @param {number} ptr - The index into the memory block where C string is located
 * @returns {string} - The JavaScript string
 */
function CstrToJS(memory, ptr)
{
	let len = 0;
	while (memory[ptr + len] !== 0) len++;
	return new TextDecoder("utf8").decode(memory.subarray(ptr, ptr + len));
}

/**
 * A simple function to transfer a string into WASM memory
 * @param {Uint8Array} memory - A Uint8Array representing a block of memory
 * @param {number} ptr - The index into the memory block the string should be copied to
 * @param {string} str - The JavaScript string to copy
 */
function copyStrToBuffer(memory, ptr, str)
{
	let len = str.length;
	let i = 0;
	while (i < len)
	{
		memory[ptr + i] = str.charCodeAt(i);
		i++;
	}
	memory[ptr + len] = 0;
}
