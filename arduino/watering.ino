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
DHT dhtSensor(DHT_PIN, DHT11); /**< a DHT11 sensor */
Adafruit_TSL2561_Unified tsl_sensor = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345); /**< a TSL2561 light sensor */

extern uint8_t mux_id; /**< @see mux_id in server.ino */
bool is_pump_on = false; /**< checks if the pump is on */
unsigned int start_time = 0; /**< start time of the pump */
unsigned int current_time = 0; /**< time of working system in ms */
const unsigned long line_time = 30 * 1000; /**< maximal time of continuous pump action */

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
 * Reads air temperature from the DHT11 sensor and sends temperature in Celsius degree.
 * @return The status of sending an information.
 */
int getTemperature() {
  char buffer[4];
  sprintf(buffer, "%d", (byte) dhtSensor.readTemperature());
  const uint8_t *temp = (const uint8_t *) buffer;
  return wifi.send(mux_id, temp, strlen(buffer));
}

/**
 * Reads air humidity from the DHT11 sensor and sends humidity in percents.
 * @return The status of sending an information.
 */
int getAirHumidity() {
  char buffer[4];
  sprintf(buffer, "%d", (byte) dhtSensor.readHumidity());
  const uint8_t *humid = (const uint8_t *) buffer;
  return wifi.send(mux_id, humid, strlen(buffer));
}


/**
 * Reads soil humidity from the sensor and sends humidity in percents.
 * @return The status of sending an information.
 */
int getSoilHumidity() {
  char buffer[4];
  byte humidity = analogRead(SOIL_PIN) / 1024.0 * 100.0;
  sprintf(buffer, "%d", humidity);
  const uint8_t *humid = (const uint8_t *) buffer;
  return wifi.send(mux_id, humid, strlen(buffer));
}

/**
 * Reads insolation from the TSL sensor and sends insolation in lux.
 * @return The status of sending an information.
 */
int getInsolation() {
  char buffer[10];
  sensors_event_t event;
  tsl_sensor.getEvent(&event);
  sprintf(buffer, "%d", (byte) event.light);
  const uint8_t *insol = (const uint8_t *) buffer;
  return wifi.send(mux_id, insol, strlen(buffer));
}

/**
 * Reads insolation from the TSL sensor and sends insolation in lux.
 * @return The status of sending an information.
 */
int getLiquidLevel() {
  char buffer[4];
  byte liquid_level = analogRead(LEVEL_PIN) / 1024.0 * 100.0;
  sprintf(buffer, "%d", liquid_level);
  const uint8_t *lvl = (const uint8_t *) buffer;
  return wifi.send(mux_id, lvl, strlen(buffer));
}

/**
 * Turns the water pump on.
 * @return The status of sending an information.
 */
int setPump() {
  is_pump_on = true;
  start_time = millis();
  digitalWrite(PUMP_PIN, LOW);
  return 1;
}

/**
 * Turns the water pump off.
 * @return The status of sending an information.
 */
int setPumpOff() {
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
