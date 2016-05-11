var net = require("net");

const SEC = 1000;
const keepOn = 5 * SEC;
const keepOff = 30 * SEC;

var system = {
    holdHumidity: 40,
    startTime: 0,
    stopTime: 0,
    soilHumidity: '',
    isRunning: false
};

setInterval(getSoilHumidity, 30 * SEC);
setInterval(function() {
    $.ajax({
        dataType: 'json',
        url: '',
        success: function(data) {
            system.isRunning = data;
        }
    });
    if (isRunning && system.soilHumidity < (system.holdHumidity - 10)) {
        setPumpOn();
	    while(Date.now() - system.startTime < keepOn);
        setPumpOff();
	    while(Date.now() - system.stopTime < keepOff);
    }
}, 1 * SEC);

function getSoilHumidity() {
	var socket = new net.Socket();
	socket.connect (PORT, HOST, function() {
  		socket.write('getsoilhumid\n');
	});
	socket.on('data', function(data) {
		console.log('SOIL HUMID: ' + data);
  		system.soilHumidity = data;
	});
	socket.on('error', function(err) {
	    console.log(err);
	    socket.destroy();
	});
}

function setPumpOn() {
	var socket = new net.Socket();
	socket.connect (PORT, HOST, function() {
  		socket.write('setpump\n');
        system.start = Date.now();
	});
	socket.on('error', function(err) {
	    console.log(err);
	    socket.destroy();
	});
}

function setPumpOff() {
	var socket = new net.Socket();
	socket.connect (PORT, HOST, function() {
  		socket.write('setpumpoff\n');
        system.stop = Date.now();
	});
	socket.on('error', function(err) {
	    console.log(err);
	    socket.destroy();
	});
}
