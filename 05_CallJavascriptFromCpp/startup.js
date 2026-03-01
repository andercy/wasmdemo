window.onload = async () =>
{
	let el = document.getElementById("output");

	// Passing this object to instantiateStreaming tells it to make a function named consolelog available for C/C++ code to import.
	const importObject = { env: { consolelog: (str, len) => logFromWASM(instance.exports.memory.buffer, str, len)}};

	const response = fetch("Library.wasm");
	const { instance } = await WebAssembly.instantiateStreaming(response, importObject);

	appendInnerHTML(el, "Sum of 24 and 3: " + instance.exports.Utils_add(24, 3) + "<br />");

	instance.exports.main();
};

// Named function referred to by importObject above
// Note that the buffer must be passed to it to make it visible, since the named function is not part of the closure in importObject.
function logFromWASM(buffer, str, len)
{
		const bytes = new Uint8Array(buffer, str, len);
		const text = new TextDecoder("utf8").decode(bytes);
		console.log(text);
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
