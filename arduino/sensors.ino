/**
 * Sets up DHT and TSL sensors.
 */
void setupSensors() {
  dhtSensor.begin();
  tsl_sensor.begin();
  tsl_sensor.enableAutoRange(true);
  tsl_sensor.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
}

/**
 * Sets up debugging by the serial.
 */
void setupDebugSensors() {
  Serial.println("Sensors test:");
  Serial.println("a - read temperature,");
  Serial.println("b - read air humidity,");
  Serial.println("c - read soil humidity,");
  Serial.println("d - read insolation,");
  Serial.println("e - read liquid level,");
  Serial.println("f - turn pump on,");
  Serial.println("g - turn pump off.");
}

/**
 * Listens to serial commends.
 */
void debugSensors() {
  char choice = Serial.read();
  switch (choice) {
    case 'a' :
      Serial.print("Temperature: ");
      Serial.print(getTemperature());
      Serial.println(" degree C");
      break;
    case 'b' :
      Serial.print("Air humidity: ");
      Serial.print(getAirHumidity());
      Serial.println(" %");
      break;
    case 'c' :
      Serial.print("Soil humidity: ");
      Serial.print(getSoilHumidity());
      Serial.println(" %");
      break;
    case 'd' :
      Serial.print("Insolation: ");
      Serial.print(getInsolation());
      Serial.println(" lux");
      break;
    case 'e' :
      Serial.print("Liquid level: ");
      Serial.print(getLiquidLevel());
      Serial.println(" %");
      break;
    case 'f' :
      setPump();
      Serial.println("The pump is turned on");
      break;
    case 'g' :
      setPumpOff();
      Serial.println("The pump is turned off");
      break;
  }
}
