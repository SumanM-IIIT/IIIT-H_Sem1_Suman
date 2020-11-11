$('nav li').hover(
	function() {
		$('ul', this).stop().slideDown(200);
	},
	function() {
		$('ul', this).stop().slideUp(200);
	}
);

/*function count_visitors(){
	var counter = $('#counter span').text(); // geting value from span
	var count = 0;
	count = parseInt(counter.value);
	count = count+1;
	counter.innerHTML = parseInt(count);
}
window.onload = count_visitors;*/

var canvas = document.getElementById("canvas");
var context = canvas.getContext("2d");
var radius = canvas.height / 2;
context.translate(radius, radius);
radius = radius * 0.9;

//analog_clock();
setInterval(analog_clock, 1000);

function analog_clock() {
	inside_clock(context, radius);
	hour_numbers(context, radius);
	calc_time(context, radius);
}

function inside_clock(context, radius) {
	var grad;
	context.beginPath();
	context.arc(0, 0, radius, 0, 2 * Math.PI);
	var temp_gradient = context.createRadialGradient(0, 0, radius/2, 0, 0, radius);
	temp_gradient.addColorStop(0, "#fdcc0d");
	temp_gradient.addColorStop(1, "black");
	context.fillStyle = temp_gradient;
	context.fill();
	grad = context.createRadialGradient(0, 0, radius * 0.95, 0, 0, radius * 1.05);
	grad.addColorStop(0, '#333');
	grad.addColorStop(0.5, '#a62c2b');
	grad.addColorStop(1, '#333');
	context.strokeStyle = grad;
	context.lineWidth = radius * 0.1;
	context.stroke();
	context.beginPath();
	context.arc(0, 0, radius * 0.1, 0, 2 * Math.PI);
	context.fillStyle = '#333';
	context.fill();
}
function hour_numbers(context, radius) {
	var ang;
	var num;
	context.font = "14px verdana";
	context.fillStyle = "white";
	context.textBaseline = "middle";
	context.textAlign = "center";
	for(num = 1; num < 13; num++) {
		ang = num * Math.PI / 6;
		context.rotate(ang);
		context.translate(0, -radius * 0.85);
		context.rotate(-ang);
		context.fillText(num.toString(), 0, 0);
		context.rotate(ang);
		context.translate(0, radius * 0.85);
		context.rotate(-ang);
	}
}
function calc_time(context, radius) {
	var now = new Date();
	var hour = now.getHours();
	var minute = now.getMinutes();
	var second = now.getSeconds();
	hour = hour % 12;
	hour = (hour * Math.PI / 6) + (minute * Math.PI / (6 * 60)) + (second * Math.PI / (360 * 60));
	time_hands(context, hour, radius * 0.5, radius * 0.05, "black");
	minute = (minute * Math.PI / 30) + (second * Math.PI / (30 * 60));
	time_hands(context, minute, radius * 0.8, radius * 0.05, "#033f63");
	second = (second * Math.PI / 30);
	time_hands(context, second, radius * 0.9, radius * 0.02, "#a62c2b");
}
function time_hands(context, position, length, width, color) {
	context.beginPath();
	context.strokeStyle = color;
	context.lineWidth = width;
	context.lineCap = "round";
	context.moveTo(0,0);
	context.rotate(position);
	context.lineTo(0, -length);
	context.stroke();
	context.rotate(-position);
}

