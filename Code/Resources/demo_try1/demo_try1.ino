#include "esp_camera.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>
#include "required_headers.h"

const char* ssid = "ESP32-Soft-accessPoint";
const char* password = "itworks";
AsyncWebServer server(80);

int graph[n*n+1][8];        // Array of n*n+1 with 0th entry being empty. Inner array consist of the 8 different directions in which node can be connected
char print_str[n+3][n*n];   // String to represent Graph
int visited_nodes[n*n] = {0};   // Array to keep track of the nodes that are visited
int stack[n*n] = {0};       // Stack for back tracing in case of a dead end
int stack_pointer = 0;      // Stack pointer to keep tract of last inserted or deleted element from Stack
int distance;
int duration;
int current_node = 1;
int current_orientation = 0;
int i;
int rec_msg[2];
int trigPin = 4;
int echoPin = 2;
int arr[2];
char string[20];

struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;
};

std::vector<MOTOR_PINS> motorPins =
{
  {12, 13},  //RIGHT_MOTOR Pins (A-IN1, A-IN2)
  {14, 15},  //LEFT_MOTOR  Pins (B-IN3, B-IN4)
};

Servo dummyServo1;
Servo panServo;

#include "movement.h"
#include "graph.h"          // Header file containing necessary functions to perform on Graph
#include "algorithm.h"      // Header file containing necessary algorithms (DFS)

void setUpPinModes()
{
  initialize_graph();
  Serial.println("Setting soft access point mode");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  delay(20000);
  server.on("/thisisme", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Pursuit of HAPPINESS");});
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Hi there from Robot!!!! {(-)_(-)}");});
  dummyServo1.attach(DUMMY_SERVO1_PIN);
  panServo.attach(PAN_PIN);
  //pinMode(IRPin, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  for (i=0;i<motorPins.size();i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
  }
  moveCar(STOP);
}

void setup() {
  Serial.begin(115200);
  setUpPinModes();
}
 void loop() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int j;
  Serial.print("\nPrinting graph values :  ");
  for(j=0;j<8;j++)
  {
    Serial.print(graph[1][j]);
    Serial.print("\t");
  }
  //Serial.print("UltraSound Sensor distance = ");
  //Serial.println(distance);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  Serial.print("current node = ");
  Serial.print(current_node);
  
  for(i=0;i<4;i++)
  {
    panServo.write(i*45);
    duration = pulseIn(echoPin, HIGH);
    distance= duration*0.034/2;
    sprintf(string,"%d",distance);
    server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
    //Serial.print("Ultrasound sensor distance = %d\n",distance);
    connect_node(current_node,i,distance,current_orientation);
    //need to print or send back data
    delay(1000);
  }
  panServo.write(90);
  //Serial.print("current node = %d     current orientation = %d\n",current_node,current_orientation);
  sprintf(string,"%d",current_node);
  server.on("/cn", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  next_node_movement(current_node,current_orientation,arr);
  current_node = arr[0];
  current_orientation = arr[1];
  sprintf(string,"%d",current_node);
  server.on("/nn", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  //Serial.print("next node = %d     next orientation = %d\n",current_node,current_orientation);
}
