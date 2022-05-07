let button = document.getElementById('goCalc');

button.addEventListener('click', e => {
	let p = click();
})

async function click() {
	let edit = 1;
	await browser.storage.local.set({edit});
	return 0 ; 
}

click().catch(e => console.error(e));