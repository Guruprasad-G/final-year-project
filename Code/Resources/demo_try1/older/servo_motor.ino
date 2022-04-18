#include "esp_camera.h"
#include <Arduino.h>
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>

#define DUMMY_SERVO1_PIN 12     //We need to create 2 dummy servos.
//So that ESP32Servo library does not interfere with pwm channel and timer used by esp32 camera.

#define PAN_PIN 16

Servo dummyServo1;
Servo panServo;

void setUpPinModes()
{
  dummyServo1.attach(DUMMY_SERVO1_PIN); 
  panServo.attach(PAN_PIN);
}

void setup() {
  // put your setup code here, to run once:
  setUpPinModes();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("Servo : 90\n");
  panServo.write(90);
  delay(5000);
  Serial.printf("Servo : 180\n");
  panServo.write(180);
  delay(5000);
  Serial.printf("Servo : 270\n");
  panServo.write(270);
  delay(5000);
  Serial.printf("Servo : 360\n");
  panServo.write(360);
  delay(5000);
  Serial.printf("Servo : 45\n");
  panServo.write(45);
  delay(5000);
  Serial.printf("Servo : 60\n");
  panServo.write(60);
  delay(5000);
}
