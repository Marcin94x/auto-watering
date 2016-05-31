#include <ESP8266.h>

const char SSID[] = "watering"; /**< wifi network name */
const char PASS[] = "watering1"; /**< wifi network password */

uint32_t len; /**< length of a TCP request */
uint8_t buffer[128]; /**< buffer for a TCP request */
uint8_t mux_id; /**< id of a TCP client */
String request; /**< a request */

/**
 * Sets up a TCP server on 8090 port.
 */
void setupServer() {
  Serial.println("setup wifi begin");
  
  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());
  
  if (wifi.joinAP(SSID, PASS)) {
    Serial.println("Join AP success");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());    
  } else {
    Serial.println("Join AP failure");
  }
  
  if (wifi.enableMUX()) {
    Serial.println("multiple ok");
  } else {
    Serial.println("multiple err");
  }
  
  if (wifi.startTCPServer(8090)) {
    Serial.println("start tcp server ok");
  } else {
    Serial.println("start tcp server err");
  }
  
  if (wifi.setTCPServerTimeout(10)) { 
    Serial.println("set tcp server timout 10 seconds");
  } else {
    Serial.println("set tcp server timout err");
  }

  Serial.println("setup end");
}

/**
 * Listens to TCP clients and reads its messages to buffer.
 */
void listenClients() {
  len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
  if (len > 0) {
    Serial.print("Status:[");
    Serial.print(wifi.getIPStatus().c_str());
    Serial.println("]");

    Serial.print("Received from :");
    Serial.print(mux_id);
    Serial.print("[");
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char) buffer[i]);
    }
    Serial.println("]");
    
    parseRequest();

    if (wifi.releaseTCP(mux_id)) {
      Serial.print("release tcp ");
      Serial.print(mux_id);
      Serial.println(" ok");
    } else {
      Serial.print("release tcp");
      Serial.print(mux_id);
      Serial.println(" err");
    }

    Serial.print("Status:[");
    Serial.print(wifi.getIPStatus().c_str());
    Serial.println("]");
  }
}

/**
 * Analyzes a received message and sends an answer.
 */
void parseRequest() {
  request = "";
  for (uint32_t i = 0; i < len - 1; i++) {
    request += (char) buffer[i];
  }
  if (request == "gettemp") {
    char buffer[4];
    sprintf(buffer, "%d", getTemperature());
    const uint8_t *temp = (const uint8_t *) buffer;
    wifi.send(mux_id, temp, strlen(buffer));
  } else if (request == "getairhumid") {
    char buffer[4];
    sprintf(buffer, "%d", getAirHumidity());
    const uint8_t *humid = (const uint8_t *) buffer;
    wifi.send(mux_id, humid, strlen(buffer));
  } else if (request == "getsoilhumid") {
    char buffer[4];
    sprintf(buffer, "%d", getSoilHumidity());
    const uint8_t *humid = (const uint8_t *) buffer;
    wifi.send(mux_id, humid, strlen(buffer));
  } else if (request == "getinsol") {
    char buffer[4];
    sprintf(buffer, "%d", getInsolation());
    const uint8_t *insol = (const uint8_t *) buffer;
    wifi.send(mux_id, insol, strlen(buffer));
  } else if (request == "getliquidlvl") {
    char buffer[4];
    sprintf(buffer, "%d", getLiquidLevel());
    const uint8_t *lvl = (const uint8_t *) buffer;
    wifi.send(mux_id, lvl, strlen(buffer));
  } else if (request == "setpump") {
    setPump();
  } else if (request == "setpumpoff") {
    setPumpOff();
  }
}
