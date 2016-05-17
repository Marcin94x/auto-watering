Auto Watering System

The system provide us an intelligent algorithm to watering plants.

Requirements:
An Arduino compatible board.
Beaglebone board.

Description:
In Arduino folder, there's code for this board. It setups all necessary to correct work sensors. It makes the device to be a TCP server. The Arduino mainly response on TCP requests. There's also implemented a watchdog against flood. For security reason the pump can't work longer than 30 sec.

TCP request     response

gettemp         air temperature in Celsius
getairhumid     air humidity in percent
getsoilhumid	soil humidity in percent
getinsol        illuminance in lux
getliquidlvl	water level in percent
setpump         turns the pump on
setpumpoff      turns the pump off
