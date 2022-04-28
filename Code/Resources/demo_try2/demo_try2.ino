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
int servo_rotate[3] = {90,0,180};

/*struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;
};

std::vector<MOTOR_PINS> motorPins =
{
  {12, 13},  //RIGHT_MOTOR Pins (A-IN1, A-IN2)
  {14, 15},  //LEFT_MOTOR  Pins (B-IN3, B-IN4)
};*/

Servo dummyServo1;
Servo panServo;

#include "movement.h"
#include "graph.h"          // Header file containing necessary functions to perform on Graph
#include "algorithm.h"      // Header file containing necessary algorithms (DFS)

void setUpPinModes()
{
  initialize_graph();
  pinMode(grey_pin, OUTPUT);  // Right Motor B-1A Grey
  pinMode(purple_pin, OUTPUT);  // Right Motor B-1B Purple
  pinMode(blue_pin, OUTPUT);  // Left Motor A-1A Blue
  pinMode(green_pin, OUTPUT);  // Left Motor A-1B Green
  Serial.println("Setting soft access point mode");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Hi there from Robot!!!! {(-)_(-)}");});
  //dummyServo1.attach(DUMMY_SERVO1_PIN);
  panServo.attach(PAN_PIN);
  //pinMode(IRPin, INPUT);
  /*for (i=0;i<motorPins.size();i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
  }*/
  moveCar(STOP);
}

// 1 2 3
// 4 5 6
// 7 8 9

void setup() {
  Serial.begin(115200);
  setUpPinModes();
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
}
 void loop() {
  int j = 0,k;
  Serial.print("\n");
  Serial.print("current node = ");
  Serial.print(current_node);
  Serial.print("\n");
  Serial.print("current orientation = ");
  Serial.print(current_orientation);
  Serial.print("\n");
  for(i=0;i<3;i++)
  {
    panServo.write(servo_rotate[i]);
    delay(1000);
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    Serial.print("\n");
    Serial.print("Pan servo rotate = ");
    Serial.print(servo_rotate[i]);
    Serial.print("\n");
    duration = pulseIn(echoPin, HIGH);
    distance= duration*0.034/2;
    Serial.print("Ultrasound sensor distance = ");
    Serial.print(distance);
    Serial.print("\n");
    Serial.print("\nPrinting graph values :  ");
    print_graph();
    Serial.print("\n");
    //sprintf(string,"%d",distance); 
    //server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
    Serial.print("Debug 1: ");
    Serial.print(current_node);
    Serial.print("\t");
    Serial.print(j);
    Serial.print("\t");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(current_orientation);
    Serial.print("\t");
    Serial.print("\n");
    connect_node(current_node,j++,distance,current_orientation);
    //need to print or send back data
    Serial.print("Next iter");
  }
  //panServo.write(90);
  //Serial.print("Reset turret");
  /*for(j=0;j<8;j++)
  {
    Serial.print(graph[1][j]);
    Serial.print("\t");
  }*/
  //pretty_print_graph();
  //Serial.print("UltraSound Sensor distance = ");
  //Serial.println(distance);
  //IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(IP);
  Serial.print("\n");
  //Serial.print("current node = %d     current orientation = %d\n",current_node,current_orientation);
  sprintf(string,"%d",current_node);
  server.on("/cn", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  next_node_movement(current_node,current_orientation,arr);
  current_node = arr[0];
  current_orientation = arr[1];
  sprintf(string,"%d",current_node);
  server.on("/nn", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  //Serial.print("next node = %d     next orientation = %d\n",current_node,current_orientation);
  moveCar(STOP);
  Serial.print("\n");
  Serial.print("next node = ");
  Serial.print(current_node);
  Serial.print("\n");
  Serial.print("next orientation = ");
  Serial.print(current_orientation);
  Serial.print("\n");
  delay(2000);
}
