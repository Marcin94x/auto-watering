#include <ESP8266.h>

const char SSID[] = "castle";
const char PASS[] = "zamek12345678";
const char HOST_NAME[] = "watering.com";
const byte HOST_PORT = 80;

uint8_t plainRequest[1024];
uint32_t requestLen;
ESP8266 server(Serial1);

void setupServer() {
  if (server.setOprToStationSoftAP());
  else
  {
    Serial.println("softap error!");
  }
  if (server.joinAP(SSID, PASS))
  {
    Serial.print("IP:");
    Serial.println(server.getLocalIP().c_str());
  }
  else
  {
    Serial.println("join AP error!");
  }
  if (server.disableMUX());
  else
  {
    Serial.println("single error!");
  }
}

void listenClients()
{
  if (server.createTCP(HOST_NAME, HOST_PORT));
  else
  {
    Serial.println("create tcp err!");
  }
  
  requestLen = server.recv(plainRequest, sizeof(plainRequest), 10000);
  if (server.releaseTCP());
  else
  {
     Serial.print("release tcp err\r\n");
  }
}

void sendLogs()
{
  //server.send();
}

void parseRequest()
{
  Serial.print("Request: ");
  //Serial.println(plainRequest);
}
