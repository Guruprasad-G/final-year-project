
#include "esp_camera.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <iostream>
#include <sstream>
const char* ssid = "ESP32-Soft-accessPoint";
const char* password = "itworks";
AsyncWebServer server(80);

int trigPin = 4;
int echoPin = 2;
int duration;
int distance;
char string[20];
int memory = 0;

struct MOTOR_PINS
{  
  int pinIN1;
  int pinIN2;    
};

std::vector<MOTOR_PINS> motorPins = 
{
  {12, 13},  //RIGHT_MOTOR Pins (A-IN1, A-IN2)
  {14, 15},  //LEFT_MOTOR  Pins (EnB, B-IN3, B-IN4)
};

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

#define RIGHT_MOTOR 0
#define LEFT_MOTOR 1

#define FORWARD 1
#define BACKWARD -1


void rotateMotor(int motorNumber, int motorDirection)
{
  if (motorDirection == FORWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);    
  }
  else if (motorDirection == BACKWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);     
  }
  else
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);       
  }
}

void moveCar(int inputValue)
{
  Serial.printf("Got value as %d\n", inputValue);  
  switch(inputValue)
  {

    case LEFT:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, FORWARD);                  
      break;
  
    case RIGHT:
      rotateMotor(RIGHT_MOTOR, BACKWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD);  
      break;
  
    case UP:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD);  
      break;
  
    case DOWN:
      rotateMotor(RIGHT_MOTOR, BACKWARD);
      rotateMotor(LEFT_MOTOR, FORWARD); 
      break;
 
    case STOP:
      rotateMotor(RIGHT_MOTOR, STOP);
      rotateMotor(LEFT_MOTOR, STOP);    
      break;
  
    default:
      rotateMotor(RIGHT_MOTOR, STOP);
      rotateMotor(LEFT_MOTOR, STOP);    
      break;
  }
}

/*void movecar(int inp)
{
  switch(inp)
  {
    case LEFT:
      moveCar(UP);
    case UP:
      moveCar(LEFT);
    case RIGHT:
      moveCar(DOWN);
    case DOWN:
      moveCar(RIGHT);
  }
}*/

void setUpPinModes()
{
  Serial.println("Setting soft access point mode");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.on("/thisisme", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Pursuit of HAPPINESS");});
  for (int i = 0; i < motorPins.size(); i++)
  {    
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
  }
  moveCar(STOP);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  setUpPinModes();
  //setup_cam();
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  moveCar(STOP);
}
 void loop() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("UltraSound Sensor distance = ");
  Serial.println(distance);
  sprintf(string,"%d",distance);
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  delay(100);
  /*Serial.println("Moving UP");
  moveCar(UP);
  delay(2000);
  Serial.println("Moving RIGHT");
  moveCar(RIGHT);
  delay(2000);
  Serial.println("Moving LEFT");
  moveCar(LEFT);
  delay(2000);
  Serial.println("Moving DOWN");
  moveCar(DOWN);
  delay(2000);*/
  if(distance>15&&distance<70)
  {
    Serial.println("Moving UP");
    server.on("/movement", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "UP");});
    moveCar(UP);
  }
  else if(distance==15||distance==14||distance==16)
  {
    if(memory<25)
    {
      Serial.println("Moving RIGHT");
      moveCar(RIGHT);
      server.on("/movement", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "RIGHT");});
      memory++;
      memory%=50;
    }
    else
    {
      Serial.println("Moving LEFT");
      moveCar(LEFT);
      server.on("/movement", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "LEFT");});
      memory++;
      memory%=50;
    }
    
  }
  else if(distance<=15)
  {
    Serial.println("DOWN");
    moveCar(DOWN);
    server.on("/movement", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "DOWN");});
  }
  else
  {
    Serial.println("STOP");
    moveCar(STOP);
    server.on("/movement", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "STOP");});
  }
}
