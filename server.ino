#include <ESP8266.h>


const char SSID[] = "castle";
const char PASS[] = "zamek12345678";
const char HOST_NAME[] = "www.baidu.com";
const byte HOST_PORT = 80;

String plainRequest = "";


void setupServer() {
  if (client.setOprToStationSoftAP());
  else
  {
    Serial.println("softap error!");
  }
  if (client.joinAP(SSID, PASS))
  {
    Serial.print("IP:");
    Serial.println(client.getLocalIP().c_str());
  }
  else
  {
    Serial.println("join AP error!");
  }
  if (client.disableMUX());
  else
  {
    Serial.println("single error!");
  }
  if (client.createTCP(HOST_NAME, HOST_PORT));
  else
  {
    Serial.println("create tcp err!");
  }
}

void listenClients()
{
  
  char *hello = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n";
  uint8_t buffer[1024] = {0};
  client.send((const uint8_t*)hello, strlen(hello));
  uint32_t len =client.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
    Serial.print("Received:[");
    for(uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
     }
     Serial.print("]\r\n");
  }
}

void parseRequest()
{
  Serial.print("Request: ");
  Serial.println(plainRequest);
}
