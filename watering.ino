#include <ESP8266.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

const byte LEVEL_PIN = A0;
const byte DHT_PIN = 9;
const byte SOIL_PIN = 10;
const byte PUMP_PIN = 11;

ESP8266 client(Serial1);
DHT dhtSensor(DHT_PIN, DHT11);
Adafruit_TSL2561_Unified tsl_sensor = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
bool is_pump_on = false;

char buffer[10];
unsigned int start_time = 0; // start time of pump
unsigned int current_time = 0;
const unsigned long line_time = 30 * 1000;

void setup() {
  Serial.begin(9600);
  //setupServer();
  setupSensors();
  setupDebugSensors();
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);
}

void loop() {
  //listenClients();
  parseRequest();
  debugSensors();
  watchdog();
}

int getTemperature() {
  sprintf(buffer, "%d", dhtSensor.readTemperature());
  const uint8_t *temp = (const uint8_t *) buffer;
  return client.send(temp, strlen(buffer));
}

int getAirHumidity() {
  sprintf(buffer, "%d", dhtSensor.readHumidity());
  const uint8_t *humid = (const uint8_t *) buffer;
  return client.send(humid, strlen(buffer));
}

int getSoilHumidity() {
  int humidity = analogRead(SOIL_PIN) / 1024.0 * 100.0;
  sprintf(buffer, "%d", humidity);
  const uint8_t *humid = (const uint8_t *) buffer;
  return client.send(humid, strlen(buffer));
}

int getInsolation() {
  sensors_event_t event;
  tsl_sensor.getEvent(&event);
  sprintf(buffer, "%d", event.light);
  const uint8_t *insol = (const uint8_t *) buffer;
  return client.send(insol, strlen(buffer));
}

int getLiquidLevel() {
  int liquid_level = analogRead(LEVEL_PIN) / 1024.0 * 100.0;
  sprintf(buffer, "%d", liquid_level);
  const uint8_t *lev = (const uint8_t *) buffer;
  return client.send(lev, strlen(buffer));
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
