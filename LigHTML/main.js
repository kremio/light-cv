const com = document.getElementById('com')

let period = 5000 //in ms
let start = undefined

let strobe = false

function set( level ){
	com.style.backgroundColor = `rgb(${level},${level},${level})`
}

function animation(timestamp){
	if( !start ){
		start = timestamp
	}
	
	const t = (timestamp - start) % period

	const sin = 127 + Math.round( Math.sin( ( t/period )*Math.PI*2 ) * 127 )
	
	set( sin )

	if(strobe){
		window.requestAnimationFrame( animation )
	}else{
		start = undefined
	}
}

/*
 * A logarithmic scale
 */
function equalTemperament12(start, end){
	const twelthRoot = Math.pow(2,1/12)
	const pitches = [start]
	let i = 0
	while(pitches[i] * twelthRoot < end){
		pitches.push( pitches[i++] * twelthRoot )
	}
	return pitches.map( (p) => Math.round(p) )
}

/*
 * A linear scale
 */
function linearSteps(start, end, step){
	const pitches = [start]
	while(start + pitches.length * step < end){
		pitches.push( start + pitches.length * step )
	}
	return pitches
}

let mouseDown = false

document.addEventListener('mousedown', (e) => {
	mouseDown = true
	console.log('mousedown')
})

document.addEventListener('mouseup', (e) => {
	mouseDown = false
  set( 0 )
	console.log('mouseup')
})


const STEP = 5
const C4 = 80
const C5 = 140
function keyboard(){
	const notes = "csdsefsgsasb"
	const keyboardDOM = document.getElementById('keyboard')
	keyboardDOM.innerHTML = linearSteps(20, 255, STEP)//equalTemperament12(10, 255)
		.reduce( ( HTML, pitch, i) => {
			const note = notes[ i % notes.length ]
			return HTML + `<div class="${note}" data-pitch="${pitch}"></div>`
		}, "")

	keyboardDOM.addEventListener('mousemove', (e) => {
		if(!mouseDown){ return }
		set( e.target.getAttribute('data-pitch') )
	})

	keyboardDOM.addEventListener('mousedown', (e) => {
		set( e.target.getAttribute('data-pitch') )
	})
}

keyboard()

document.getElementById('strobe-btn').addEventListener('click',(e) =>{
	strobe = !strobe
	if( strobe ){
		window.requestAnimationFrame( animation )
	}
})

document.getElementById('period-ctrl').addEventListener('change',(e) =>{
	period = e.target.value
	console.log(period)
})

document.getElementById('value-ctrl').addEventListener('mousemove',(e) =>{
	set( Number(e.target.value) )
})

function calibrationPulse(val, duration = 1100){
	return new Promise((s,f) => {
		set(val)
		setTimeout( s, duration)
	})
}

document.getElementById('calibration-btn').addEventListener('click', (e) =>{
	calibrationPulse(0)
	.then( () => calibrationPulse(255) )
	.then( () => calibrationPulse(C4) )
  .then( () => calibrationPulse(C5) )
  .then( () => set(0) )


	/*
	calibrationPulse(255)
	.then( () => calibrationPulse(0) )
	.then( () => calibrationPulse(255) )
	.then( () => calibrationPulse(C4) )
	.then( () => calibrationPulse(C4 + STEP) )
	.then( () => set(0) )
	*/
})

const stepsSelect = document.getElementById('steps-ctrl')

const arr = [];
for(let i = 1; i <= 255; i++){
	arr.push(i)
}
arr.map( (s) => s)
.filter( (s) => !(""+ (255 / s)).includes('.') )
.map( (s) => {
	const opt = document.createElement('option')
	opt.value = s
	opt.innerHTML = s
	return opt
}).forEach( (opt) => {
	stepsSelect.appendChild(opt)
})

const stepsDurationRange = document.getElementById('steps-duration-ctrl')
const stepsDuration = document.getElementById('steps-duration-label')

function showStepDuration(){
	stepsDuration.innerHTML =  stepsDurationRange.value
}
	

stepsDurationRange.addEventListener('mousemove',showStepDuration )
showStepDuration()


function sequence( fromVal, valIncrease, time ){
	if(fromVal == 255){
		set( 0 ) //done
		return
	}

	set( fromVal + valIncrease )
	setTimeout( () => sequence( fromVal + valIncrease, valIncrease, time ), time )
}

document.getElementById('sequence-btn').addEventListener('click', (e) =>{
	sequence( 0, 255 / Number(stepsSelect.value), stepsDurationRange.value )
})
