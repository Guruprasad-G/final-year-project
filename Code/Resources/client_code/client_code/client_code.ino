#include "esp_camera.h"
#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ESPAsyncWebServer.h"

const char* client_ssid = "GP";
const char* password = "gguruprasad2018";
//const char* esp1server = "http://192.168.43.123:8000/"; //ESP1's local IP I think.
String esp1_data;
String server_name = "http://192.168.43.123:8000/";

const char* server_ssid = "ESP32-Soft-accessPoint";
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.softAP(client_ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: " + IP);
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Welcome to Swarm Robo\nHere you will find more than one Robo doing a task making it efficient to solve\n For example, this robo will navigate through a maze using DFS algorithm.\nIt will search for a taget that is hidden somewhere in the maze.\nIt will also collect the environment data and send it to other robots as well as the laptop.");});
  WiFi.begin(server_ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);
  if(WiFi.status()== WL_CONNECTED ){ 
      esp1_data = httpGETRequest(server_name);
  }
  Serial.println("ESP1 Data Received is ----->>>>> \n" + esp1_data + "\n");
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "HELLO\n-_-");});
  
}

String httpGETRequest(String server_name) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(server_name.c_str());
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
