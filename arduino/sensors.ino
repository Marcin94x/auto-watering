void setupSensors() {
  dhtSensor.begin();
  tsl_sensor.begin();
  tsl_sensor.enableAutoRange(true);
  tsl_sensor.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
}

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

void debugSensors() {
  char choice = Serial.read();
  switch (choice) {
    case 'a' :
      Serial.print("Temperature: ");
      Serial.print(dhtSensor.readTemperature());
      Serial.println(" degree C");
      break;
    case 'b' :
      Serial.print("Air humidity: ");
      Serial.print(dhtSensor.readHumidity());
      Serial.println(" %");
      break;
    case 'c' :
      Serial.print("Soil humidity: ");
      Serial.print(analogRead(SOIL_PIN) / 1024.0 * 100.0);
      Serial.println(" %");
      break;
    case 'd' :
      Serial.print("Insolation: ");
      sensors_event_t event;
      tsl_sensor.getEvent(&event);
      Serial.print(event.light);
      Serial.println(" lux");
      break;
    case 'e' :
      Serial.print("Liquid level: ");
      Serial.print(analogRead(LEVEL_PIN) / 1024.0 * 100.0);
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
