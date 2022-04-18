
#define n               3   // Length/Breadth of the square arena

#define u               0   // Up
#define ur              1   // Up Right
#define r               2   // Right
#define dr              3   // Down Right
#define d               4   // Down
#define dl              5   // Down Left
#define l               6   // Left
#define ul              7   // Up Left

#define threshold_val  10   // If sensor value is beyond this threshold, we consider that there is no obstacle/wall

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


#define IRPin           4
#define DUMMY_SERVO1_PIN 12     //We need to create 2 dummy servos.
//So that ESP32Servo library does not interfere with pwm channel and timer used by esp32 camera.
#define PAN_PIN 16
