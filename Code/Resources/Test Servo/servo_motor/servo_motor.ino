#include "esp_camera.h"
#include <Arduino.h>
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>

//#define DUMMY_SERVO1_PIN 12     //We need to create 2 dummy servos.
//So that ESP32Servo library does not interfere with pwm channel and timer used by esp32 camera.

#define PAN_PIN 16

int trigPin = 4;
int echoPin = 2;
int distance;
int duration;

//Servo dummyServo1;
Servo panServo;

void setUpPinModes()
{
  //dummyServo1.attach(DUMMY_SERVO1_PIN); 
  panServo.attach(PAN_PIN);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
}

void setup() {
  // put your setup code here, to run once:
  setUpPinModes();
  Serial.begin(115200);
}

void loop() {
  //Rotate Pan servo 90 and get Up readings
  Serial.print("Calling Pan servo to rotate 90 degrees\n");
  panServo.write(90);
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
  delay(3000);

  //Rotate Pan servo 0 and get Right readings
  Serial.print("Calling Pan servo to rotate 0 degrees\n");
  panServo.write(0);
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
  delay(3000);
  
  //Rotate Pan servo 180 and get Left readings
  Serial.print("Calling Pan servo to rotate 180 degrees\n");
  panServo.write(180);
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
  delay(3000);
  
  panServo.write(90);
  Serial.print("Resetting Servo to 90\n");
}
