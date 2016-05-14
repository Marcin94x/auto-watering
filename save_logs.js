var net = require("net");
var fs = require("fs");
var najax = require('najax');

const log = '/var/www/application/logs/sensors.txt';
const SEC = 1000;
const MIN = 60 * SEC;

var webUrl = 'http://localhost:8080/index.php/';
var system = {
    arduinoHost: '192.168.169.5',
    arduinoPort: '8090'
};
var reading = {
	date: '',
	temperature: '',
	airHumidity: '',
	soilHumidity: '',
	insolation: '',
	liquidLevel: ''
};

setInterval(function() {
    najax({
        dataType: 'json',
        url: webUrl + 'watering/getstatus',
        success: function(data) {
            system.arduinoHost = data.arduino_ip;
            system.arduinoPort = data.arduino_port;
        }
    });
}, 10 * SEC);
setInterval(function() {
	setTimeout(getTemperature, 1 * MIN);
	setTimeout(getAirHumidity, 2 * MIN);
	setTimeout(getSoilHumidity, 3 * MIN);
	setTimeout(getInsolation,  4 * MIN);
	setTimeout(getLiquidLevel, 5 * MIN);
	setTimeout(writeToFile, 6 * MIN, log);
	setTimeout(writeToDb, 7 * MIN);
}, 10 * MIN);
	
function getTemperature() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('gettemp\n');
	});
	socket.on('data', function(data) {
  		reading.temperature = data;
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}
	
function getAirHumidity() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('getairhumid\n');
	});
	socket.on('data', function(data) {
  		reading.airHumidity = data;
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}

function getSoilHumidity() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('getsoilhumid\n');
	});
	socket.on('data', function(data) {
  		reading.soilHumidity = data;
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}

function getInsolation() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('getinsol\n');
	});
	socket.on('data', function(data) {
  		reading.insolation = data;
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}

function getLiquidLevel() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('getliquidlvl\n');
	});
	socket.on('data', function(data) {
  		reading.liquidLevel = data;
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}

function writeToFile(file) {
	var d = new Date();
	reading.date = d.getFullYear() + '-' + d.getMonth() + '-' + d.getDate();
	reading.date += ' ';
	reading.date += d.getHours() + ':' + d.getMinutes() + ':' + d.getSeconds();
	fs.appendFile(file, '\n', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.date + ';', function(err) {if (err) throw err});
	fs.appendFile(file, reading.temperature + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.airHumidity + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.soilHumidity + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.insolation + ';', function(err) {if (err) throw err;});
	fs.appendFile(file, reading.liquidLevel + ';', function(err) {if (err) throw err;});
}

function writeToDb() {
	najax({
        dataType: 'json',
        url: webUrl + 'watering/savedata',
        method: 'POST',
        data: {
        	temperature: parseInt(reading.temperature), 
 			air_humidity: parseInt(reading.airHumidity),
 			soil_humidity: parseInt(reading.soilHumidity),
 			insolation: parseInt(reading.insolation),
 			water_level: parseInt(reading.liquidLevel)
        }
    });
}