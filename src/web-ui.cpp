#include "modular-ui.h"
#include "ui.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

//  /*
//   AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
//   -----
//   Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
//   Important Notice: Star the repository on Github if you like the library! :)
//   Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
// */

const char* ssid = "";
const char* password = "";

// // Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

String getOutputStates(){
  JSONVar myArray;
  //for (int i =0; i<NUM_OUTPUTS; i++){
    myArray["controls"][0]["name"] = "vu";
    myArray["controls"][0]["state"] = String(vu);
    myArray["controls"][1]["name"] = "white";
    myArray["controls"][1]["state"] = String(white);
    myArray["controls"][2]["name"] = "animation";
    myArray["controls"][2]["state"] = String(showAnimation);
    myArray["controls"][3]["name"] = "color";
    myArray["controls"][3]["state"] = getColorWheelValue();
  //}
  String jsonString = JSON.stringify(myArray);
  return jsonString;
}

void notifyClients(String state) {
  ws.textAll(state);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String message = (char*)data;
    if (message == "states") {
      notifyClients(getOutputStates());
    }
    else if (message == "vu") {
      toggleVuState();
      notifyClients(getOutputStates());
    }   
    else if (message == "white") {
      toggleWhiteState();
      notifyClients(getOutputStates());
    }
    else if (message == "animation") {
      toggleAnimationState();
      notifyClients(getOutputStates());
    }
    else if (message.startsWith("#")){
      setColorWheelValue(message);
    }
    else
    {
      char * value = (char *)data;
      int snuh = atoi(value);
      setAnimation(snuh);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}


void setupWebUi(){
  WiFi.setHostname("modular-ui.local"); //define hostname
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());

  initSPIFFS();

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html",false);
  });

  server.serveStatic("/", SPIFFS, "/");

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();
}

void webUiLoop() {
  ws.cleanupClients();
}

void updateWebUi(){
  notifyClients(getOutputStates());
}