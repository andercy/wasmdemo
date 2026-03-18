// All WASM related JavaScript code is now contained within this class.
// You must instantiate the class with a file name then call loadWASM to be able to use it properly.
export default class WASMInstance
{
	constructor(wasmFile, memSize)
	{
		this.wasmFile = wasmFile;
		this.memSize = memSize ?? 4;
		this.instance = null;
		this.exports = null;
		this.memory = null;
	}

	/**
	* Loads a WASM file and initializes it.
	* @param {string} wasmFile The name of the file containing the WASM code to load.
	* @returns { {exports: Object, memory: Uint8Array}} A promise resolving to an object containing the WASM exports and memory.
	*/
	async loadWASM()
	{
		const wasmMemory = new WebAssembly.Memory({initial: this.memSize});

		const importObject =
		{
			env:
			{
				WASMUtils_consolelog: (str) => this.logFromWASM(str),
				WASMUtils_consolelogHexInt: (val) => this.logHexIntFromWASM(val),
				WASMUtils_updateElement: (elId, attribute, value) => this.updateElementFromWASM(elId, attribute, value),	// The new function
				memory: wasmMemory
			},
			// Using the snprintf function in Person.displayPersonTable causes WASI to add JavaScript functions it needs to import to an object named wasi_snapshot_preview1.
			// If this is not added, we will get an error saying something like this:
			// "WASMInstance.js:51 Error loading WASM file: TypeError: WebAssembly.instantiate(): Import #1 "wasi_snapshot_preview1": module is not an object or function".
			wasi_snapshot_preview1:
			{
				fd_close: (p1) => { console.log("fd_close: " + p1); },
				fd_seek: (p1, p2, p3, p4) => { console.log("fd_seek: " + p1 + ", " + p2 + ", " + p3 + ", " + p4); },
				fd_write: (p1, p2, p3, p4) => { console.log("fd_write: " + p1 + ", " + p2 + ", " + p3 + ", " + p4); throw new Error("fd_write not implemented"); }
			}
		};

		// instantiate was placed in a try catch to make it easier to debug when something is wrong with importObject.
		try
		{
			const response = fetch(this.wasmFile);
			const { instance } = await WebAssembly.instantiateStreaming(response, importObject);
			this.instance = instance;
			this.exports = instance.exports;
			// Create a byte buffer to use for all references to WASM memory going forward.
			this.memory = new Uint8Array(wasmMemory.buffer);
		}
		catch (ex)
		{
			console.error("Error loading WASM file:", ex);
		}
	}

	/**
	* Converts a C string to a JS string
	* @param {number} ptr - The index into the memory block where C string is located
	* @returns {string} - The JavaScript string
	*/
	CstrToJS(ptr)
	{
		let len = 0;
		while (this.memory[ptr + len] !== 0) len++;
		return new TextDecoder("utf8").decode(this.memory.subarray(ptr, ptr + len));
	}

	/**
	* A simple function to transfer a string into WASM memory
	* @param {number} ptr - The index into the memory block the string should be copied to
	* @param {string} str - The JavaScript string to copy
	*/
	copyStrToBuffer(ptr, str)
	{
		let len = str.length;
		let i = 0;
		while (i < len)
		{
			this.memory[ptr + i] = str.charCodeAt(i);
			i++;
		}
		this.memory[ptr + len] = 0;
	}

	/**
	* This function just calls console.log.
	* It exists just to be called from WASM.
	* JavaScript code should just call console.log directly.
	*/
	logFromWASM(str)
	{
		const jsStr = this.CstrToJS(str);
		console.log(jsStr);
	}

	/**
	* Updates an element in the DOM from WASM.
	* Exists only to be called from WASM.
	* @param {string} elId id attribute of element to modify
	* @param {string} attribute Name of the attribute to modify. If this is "innerHTML", this will set the innerHTML property of the element, which is not actually an attribute.
	* @param {string} value New value to put in the attribute
	*/
	updateElementFromWASM(elId, attribute, value)
	{
		const elIdStr = this.CstrToJS(elId);
		const attributeStr = this.CstrToJS(attribute);
		const valueStr = this.CstrToJS(value);

		const el = document.getElementById(elIdStr);
		if (el)
		{
			if (attributeStr === "innerHTML")
			{
				// innerHTML is not actually an attribute, so we set it directly instead of with setAttibute.
				el.innerHTML = valueStr;
			}
			else
			{
				el.setAttribute(attributeStr, valueStr);
			}
		}
	}

	/**
	* Like logFromWASM, but for hexadecimal integers.
	* Also should not need to be called from JavaScript code.
	*/
	logHexIntFromWASM(val)
	{
		console.log("0x" + val.toString(16));
	}

	/**
	* This function sets up the parameters needed to be passed to a WASM function and then calls the function.
	* For string parameters, it handles all allocation, copying and freeing of memory necessary to call WASM.
	* For number parameters, it passes them directly.
	* Other parameter types, such as objects or arrays, are not currently supported.
	* @param {function} functionRef An instance of the WASM function to call. This should be a function in the exports object.
	* @param {Array<string|number>} parameterArray An array of parameters that functionRef expects. Only string and integer parameters are supported right now.
	* @returns The value returned from the function given by functionRef. This is always an int.
	*/
	callWASM(functionRef, parameterArray)
	{
		let params = new Array(parameterArray.length);

		// Allocate memory for any string parameters and copy the strings to the allocated WASM memory space.
		for (let i = 0; i < parameterArray.length; i++)
		{
			if (typeof parameterArray[i] === "string")
			{
				params[i] = this.exports.WASMUtils_wmalloc(parameterArray[i].length + 1);
				this.copyStrToBuffer(params[i], parameterArray[i]);
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
				this.exports.WASMUtils_wfree(params[i]);
			}
		}

		// This is always just an int, although it could be a pointer.
		return result;
	}
}
