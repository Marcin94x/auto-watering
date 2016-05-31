#include <ESP8266.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

/**
 * Sensors pins.
 */
const byte LEVEL_PIN = A0; /**< water level sensor pin */
const byte DHT_PIN = 9; /**< air humidity and air temperature sensor pin */
const byte SOIL_PIN = 10; /**< soil humidity sensor pin */
const byte PUMP_PIN = 11; /**< relay for the water pump pin */

/**
 * Sensors that use included libraries.
 */ 
ESP8266 wifi(Serial3, 115200); /**< a ESP8266 wifi module connected to Serial 3 */
DHT dhtSensor(DHT_PIN, DHT11); /**< a DHT11 air temperature and humidity sensor */
Adafruit_TSL2561_Unified tsl_sensor = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345); /**< a TSL2561 light sensor */

const unsigned long line_time = 30 * 1000; /**< maximal time of continuous pump action */
bool is_pump_on = false; /**< checks if the pump is on */
unsigned int start_time = 0; /**< start time of the pump */
unsigned int current_time = 0; /**< time of working system in ms */

/**
 * Sets up serial, server, sensors, debugging and the pump.
 */
void setup() {
  Serial.begin(9600); /**< sets the data rate in bits per second for the serial transmission */
  setupServer();
  setupSensors();
  setupDebugSensors();
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH); /**< sets the relay on to keep the pump off */
}

/**
 * Listens to TCP clients, a serial console and keeps the protection against flood in an endless loop.
 */
void loop() {
  listenClients();
  debugSensors();
  watchdog();
}

/**
 * Reads air temperature from the DHT11 sensor and returns temperature in Celsius degree.
 * @return Air temperature in Celsius degree.
 */
byte getTemperature() {
  return dhtSensor.readTemperature();
}

/**
 * Reads air humidity from the DHT11 sensor and returns humidity in percents.
 * @return Air humidity in percents.
 */
byte getAirHumidity() {
  return dhtSensor.readHumidity();
}


/**
 * Reads soil humidity from the sensor and returns humidity in percents.
 * @return Soil humidity in percents.
 */
byte getSoilHumidity() {
  return analogRead(SOIL_PIN) / 1024.0 * 100.0;
}

/**
 * Reads insolation from the TSL sensor and returns insolation in lux.
 * @return Insolation in lux.
 */
byte getInsolation() {
  sensors_event_t event;
  tsl_sensor.getEvent(&event);
  return event.light;
}

/**
 * Reads water level from the sensor and returns liquid level in percents.
 * @return Liquid level in percents.
 */
byte getLiquidLevel() {
  return analogRead(LEVEL_PIN) / 1024.0 * 100.0;
}

/**
 * Turns the water pump on.
 * @return Success.
 */
byte setPump() {
  is_pump_on = true;
  start_time = millis();
  digitalWrite(PUMP_PIN, LOW);
  return 1;
}

/**
 * Turns the water pump off.
 * @return Success.
 */
byte setPumpOff() {
  is_pump_on = false;
  digitalWrite(PUMP_PIN, HIGH);
  return 1;
}

/**
 * Turns the water pump off when its working time is exceeded.
 */
void watchdog() {
  current_time = millis();
  if (is_pump_on && (current_time - start_time) > line_time) {
    setPumpOff();
    Serial.println("The pump is turned off - ALARM!!!");
  } 
}
