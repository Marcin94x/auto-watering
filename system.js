var net = require("net");
var najax = require('najax');

const SEC = 1000;
const keepOn = 5 * SEC;
const keepOff = 45 * SEC;

var webUrl = 'http://localhost:8080/index.php/';
var system = {
    holdHumidity: 0,
    soilHumidity: 0,
    isRunning: 0,
    arduinoHost: '192.168.169.5',
    arduinoPort: '8090'
};

setInterval(getSoilHumidity, 60 * SEC);
setInterval(function() {
    najax({
        dataType: 'json',
        url: webUrl + 'watering/getstatus',
        success: function(data) {
            system.isRunning = parseInt(data.isOn);
            system.holdHumidity = parseInt(data.humidity);
            system.arduinoHost = data.arduino_ip;
            system.arduinoPort = data.arduino_port;
        }
    });
}, 5 * SEC);
setInterval(function() {
	if (system.isRunning && system.soilHumidity < (system.holdHumidity - 10)) {
        setPumpOn();
	    setTimeout(setPumpOff, keepOn);
    }
}, keepOff);

function getSoilHumidity() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('getsoilhumid\n');
	});
	socket.on('data', function(data) {
		data = parseInt(data);
  		system.soilHumidity = data;
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}

function setPumpOn() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('setpump\n');
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}

function setPumpOff() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('setpumpoff\n');
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}