browser.storage.onChanged.addListener( (changes, area) => {
    if(area==='local' && 'edit' in changes) {
	select();
    }
});

async function select() {   
    document.body.addEventListener('click', e => {

	forward(e.target.src).then(answer => {

	    alert(answer);

	});
	
    }, {once: true});
}

async function forward(url) {
    
    let t = await (fetch('http://localhost:8020', {
	headers: {
	    'Url': url
	}
    }).then(resp => resp.text()));
    
    return t;
}
