#include <DHT.h>

DHT dhtSensor(DHT_PIN, DHT11);

void setupSensors()
{
  dhtSensor.begin();
}

void listenSensors()
{
  Serial.print("Air temperature: ");
  Serial.println(dhtSensor.readTemperature());
  Serial.print("Air humidity: ");
  Serial.println(dhtSensor.readHumidity());
}
