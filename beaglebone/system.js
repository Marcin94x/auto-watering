var net = require("net");
var najax = require('najax');

const SEC = 1000; /**< 1 s == 1,000 ms */
const keepOn = 5 * SEC; /**< time of watering */
const keepOff = 45 * SEC; /**< time of break after watering */
const webUrl = 'http://localhost:8080/index.php/'; /**< address of web application */

var system = {
    holdHumidity: 0, /**< hold soil humidity aprox. */
    soilHumidity: 0, /**< current soil humidity */
    isRunning: 0, /**< is auto watering turned on */
    arduinoHost: '', /**< IP address of Arduino TCP server */
    arduinoPort: '' /**< port of Arduino TCP server */
};

setInterval(getSoilHumidity, 60 * SEC);
setInterval(getData, 5 * SEC);
setInterval(water, keepOff);

/**
 * Fetches current data from the web server.
 */
function getData() {
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
}

/**
 * Starts watering.
 * Watering is started when auto watering is turned on and soil humidity is under requested humidity minus 10 %.
 */
function water() {
    if (system.isRunning && system.soilHumidity < (system.holdHumidity - 10)) {
        setPumpOn();
	setTimeout(setPumpOff, keepOn);
    }
}

/**
 * Fetches current soil humidity.
 */
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

/**
 * Sends a TCP request to set the pump on.
 */
function setPumpOn() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('setpump\n');
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}

/**
 * Sends a TCP request to set the pump off.
 */
function setPumpOff() {
	var socket = new net.Socket();
	socket.connect(system.arduinoPort, system.arduinoHost, function() {
  		socket.write('setpumpoff\n');
	});
	socket.on('error', function(err) {
	    socket.destroy();
	});
}
