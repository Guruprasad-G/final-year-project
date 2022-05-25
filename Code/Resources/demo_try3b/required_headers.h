
#define n               3   // Length/Breadth of the square arena

#define u               0   // Up
#define ur              1   // Up Right
#define r               2   // Right
#define dr              3   // Down Right
#define d               4   // Down
#define dl              5   // Down Left
#define l               6   // Left
#define ul              7   // Up Left

#define threshold_val   15  // If sensor value is beyond this threshold, we consider that there is no obstacle/wall

#define north           0   // Directions with some value for updating graph and movement based on robot orientation
#define northeast       1
#define east            2
#define southeast       3
#define south           4
#define southwest       5
#define west            6
#define northwest       7

#define UP              1
#define DOWN            2
#define LEFT            3
#define RIGHT           4
#define STOP            0

#define RIGHT_MOTOR     0
#define LEFT_MOTOR      1

#define FORWARD         1
#define BACKWARD       -1

#define grey_pin    12
#define purple_pin  13
#define blue_pin    15
#define green_pin   14
#define trig_pin     4
#define echo_pin     2

//#define IRPin           4
#define DUMMY_SERVO1_PIN 12     //We need to create 2 dummy servos.
//So that ESP32Servo library does not interfere with pwm channel and timer used by esp32 camera.
#define PAN_PIN 16

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
