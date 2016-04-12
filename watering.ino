#include <ESP8266.h>
#include <DHT.h>
#include <stdio.h>

const byte DHT_PIN = 9;
const byte PUMP_PIN = 11;

ESP8266 client(Serial1);
DHT dhtSensor(DHT_PIN, DHT11);
char buffer[10];
unsigned int start_time = 0; // start time of pump
unsigned int current_time = 0;
const int line_time = 10 * 60 * 1000;

void setup()
{
  Serial.begin(9600);
  setupServer();
  setupSensors();
}

void loop()
{
	listenClients();
	parseRequest();
	if (current_time - start_time > line_time) {
		setPumpOff();
	}
	current_time = millis();
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
	//sprintf(buffer, "%d", dhtSensor.readHumidity());
	const uint8_t *humid = (const uint8_t *) buffer;
	return client.send(humid, strlen(buffer));
}

int getInsolation() {
	//TODO
	const uint8_t *insol = (const uint8_t *) buffer;
	return client.send(insol, strlen(buffer));
}	

int setPump() {
	digitalWrite(PUMP_PIN, HIGH);
	start_time = millis();
	return 1;
}

int setPumpOff() {
	digitalWrite(PUMP_PIN, LOW);
	return 1;
}
