browser.storage.onChanged.addListener( (changes, area) => {
	if(area==='local' && 'edit' in changes) {
		parse();
	}
});

async function parse() {

	const urls = [];

	[...document.images].forEach(image => {
		urls.push(image.src)
	});

	console.log(urls[7])

	let t = fetch('http://localhost:8020', {
		headers: {
			'Url': urls[7]
		}
	}).then(resp => resp.text())
		.then(data => console.log(data));


}
