#include "esp_camera.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "HTTPClient.h"
#include "Arduino.h"
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>
#include "required_headers.h"

int graph[n*n+1][8];        // Array of n*n+1 with 0th entry being empty. Inner array consist of the 8 different directions in which node can be connected
char print_str[n+3][n*n+1];   // String to represent Graph
int visited_nodes[n*n] = {0};   // Array to keep track of the nodes that are visited
int stack[n*n] = {0};       // Stack for back tracing in case of a dead end
int stack_pointer = 0;      // Stack pointer to keep tract of last inserted or deleted element from Stack
int distance;
int duration;
int current_node = 1;
int current_orientation = 0;
int previous_node = 0;
int i;
int rec_msg[2];
int arr[2];
char string[20];

Servo dummyServo1;
Servo panServo;
const char* ssid = "ESP32-Soft-accessPoint";
const char* password = "itworks";
AsyncWebServer server(80);

#include "movement.h"
#include "graph.h"          // Header file containing necessary functions to perform on Graph
#include "algorithm.h"      // Header file containing necessary algorithms (DFS)
#include "robot_commands.h"

// 1 2 3
// 4 5 6
// 7 8 9

void setup() 
{
  Serial.begin(115200);
  initialize_graph();             //Required for pretty print of Graph
                                  //Ultra Sound Sensor Pins Declaration
  pinMode(trig_pin, OUTPUT); 
  pinMode(echo_pin, INPUT);
                                  // Motor Pins Declaration
  pinMode(grey_pin, OUTPUT);      // Right Motor B-1A Grey
  pinMode(purple_pin, OUTPUT);    // Right Motor B-1B Purple
  pinMode(blue_pin, OUTPUT);      // Left Motor A-1A Blue
  pinMode(green_pin, OUTPUT);     // Left Motor A-1B Green
  moveCar(STOP);                  // Keep Robot in Stop State initially
                                  // Servo Motor Pins Declaration
  //dummyServo1.attach(DUMMY_SERVO1_PIN);
  panServo.attach(PAN_PIN);
  //setup_wlan();

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: " + IP);
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Welcome to Swarm Robo\nHere you will find more than one Robo doing a task making it efficient to solve\n For example, this robo will navigate through a maze using DFS algorithm.\nIt will search for a taget that is hidden somewhere in the maze.\nIt will also collect the environment data and send it to other robots as well as the laptop.");});

}
void loop() 
{
  Serial.println("\nCurrent node = " + current_node);
  Serial.println("Current orientation = " + current_orientation);
  sprintf(string,"%d",current_node);
  server.on("/current_node", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  sprintf(string,"%d",current_orientation);
  server.on("/current_orientation", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});

  //-------------------------------------------------

  Serial.println("Calling Pan servo to rotate 90");
  
  panServo.write(90);
  delay(1000);
  
  Serial.println("Pan servo rotate = 90");
  //sprintf(string,"%d",servo_position);
  //server.on("/servo_position", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  
  digitalWrite(trig_pin, LOW); delayMicroseconds(2); digitalWrite(trig_pin, HIGH); delayMicroseconds(10); digitalWrite(trig_pin, LOW);  
  duration = pulseIn(echo_pin, HIGH);
  distance = duration*0.034/2;
  
  Serial.println("Ultrasound sensor distance = " + distance);
  sprintf(string,"%d",distance);
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  
  connect_node(current_node,0,distance,current_orientation);
  delay(2000);

  //---------------------------------------------------

  Serial.println("Calling Pan servo to rotate 0");
  
  panServo.write(0);
  delay(1000);
  
  Serial.println("Pan servo rotate = 0");
  //sprintf(string,"%d",servo_position);
  //server.on("/servo_position", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  
  digitalWrite(trig_pin, LOW); delayMicroseconds(2); digitalWrite(trig_pin, HIGH); delayMicroseconds(10); digitalWrite(trig_pin, LOW);  
  duration = pulseIn(echo_pin, HIGH);
  distance = duration*0.034/2;
  
  Serial.println("Ultrasound sensor distance = " + distance);
  sprintf(string,"%d",distance);
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  
  connect_node(current_node,0,distance,current_orientation);
  delay(2000);
  
  //--------------------------------------------------------------------------------

  Serial.println("Calling Pan servo to rotate 180");
  
  panServo.write(180);
  delay(1000);
  
  Serial.println("Pan servo rotate = 180");
  //sprintf(string,"%d",servo_position);
  //server.on("/servo_position", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  
  digitalWrite(trig_pin, LOW); delayMicroseconds(2); digitalWrite(trig_pin, HIGH); delayMicroseconds(10); digitalWrite(trig_pin, LOW);  
  duration = pulseIn(echo_pin, HIGH);
  distance = duration*0.034/2;
  
  Serial.println("Ultrasound sensor distance = " + distance);
  sprintf(string,"%d",distance);
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  
  connect_node(current_node,0,distance,current_orientation);
  delay(2000);

  //------------------------------------------------------------------------------------

  panServo.write(90);
  Serial.println("Resetting Servo to 90");
  delay(2000);
  
  Serial.println("Before going to next node, this is the graph connections :- ");
  Serial.println("N - \tU\tUR\tR\tDR\tD\tDL\tL\tUL");
  print_graph(); Serial.println();
  pretty_print_graph(); Serial.println();
  Serial.print("Printing Stack values");
  for(i=0;i<8;i++)
  {
    Serial.print(stack[i]);
    Serial.print("  ");
  }
  Serial.println();

  Serial.println("Calling next node movement");
  next_node_movement(current_node,current_orientation,arr);
  current_node = arr[0]; current_orientation = arr[1];
  Serial.println("Next node = " + current_node);
  Serial.println("Next orientation = " + current_orientation);
  moveCar(STOP);
  patch_graph();
  delay(2000);
  
}
/*
void obtain_sensor_values_from_robot(int servo_position)
{
  
}
*/
