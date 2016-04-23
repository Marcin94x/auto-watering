#include <ESP8266.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

const byte LEVEL_PIN = A0;
const byte DHT_PIN = 9;
const byte SOIL_PIN = 10;
const byte PUMP_PIN = 11;

ESP8266 wifi(Serial3, 115200);
DHT dhtSensor(DHT_PIN, DHT11);
Adafruit_TSL2561_Unified tsl_sensor = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

extern uint8_t mux_id;
bool is_pump_on = false;
unsigned int start_time = 0; // start time of pump
unsigned int current_time = 0;
const unsigned long line_time = 30 * 1000;

void setup() {
  Serial.begin(9600);
  setupServer();
  setupSensors();
  setupDebugSensors();
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);
}

void loop() {
  listenClients();
  debugSensors();
  watchdog();
}

int getTemperature() {
  char buffer[10];
  sprintf(buffer, "t:%d", 33);
  Serial.print("buffer: ");
  Serial.println(buffer);
  const uint8_t *temp = (const uint8_t *) buffer;
  return wifi.send(mux_id, temp, strlen(buffer));
}

int getAirHumidity() {
  char buffer[10];
  sprintf(buffer, "%d", dhtSensor.readHumidity());
  const uint8_t *humid = (const uint8_t *) buffer;
  return wifi.send(mux_id, humid, strlen(buffer));
}

int getSoilHumidity() {
  char buffer[10];
  int humidity = analogRead(SOIL_PIN) / 1024.0 * 100.0;
  sprintf(buffer, "%d", humidity);
  const uint8_t *humid = (const uint8_t *) buffer;
  return wifi.send(mux_id, humid, strlen(buffer));
}

int getInsolation() {
  char buffer[10];
  sensors_event_t event;
  tsl_sensor.getEvent(&event);
  sprintf(buffer, "%d", event.light);
  const uint8_t *insol = (const uint8_t *) buffer;
  return wifi.send(mux_id, insol, strlen(buffer));
}

int getLiquidLevel() {
  char buffer[10];
  int liquid_level = analogRead(LEVEL_PIN) / 1024.0 * 100.0;
  sprintf(buffer, "%d", liquid_level);
  const uint8_t *lev = (const uint8_t *) buffer;
  return wifi.send(mux_id, lev, strlen(buffer));
}

int setPump() {
  is_pump_on = true;
  start_time = millis();
  digitalWrite(PUMP_PIN, LOW);
  return 1;
}

int setPumpOff() {
  is_pump_on = false;
  digitalWrite(PUMP_PIN, HIGH);
  return 1;
}

void watchdog() {
  current_time = millis();
  if (is_pump_on && (current_time - start_time) > line_time) {
    setPumpOff();
    Serial.println("The pump is turned off - ALARM!!!");
  } 
}
