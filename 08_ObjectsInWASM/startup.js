window.onload = async () =>
{
	const wasm = await loadWASM("Library.wasm");

	let el = document.getElementById("output");


	wasm.exports.main();

	// Create a new Person object in WASM memory.
	// This demonstrates the use of the callWASM helper funciton.
	// We could actually simplify this even more by not creating parameterArray and just passing the array directly to callWASM.
	let parameterArray = ["James", "Kirk", 42, 178, 77000];
	let personPtr = callWASM(wasm, wasm.exports.Person_NewPerson, parameterArray);

	// Display the Person in the output element.
	// Here we do not create a parameter array, and instead pass it directly to callWASM.
	// Notice how we immediately free the memory for fullNamePtr, since it is no longer needed once the JavaScript string is created.
	let fullNamePtr = callWASM(wasm, wasm.exports.Person_getFullName, [personPtr]);
	let fullName = CstrToJS(wasm.memory, fullNamePtr);
	wasm.exports.Utils_wfree(fullNamePtr);

	// Note how we do not free the pointers returned for firstName and lastName.
	// Those functions start with getRef, so the Person object still controls them, so we must not free them. (See Rule 7)
	// Note also how most of these functions do not need callWASM, since they do not pass strings.
	// They are just using callWASM to demonstrate the function.
	let firstNamePtr = callWASM(wasm, wasm.exports.Person_getRefFirstName, [personPtr]);
	let firstName = CstrToJS(wasm.memory, firstNamePtr);

	let lastNamePtr = callWASM(wasm, wasm.exports.Person_getRefLastName, [personPtr]);
	let lastName = CstrToJS(wasm.memory, lastNamePtr);

	// Notice how no special handling is needed for functions returning ints.
	// They are simple enough to just use the result inline.
	// They are so simple in fact, that we do not actually need to use callWASM, as demonstrated by the weight line.
	let userDisplay = "<table><tr><td colspan=\"2\" align=\"center\"><b>" + fullName + "</b></td></tr>" +
			makeRow("First Name", firstName) +
			makeRow("Last Name", lastName) +
			makeRow("Age (years)", callWASM(wasm, wasm.exports.Person_getAgeYears, [personPtr])) +
			makeRow("Height (cm)", callWASM(wasm, wasm.exports.Person_getHeightCM, [personPtr])) +
			makeRow("Weight (grams)", wasm.exports.Person_getWeightG(personPtr)) +
			"</table>";

	el.innerHTML = userDisplay;

	// This will free the memory for firstName and lastName.
	// It will also free the Person object itself, which was allocated with NewPerson above.
	wasm.exports.Person_DeletePerson(personPtr);
};

function makeRow(label, data)
{
	return "<tr><td>" + label + ":</td><td>" + data + "</td></tr>";
}
