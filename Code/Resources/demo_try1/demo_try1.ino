#include "esp_camera.h"
#include "WiFi.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>
#include "required_headers.h"

//const char* ssid = "ESP32-Soft-accessPoint";
//const char* password = "itworks";
//AsyncWebServer server(80);
const char* ssid = "GP";
const char* password = "gguruprasad2018";
void startCameraServer();

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
  //************************************************************************************************************
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  WiFi.begin(ssid, password);

  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  //  Serial.print(".");
  //}
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
  
  //**************************************************************************************************************
  initialize_graph();
  pinMode(grey_pin, OUTPUT);  // Right Motor B-1A Grey
  pinMode(purple_pin, OUTPUT);  // Right Motor B-1B Purple
  pinMode(blue_pin, OUTPUT);  // Left Motor A-1A Blue
  pinMode(green_pin, OUTPUT);  // Left Motor A-1B Green
  Serial.println("Setting soft access point mode");
  //WiFi.softAP(ssid, password);
  //IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(IP);
  //server.begin();
  //server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Hi there from Robot!!!! {(-)_(-)}");});
  //dummyServo1.attach(DUMMY_SERVO1_PIN);
  //panServo.attach(PAN_PIN);
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
  /*connect_node(current_node,j++,distance,current_orientation);
  moveCar(RIGHT);
  distance = us();
  connect_node(current_node,j++,distance,current_orientation);
  moveCar(RIGHT);
  distance = us();
  connect_node(current_node,j++,distance,current_orientation);
  moveCar(LEFT);
  moveCar(LEFT);
  moveCar(LEFT);
  distance = us();
  connect_node(current_node,j++,distance,current_orientation);
  moveCar(LEFT);
  distance = us();
  connect_node(current_node,j++,distance,current_orientation);
  moveCar(RIGHT);
  moveCar(RIGHT);*/
  Serial.print("\nPrinting graph values :  ");
  Serial.print("\n");
  print_graph();
  for(i=0;i<3;i++)
  {
    if(i==0)
    panServo.write(90);
    else if(i == 1)
    panServo.write(0);
    else if(i==2)
    panServo.write(180);
    else
    break;
    delay(6000);
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
    Serial.print("Current node = ");
    Serial.print(current_node);
    Serial.print("\t");
    Serial.print("Pan direction = ");
    Serial.print(j);
    Serial.print("\t");
    Serial.print("Sensor val = ");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print("Current orientation = ");
    Serial.print(current_orientation);
    Serial.print("\t");
    Serial.print("\n");
    connect_node(current_node,j++,distance,current_orientation);
    //need to print or send back data
    Serial.print("Next iter");
  }
  Serial.print("Reset turret");
  panServo.write(90);
  /*for(j=0;j<8;j++)
  {
    Serial.print(graph[1][j]);
    Serial.print("\t");
  }*/
  Serial.print("\nPrinting graph values :  ");
  print_graph();
  Serial.print("\n");
  //Serial.print("UltraSound Sensor distance = ");
  //Serial.println(distance);
  //IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(IP);
  Serial.print("\n");
  //Serial.print("current node = %d     current orientation = %d\n",current_node,current_orientation);
  //sprintf(string,"%d",current_node);
  //server.on("/cn", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  next_node_movement(current_node,current_orientation,arr);
  current_node = arr[0];
  current_orientation = arr[1];
  //sprintf(string,"%d",current_node);
  //server.on("/nn", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
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
