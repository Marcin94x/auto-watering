var net = require("net");
var fs = require("fs");

var HOST = '192.168.169.4';
var PORT = '8090'
var socket = new net.Socket();
var log = 'log.txt';
var date = '';
	
function getTemperature() {
	socket.connect (PORT, HOST, function() {
		console.log('CONNECTED TO: ' + HOST + ':' + PORT);
  		socket.write('gettemp');
	});
	socket.on('data', function(data) {
  		console.log('DATA: ' + data);
  		return data;
	});
}
	
function getAirHumidity() {
	socket.connect (PORT, HOST, function() {
  		socket.write('getairhumid');
	});
	socket.on('data', function(data) {
  		return data;
	});
}

function getSoilHumidity() {
	socket.connect (PORT, HOST, function() {
  		socket.write('getsoilhumid');
	});
	socket.on('data', function(data) {
  		return data;
	});
}

function getInsolation() {
	socket.connect (PORT, HOST, function() {
  		socket.write('getinsol');
	});
	socket.on('data', function(data) {
  		return data;
	});
}

function getLiquidLevel() {
	socket.connect (PORT, HOST, function() {
  		socket.write('getliquidlvl');
	});
	socket.on('data', function(data) {
  		return data;
	});
}

fs.writeFile(log, '', function(){});
setInterval(function() {
	var d = new Date();
	date = d.getFullYear() + '-' + d.getMonth() + '-' + d.getDate();
	date += ' ';
	date += d.getHours() + ':' + d.getMinutes() + ':' + d.getSeconds();
	fs.appendFile(log, date + ';', function(err) {if (err) throw err});
	fs.appendFile(log, getTemperature() + ';', function(err) {if (err) throw err;});
	fs.appendFile(log, getAirHumidity() + ';', function(err) {if (err) throw err;});
	fs.appendFile(log, getSoilHumidity() + ';', function(err) {if (err) throw err;});
	fs.appendFile(log, getInsolation() + ';', function(err) {if (err) throw err;});
	fs.appendFile(log, getLiquidLevel() + ';', function(err) {if (err) throw err;});
	fs.appendFile(log, '\n', function(err) {if (err) throw err;});
}, 1 * 60 * 1000);