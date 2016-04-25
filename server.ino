#include <ESP8266.h>

const char SSID[] = "castle";
const char PASS[] = "zamek12345678";

uint32_t len;
uint8_t buffer[128];
uint8_t mux_id;
String request;

void setupServer() {
  Serial.println("setup wifi begin");
  
  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());
  
  if (wifi.setOprToStationSoftAP()) {
    Serial.println("to station + softap ok");
  } else {
    Serial.println("to station + softap err");
  }
  
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

void listenClients()
{
  len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
  if (len > 0) {
    Serial.print("Status:[");
    Serial.print(wifi.getIPStatus().c_str());
    Serial.println("]");

    Serial.print("Received from :");
    Serial.print(mux_id);
    Serial.print("[");
    for(uint32_t i = 0; i < len; i++) {
      Serial.print((char) buffer[i]);
    }
    Serial.println("]");
    parseRequest();

    /*if(wifi.send(mux_id, buffer, len)) {
      Serial.print("send back ok\r\n");
    } 
    else {
      Serial.print("send back err\r\n");
    }*/

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

void parseRequest()
{
  request = "";
  for (uint32_t i = 0; i < len - 1; i++) {
    request += (char) buffer[i];
  }
  if (request == "gettemp") {
    getTemperature();
  } else if (request == "getairhumid") {
    getAirHumidity();
  } else if (request == "getsoilhumid") {
    getSoilHumidity();
  } else if (request == "getinsol") {
    getInsolation();
  } else if (request == "getliquidlvl") {
    getLiquidLevel();
  } else if (request == "setpump") {
    setPump();
  } else if (request == "setpumpoff") {
    setPumpOff();
  }
}
