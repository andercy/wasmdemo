window.onload = async () =>
{
	let el = document.getElementById("output");

	// This creates a block of memory that WASM can use.
	// The size is given in 64KB blocks, so this size of 4 gives us 256KB of memory.
	let memory = new WebAssembly.Memory({initial: 4});

	// For this demo, we add the memory object to importObject.
	// When we link with the flag --import-memory, this causes the WASM module to use this JavaScript allocated block for its memory, instead of creating its own.
	// By default, it will just create its own memory (most likely 2 blocks, 128KB) and expose it through instance.exports.memory.
	const importObject = { env:
			{
				consolelog: (str, len) => logFromWASM(memory.buffer, str, len),
				consolelogHexInt: (val) => logHexIntFromWASM(val),
				memory: memory
			}};

	const response = fetch("Library.wasm");
	const { instance } = await WebAssembly.instantiateStreaming(response, importObject);

	appendInnerHTML(el, "Sum of 24 and 3: " + instance.exports.Utils_add(24, 3) + "<br />");

	instance.exports.main();
};

function logFromWASM(buffer, str, len)
{
		const bytes = new Uint8Array(buffer, str, len);
		const text = new TextDecoder("utf8").decode(bytes);
		console.log(text);
}

// We added this function to easily display addresses in the typical hex format.
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
