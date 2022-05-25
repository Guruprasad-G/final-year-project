#include "esp_camera.h"
#include <base64.h>             // for encoding buffer to display image on page
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
 #include <SPIFFS.h>
 #include <FS.h>
 #include "soc/soc.h"
 #include "soc/rtc_cntl_reg.h"
 uint32_t lastStatus = millis();           // last time status light changed status (to flash all ok led)
 bool sdcardPresent;                       // flag if an sd card is detected
 int imageCounter;                         // image file name on sd card counter
 String spiffsFilename = "/image.jpg";     // image name to use when storing in spiffs
 String ImageResDetails = "Unknown";       // image resolution info
const int TimeBetweenStatus = 600;                     // speed of flashing system running ok status light (milliseconds)

bool initialiseCamera();
void readRGBImage();

#define CAMERA_MODEL_AI_THINKER
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
 #define PIXFORMAT PIXFORMAT_JPEG;

bool sendRGBfile = 0;

void setup() {
  // put your setup code here, to run once:
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1);     // Turn-off the 'brownout detector'
  // set up camera
     Serial.print(("\nInitialising camera: "));
     if (initialiseCamera()) {
       Serial.println("OK");
     }
     else {
       Serial.println("failed");
     }

 // Spiffs - for storing images without an sd card
 //       see: https://circuits4you.com/2018/01/31/example-of-esp8266-flash-file-system-spiffs/
   if (!SPIFFS.begin(true)) {
     Serial.println(("An Error has occurred while mounting SPIFFS - restarting"));
     delay(5000);
     ESP.restart();                               // restart and try again
     delay(5000);
   } else {
     // SPIFFS.format();      // wipe spiffs
       Serial.print(("SPIFFS mounted successfully: "));
       Serial.printf("total bytes: %d , used: %d \n", SPIFFS.totalBytes(), SPIFFS.usedBytes());
   }
   readRGBImage();
   delay(5000);
   readRGBImage();
}
///////////////////////////////////////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  if ((unsigned long)(millis() - lastStatus) >= TimeBetweenStatus) {
     lastStatus = millis();                                               // reset timer
    }
  Serial.println(("Looping...."));
  delay(40000);
}
///////////////////////////////////////////////////////////////////////////////
bool initialiseCamera() {
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
   //config.jpeg_quality = 12;                     // 0-63 lower number means higher quality (can cause failed image capture if set too low at higher resolutions)
   //config.fb_count = 1;                          // if more than one, i2s runs in continuous mode. Use only with JPEG

   // check the esp32cam board has a psram chip installed (extra memory used for storing captured images)
   //    Note: if not using "AI thinker esp32 cam" in the Arduino IDE, SPIFFS must be enabled
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

   esp_err_t camerr = esp_camera_init(&config);  // initialise the camera
   if (camerr != ESP_OK) {
     Serial.printf("ERROR: Camera init failed with error 0x%x", camerr);
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

   return (camerr == ESP_OK);                    // return boolean result of camera initialisation
}

//////////////////////////////////////////////////////////////////////////////
void readRGBImage() { 
 
 uint32_t tTimer;     // used to time tasks                                                                    // open link with client

 if (!sendRGBfile) {
     Serial.println("Live image as rgb data, requested by ");                                                            // 'sendText' sends the String to both serial port and web page
 }

 // make sure psram is available
 if (!psramFound()) {
   Serial.println("error: no psram available to store the RGB data");
   return;
 }


 //   ****** the main code for converting an image to RGB data *****

   // capture a live image from camera (as a jpg)
     camera_fb_t * fb = NULL;
     tTimer = millis();                                                                                    // store time that image capture started
     fb = esp_camera_fb_get();
     if (!fb) {
       Serial.println("Error: failed to capture image from camera");
       return;
     } else {
       Serial.println("JPG image capture took " + String(millis() - tTimer) + " milliseconds");              // report time it took to capture an image
       Serial.println("Image resolution=" + String(fb->width) + "x" + String(fb->height));
       Serial.println("Image size=" + String(fb->len) + " bytes");
       Serial.println("Image format=" + String(fb->format));
       Serial.println("Free memory=" + String(ESP.getFreeHeap()) + " bytes");
     }
      esp_camera_fb_return(fb);   // camera frame buffer
   //heap_caps_free(ptrVal);     // rgb data
}
/*
   // display captured image using base64 - seems a bit unreliable especially with larger images?
    if (!sendRGBfile) {
      client.print("<br>Displaying image direct from frame buffer");
      String base64data = base64::encode(fb->buf, fb->len);      // convert buffer to base64
      client.print(" - Base64 data length = " + String(base64data.length()) + " bytes\n" );
      client.print("<br><img src='data:image/jpg;base64," + base64data + "'></img><br>\n");
    }
*/
  /////////---------------------------------------------------------------
/*
   // allocate memory to store the rgb data (in psram, 3 bytes per pixel)
     Serial.println("<br>Free psram before rgb data allocated = " + String(heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024) + "K");
     void *ptrVal = NULL;                                                                                 // create a pointer for memory location to store the data
     uint32_t ARRAY_LENGTH = fb->width * fb->height * 3;                                                  // calculate memory required to store the RGB data (i.e. number of pixels in the jpg image x 3)
     if (heap_caps_get_free_size( MALLOC_CAP_SPIRAM) <  ARRAY_LENGTH) {
       Serial.println("error: not enough free psram to store the rgb data");
       return;
     }
     ptrVal = heap_caps_malloc(ARRAY_LENGTH, MALLOC_CAP_SPIRAM);                                          // allocate memory space for the rgb data
     uint8_t *rgb = (uint8_t *)ptrVal;                                                                    // create the 'rgb' array pointer to the allocated memory space
     Serial.println("Free psram after rgb data allocated = " + String(heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024) + "K");

   // convert the captured jpg image (fb) to rgb data (store in 'rgb' array)
     tTimer = millis();                                                                                   // store time that image conversion process started
     bool jpeg_converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, rgb);
     if (!jpeg_converted) {
       Serial.println("error: failed to convert image to RGB data");
       return;
     }
     Serial.println("Conversion from jpg to RGB took " + String(millis() - tTimer) + " milliseconds");// report how long the conversion took


   // if sendRGBfile is set then just send raw RGB data and close
   if (sendRGBfile) {
     //Serial.println(rgb, ARRAY_LENGTH);          // send the raw rgb data
     esp_camera_fb_return(fb);                 // camera frame buffer
     delay(1000);
     return;
   }


 //   ****** examples of using the resulting RGB data *****

   // display some of the resulting data
       uint32_t resultsToShow = 50;                                                                       // how much data to display
       Serial.println("<br>R,G,B data for first " + String(resultsToShow / 3) + " pixels of image");
       for (uint32_t i = 0; i < resultsToShow-2; i+=3) {
         Serial.println(String(rgb[i+2]) + "," + String(rgb[i+1]) + "," + String(rgb[i+0]));           // Red , Green , Blue
         // // calculate the x and y coordinate of the current pixel
         //   uint16_t x = (i / 3) % fb->width;
         //   uint16_t y = floor( (i / 3) / fb->width);
       }

   // find the average values for each colour over entire image
       uint32_t aRed = 0;
       uint32_t aGreen = 0;
       uint32_t aBlue = 0;
       for (uint32_t i = 0; i < (ARRAY_LENGTH - 2); i+=3) {                                               // go through all data and add up totals
         aBlue+=rgb[i];
         aGreen+=rgb[i+1];
         aRed+=rgb[i+2];
       }
       aRed = aRed / (fb->width * fb->height);                                                            // divide total by number of pixels to give the average value
       aGreen = aGreen / (fb->width * fb->height);
       aBlue = aBlue / (fb->width * fb->height);
       Serial.println("Average Blue = " + String(aBlue));
       Serial.println("Average Green = " + String(aGreen));
       Serial.println("Average Red = " + String(aRed));


 //   *******************************************************


 // finished with the data so free up the memory space used in psram
   esp_camera_fb_return(fb);   // camera frame buffer
   heap_caps_free(ptrVal);     // rgb data
/*
////////////////////////////////////----------------------
}  // readRGBImage*/
