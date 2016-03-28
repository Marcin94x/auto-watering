const byte DHT_PIN = 9;

byte temperature;
byte humidity;

void setup()
{
  Serial.begin(9600);
  setupServer();
  setupSensors();
}

void loop()
{
  listenClients();
  listenSensors();
  parseRequest();
  sendLogs();
}
