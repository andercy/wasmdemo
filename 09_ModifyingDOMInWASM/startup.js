// For this demo, we have converted the WASM utitilies into a class in a module.
import WASMInstance from "./WASMInstance.js"

window.onload = async () =>
{
	// We added this so it automatically stops if DevTools is open.
	debugger;

	// WASM functionality is now all provided through a class in WASMInstance.js, so it is no longer in the global space.
	// To use it, a new instance of the class must be created by passing a file name to the constructor.
	// Then, it must call loadWASM to actually retrieve the code an initialize it.
	// Once this is done, all access to WASM, including memory, exported functions and utility functions, are provided through the instance.
	const wasm = new WASMInstance("Library.wasm");
	await wasm.loadWASM();

	wasm.exports.main();

	// Create a new Person object in WASM memory.
	let personPtr = wasm.callWASM(wasm.exports.Person_newPerson, ["James", "Kirk", 42, 178, 77000]);
	wasm.callWASM(wasm.exports.Person_displayPersonTable, [personPtr, "output1"]);

	// Modify existing person. Note that the existing name strings are freed.
	wasm.callWASM(wasm.exports.Person_setFirstName, [personPtr, "William"]);
	wasm.callWASM(wasm.exports.Person_setLastName, [personPtr, "Shatner"]);
	wasm.exports.Person_setAgeYears(personPtr, 95);

	// Make a completely new Person and display it. Note how it does not affect the first Person.
	let personPtr2 = wasm.callWASM(wasm.exports.Person_newPerson, ["Jean-Luc", "Picard", 85, 178, 82000]);
	wasm.callWASM(wasm.exports.Person_displayPersonTable, [personPtr2, "output2"]);

	// Display the modified first Person. Notice how creating the second Person has no effect on it.
	wasm.callWASM(wasm.exports.Person_displayPersonTable, [personPtr, "output3"]);

	// When deletePerson is called, it frees the name strings and the Person object.
	wasm.exports.Person_deletePerson(personPtr);
	wasm.exports.Person_deletePerson(personPtr2);
};
