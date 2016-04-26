var net = require("net");
var fs = require("fs");

const HOST = '192.168.169.8';
const PORT = '8090';
const log = 'log.txt';
const MIN = 60 * 1000;

var reading = {
	date: '',
	temperature: '',
	airHumidity: '',
	soilHumidity: '',
	insolation: '',
	liquidLevel: ''
};
	
function getTemperature() {
	var socket = new net.Socket();
	socket.connect(PORT, HOST, function() {
  		socket.write('gettemp\n');
	});
	socket.on('data', function(data) {
  		console.log('TEMP: ' + data);
  		reading.temperature = data;
	});
	socket.on('error', function(err) {
	    console.log(err);
	    socket.destroy();
	});
}
	
function getAirHumidity() {
	var socket = new net.Socket();
	socket.connect(PORT, HOST, function() {
  		socket.write('getairhumid\n');
	});
	socket.on('data', function(data) {
		console.log('AIR HUMID: ' + data);
  		reading.airHumidity = data;
	});
	socket.on('error', function(err) {
	    console.log(err);
	    socket.destroy();
	});
}

function getSoilHumidity() {
	var socket = new net.Socket();
	socket.connect (PORT, HOST, function() {
  		socket.write('getsoilhumid\n');
	});
	socket.on('data', function(data) {
		console.log('SOIL HUMID: ' + data);
  		reading.soilHumidity = data;
	});
	socket.on('error', function(err) {
	    console.log(err);
	    socket.destroy();
	});
}

function getInsolation() {
	var socket = new net.Socket();
	socket.connect (PORT, HOST, function() {
  		socket.write('getinsol\n');
	});
	socket.on('data', function(data) {
		console.log('INSOLATION: ' + data);
  		reading.insolation = data;
	});
	socket.on('error', function(err) {
	    console.log(err);
	    socket.destroy();
	});
}

function getLiquidLevel() {
	var socket = new net.Socket();
	socket.connect (PORT, HOST, function() {
  		socket.write('getliquidlvl\n');
	});
	socket.on('data', function(data) {
		console.log('WATER LEVEL: ' + data);
  		reading.liquidLevel = data;
	});
	socket.on('error', function(err) {
	    console.log(err);
	});
}

function writeToFile(file) {
	var d = new Date();
	reading.date = d.getFullYear() + '-' + d.getMonth() + '-' + d.getDate();
	reading.date += ' ';
	reading.date += d.getHours() + ':' + d.getMinutes() + ':' + d.getSeconds();
	fs.appendFile(file, reading.date + ';', function(err) {if (err) throw err});
	fs.appendFile(file, reading.temperature + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.airHumidity + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.soilHumidity + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.insolation + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.liquidLevel + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, '\n', function(err) {if (err) throw err;});
}

fs.writeFile(log, '', function(err) {if (err) throw err});
setInterval(function() {
	setTimeout(getTemperature, 30 * 1000);
	setTimeout(getAirHumidity, MIN);
	setTimeout(getSoilHumidity, MIN + 30 * 1000);
	setTimeout(getInsolation,  2 * MIN);
	setTimeout(getLiquidLevel, 2 * MIN + 30 * 1000);
	setTimeout(writeToFile(log), 3 * MIN);
}, 4 * MIN);