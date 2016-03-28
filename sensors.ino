#include <DHT.h>

DHT dhtSensor(DHT_PIN, DHT11);

void setupSensors()
{
  dhtSensor.begin();
}

void listenSensors()
{
  temperature = dhtSensor.readTemperature();
  humidity = dhtSensor.readHumidity();
  
  Serial.print("Air temperature: ");
  Serial.println(temperature);
  Serial.print("Air humidity: ");
  Serial.println(humidity);
}
