#include "esp_camera.h"
#include <Arduino.h>
#include <iostream>
#include <sstream>

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

    case UP:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, FORWARD);                  
      break;
  
    case DOWN:
      rotateMotor(RIGHT_MOTOR, BACKWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD);  
      break;
  
    case LEFT:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD);  
      break;
  
    case RIGHT:
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

void setUpPinModes()
{
      
  for (int i = 0; i < motorPins.size(); i++)
  {    
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
  }
  moveCar(STOP);
}

void setup() {
  // put your setup code here, to run once:
  setUpPinModes();
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("Going UP");
  moveCar(UP);
  delay(5000);
  Serial.printf("Going DOWN");
  moveCar(DOWN);
  delay(5000);
  Serial.printf("Going RIGHT");
  moveCar(RIGHT);
  delay(5000);
  Serial.printf("Going LEFT");
  moveCar(LEFT);
  delay(5000);
  Serial.printf("Stopping");
  moveCar(STOP);
  delay(5000);
}
