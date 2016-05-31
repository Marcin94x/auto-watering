Auto Watering System
====================

The system provides us an intelligent algorithm to watering plants.
------------------------------------------------------------------

### Requirements:

* An Arduino compatible board.
* DHT11 sensor.
* TSL2561 sensor.
* ESP8266 wifi module.
* An analog soil humidity sensor.
* An analog liquid level sensor.
* A relay.
* A pump.
* A Beaglebone board.

#### Libraries for Arduino:

* Adafruit TSL2561 https://github.com/adafruit/Adafruit_TSL2561
* DHT sensor library https://github.com/adafruit/DHT-sensor-library
* ESP8266 https://github.com/itead/ITEADLIB_Arduino_WeeESP8266

### Description:

#### Arduino:
In Arduino folder, there's C/C++ code for this board. It setups all necessary to correct work sensors. It makes the device to be a TCP server. The Arduino mainly response on TCP requests. There's also implemented a watchdog against flood. For security reason, the pump can't work longer than 30 sec.

TCP request -------------------> response

* gettemp ---------------------> air temperature in Celsius
* getairhumid -----------------> air humidity in percent
* getsoilhumid ----------------> soil humidity in percent
* getinsol --------------------> illuminance in lux
* getliquidlvl ----------------> water level in percent
* setpump ---------------------> turns the pump on
* setpumpoff ------------------> turns the pump off

#### Beaglebone:
Beaglebone folder contains two scripts for this micro computer.
System.js is responsible for sending get humidity requests to have current humidity every half minute. The script also sends HTTP POST requests to fetch information from a database. After grabbing data, the algorithm analyzes it and stars or stops watering. Watering takes 5 sec because of very strong pump and then there's 45 sec break.

#### Web application:
More info in www folder.
