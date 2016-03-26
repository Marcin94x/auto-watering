#include <ESP8266.h>

const char SSID[] = "castle";
const char PASS[] = "zamek12345678";
const char HOST_NAME[] = "www.baidu.com";
const byte HOST_PORT = 80;

String plainRequest = "";
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
  char *hello = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n";
  uint8_t buffer[1024] = {0};
  server.send((const uint8_t*)hello, strlen(hello));
  uint32_t len = server.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
    Serial.print("Received:[");
    for(uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
     }
     Serial.print("]\r\n");
  }
  if (server.releaseTCP());
  else
  {
     Serial.print("release tcp err\r\n");
  }
}

void parseRequest()
{
  Serial.print("Request: ");
  Serial.println(plainRequest);
}
