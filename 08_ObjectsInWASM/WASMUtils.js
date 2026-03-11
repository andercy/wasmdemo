/**
 * Loads a WASM file and initializes it.
 * @param {string} wasmFile The name of the file containing the WASM code to load.
 * @returns { {exports: Object, memory: Uint8Array}} A promise resolving to an object containing the WASM exports and memory.
 */
async function loadWASM(wasmFile)
{
	let wasmMemory = new WebAssembly.Memory({initial: 4});

	const importObject = { env:
			{
				Utils_consolelog: (str, len) => logFromWASM(wasmMemory.buffer, str, len),
				Utils_consolelogHexInt: (val) => logHexIntFromWASM(val),
				memory: wasmMemory
			}};

	const response = fetch(wasmFile);
	const { instance } = await WebAssembly.instantiateStreaming(response, importObject);
	const exports = instance.exports;
	// Create a byte buffer to use for all references to WASM memory going forward.
	const memory = new Uint8Array(wasmMemory.buffer);
	return { exports, memory };
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

/**
 * This function just calls console.log.
 * It exists just to be called from WASM.
 * JavaScript code should just call console.log directly.
 */
function logFromWASM(buffer, str, len)
{
		const bytes = new Uint8Array(buffer, str, len);
		const text = new TextDecoder("utf8").decode(bytes);
		console.log(text);
}

/**
 * Like logFromWASM, but for hexadecimal integers.
 * Also should not need to be called from JavaScript code.
 */
function logHexIntFromWASM(val)
{
	console.log("0x" + val.toString(16));
}

/**
 * This function sets up the parameters needed to be passed to a WASM function and then calls the function.
 * For string parameters, it handles all allocation, copying and freeing of memory necessary to call WASM.
 * For number parameters, it passes them directly.
 * Other parameter types, such as objects or arrays, are not currently supported.
 * @param {*} wasm The object containing the exports and memory for the WASM instance.
 * @param {*} functionRef An instance of the WASM function to call. This should be a function in the exports object.
 * @param {*} parameterArray An array of parameters that functionRef expects. Only string and integer parameters are supported right now.
 * @returns The value returned from the function given by functionRef. This is always an int.
 */
function callWASM(wasm, functionRef, parameterArray)
{
	let params = new Array(parameterArray.length);

	// Allocate memory for any string parameters and copy the strings to the allocated WASM memory space.
	for (let i = 0; i < parameterArray.length; i++)
	{
		if (typeof parameterArray[i] === "string")
		{
			params[i] = wasm.exports.Utils_wmalloc(parameterArray[i].length + 1);
			copyStrToBuffer(wasm.memory, params[i], parameterArray[i]);
		}
		else if (typeof parameterArray[i] === "number")
		{
			params[i] = parameterArray[i];
		}
	}

	// Actually call the function, using call and the span operator to take the parameters out of the array.
	// Note that the null is for the "this" object, which does not exist for this call.
	let result = functionRef.call(null, ...params);

	// Free memory allocated for string parameters.
	for (let i = 0; i < parameterArray.length; i++)
	{
		if (typeof parameterArray[i] === "string")
		{
			wasm.exports.Utils_wfree(params[i]);
		}
	}

	// This is always just an int, although it could be a pointer.
	return result;
}
