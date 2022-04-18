#include "esp_camera.h"
#include <Arduino.h>
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>

int graph[n*n+1][8];        // Array of n*n+1 with 0th entry being empty. Inner array consist of the 8 different directions in which node can be connected
char print_str[n+3][n*n];   // String to represent Graph
int visited_nodes[n*n] = {0};   // Array to keep track of the nodes that are visited
int stack[n*n] = {0};       // Stack for back tracing in case of a dead end
int stack_pointer = 0;      // Stack pointer to keep tract of last inserted or deleted element from Stack
int distance;
int current_node = 1;
int current_orientation = 0;
int i;
int rec_msg[2];

#include "required_headers.h"
#include "movement.h"
#include "graph.h"          // Header file containing necessary functions to perform on Graph
#include "algorithm.h"      // Header file containing necessary algorithms (DFS)

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

void setUpPinModes()
{
  initialize_graph();
  dummyServo1.attach(DUMMY_SERVO1_PIN);
  panServo.attach(PAN_PIN);
  pinMode(IRPin, INPUT);
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
  for(i=0;i<8;i++)
  {
    panServo.write(i*45);
    distance = digitalRead(IRPin);
    connect_node(current_node,i,distance,current_orientation)
    //need to print or send back data
    delay(1000);
  }
  rec_msg = next_node_movement(int current_node, int current_orientation);
  current_node = rec_msg[0];
  current_orientation = rec_msg[1];
}
