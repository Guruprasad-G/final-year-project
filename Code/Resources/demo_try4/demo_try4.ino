#include "esp_camera.h"
#include "WiFi.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "ESPAsyncWebServer.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "Arduino.h"
#include "driver/rtc_io.h"
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
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
int i;
int rec_msg[2];
int trigPin = 4;
int echoPin = 2;
int arr[2];
char string[20];
int servo_rotate[3] = {90,0,180};

Servo dummyServo1;
Servo panServo;

// Replace with your network credentials
const char* ssid = "GP";
const char* password = "gguruprasad2018";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

boolean takeNewPhoto = false;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { text-align:center; }
    .vert { margin-bottom: 10%; }
    .hori{ margin-bottom: 0%; }
  </style>
</head>
<body>
  <div id="container">
    <h2>ESP32-CAM Last Photo</h2>
    <p>It might take more than 5 seconds to capture a photo.</p>
    <p>
      <button onclick="rotatePhoto();">ROTATE</button>
      <button onclick="capturePhoto()">CAPTURE PHOTO</button>
      <button onclick="location.reload();">REFRESH PAGE</button>
    </p>
  </div>
  <div><img src="saved-photo" id="photo" width="70%"></div>
</body>
<script>
  var deg = 0;
  function capturePhoto() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/capture", true);
    xhr.send();
  }
  function rotatePhoto() {
    var img = document.getElementById("photo");
    deg += 90;
    if(isOdd(deg/90)){ document.getElementById("container").className = "vert"; }
    else{ document.getElementById("container").className = "hori"; }
    img.style.transform = "rotate(" + deg + "deg)";
  }
  function isOdd(n) { return Math.abs(n % 2) == 1; }
</script>
</html>)rawliteral";

// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

#include "movement.h"
#include "graph.h"          // Header file containing necessary functions to perform on Graph
#include "algorithm.h"      // Header file containing necessary algorithms (DFS)
#include "camera.h"

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

  setup_camera();
}
void loop() 
{
  int j = 0,k;
  Serial.print("\nCurrent node = \n");
  Serial.print(current_node);
  Serial.print("Current orientation = \n");
  Serial.print(current_orientation);
  sprintf(string,"%d",current_orientation); 
  server.on("/currentnode", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});

  //Rotate Pan servo 90 and get Up readings
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
  sprintf(string,"%d",distance); 
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  connect_node(current_node,j++,distance,current_orientation);
  delay(2000);

  //Rotate Pan servo 0 and get Right readings
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
  sprintf(string,"%d",distance); 
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  connect_node(current_node,j++,distance,current_orientation);
  delay(2000);
  
  //Rotate Pan servo 180 and get Left readings
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
  sprintf(string,"%d",distance); 
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  connect_node(current_node,j++,distance,current_orientation);
  delay(2000);
  
  panServo.write(90);
  Serial.print("Resetting Servo to 90\n");

  Serial.print("Before going to next node, this is the graph connections :- ");
  pretty_print_graph();
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
