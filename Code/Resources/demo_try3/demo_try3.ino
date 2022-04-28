#include "esp_camera.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "Arduino.h"
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>
#include "required_headers.h"

int graph[n*n+1][8];        // Array of n*n+1 with 0th entry being empty. Inner array consist of the 8 different directions in which node can be connected
char print_str[n+3][n*n];   // String to represent Graph
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
int trigPin = 4;
int echoPin = 2;
int arr[2];
char string[20];
int servo_rotate[3] = {90,0,180};

//Servo dummyServo1;
Servo panServo;

#include "movement.h"
#include "graph.h"          // Header file containing necessary functions to perform on Graph
#include "algorithm.h"      // Header file containing necessary algorithms (DFS)

// 1 2 3
// 4 5 6
// 7 8 9

void setup() 
{
  Serial.begin(115200);
  initialize_graph();             //Required for pretty print of Graph
                                  //Ultra Sound Sensor Pins Declaration
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
                                  // Motor Pins Declaration
  pinMode(grey_pin, OUTPUT);      // Right Motor B-1A Grey
  pinMode(purple_pin, OUTPUT);    // Right Motor B-1B Purple
  pinMode(blue_pin, OUTPUT);      // Left Motor A-1A Blue
  pinMode(green_pin, OUTPUT);     // Left Motor A-1B Green
  moveCar(STOP);                  // Keep Robot in Stop State initially
                                  // Servo Motor Pins Declaration
  //dummyServo1.attach(DUMMY_SERVO1_PIN);
  panServo.attach(PAN_PIN);
}
void loop() 
{
  int j = 0,k;
  Serial.print("\nCurrent node = ");
  Serial.print(current_node);
  Serial.print("\n");
  Serial.print("Current orientation = ");
  Serial.print(current_orientation);
  Serial.print("\n");

  //Rotate Pan servo 90 and get Up readings
  Serial.print("Calling Pan servo to rotate 90 degrees\n");
  panServo.write(90);
  delay(1000);
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  Serial.print("Pan servo rotate = 90\n");
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Ultrasound sensor distance = ");
  Serial.print(distance);
  Serial.print("\n");
  connect_node(current_node,0,distance,current_orientation);
  delay(2000);

  //Rotate Pan servo 0 and get Right readings
  Serial.print("Calling Pan servo to rotate 0 degrees\n");
  panServo.write(0);
  delay(1000);
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  Serial.print("Pan servo rotate = 0\n");
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Ultrasound sensor distance = ");
  Serial.print(distance);
  Serial.print("\n");
  connect_node(current_node,2,distance,current_orientation);
  delay(2000);
  
  //Rotate Pan servo 180 and get Left readings
  Serial.print("Calling Pan servo to rotate 180 degrees\n");
  panServo.write(180);
  delay(1000);
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  Serial.print("Pan servo rotate = 180\n");
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Ultrasound sensor distance = ");
  Serial.print(distance);
  Serial.print("\n");
  connect_node(current_node,6,distance,current_orientation);
  delay(2000);
  
  panServo.write(90);
  Serial.print("Resetting Servo to 90\n");
  delay(2000);
  
  Serial.print("Before going to next node, this is the graph connections :- \n");
  Serial.print("N - \tU\tUR\tR\tDR\tD\tDL\tL\tUL");
  Serial.print("\n");
  print_graph();
  generate_graph();
  pretty_print_graph();
  Serial.print("\n");

  Serial.print("Printing Stack values");
  for(i=0;i<8;i++)
  {
    Serial.print(stack[i]);
    Serial.print("  ");
  }
  Serial.print("\n");

  Serial.print("Calling next node movement\n");
  next_node_movement(current_node,current_orientation,arr);
  
  current_node = arr[0];
  current_orientation = arr[1];
  Serial.print("Next node = ");
  Serial.print(current_node);
  Serial.print("\n");
  Serial.print("Next orientation = ");
  Serial.print(current_orientation);
  Serial.print("\n");
  
  moveCar(STOP);
  delay(2000);
}
