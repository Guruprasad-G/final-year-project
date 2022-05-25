#include "esp_camera.h"
#include <SPIFFS.h>
#include <FS.h>

bool initialiseCamera();
void readRGBImage();

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
    Serial.println("PSRAM Found!!!");
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    Serial.println("PSRAM NOT Found!!!");
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
}

void loop() {
  // put your main code here, to run repeatedly:
   uint32_t tTimer;
  delay(2000);
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  fb = esp_camera_fb_get();
  if (!fb) {
       Serial.println("Error: failed to capture image from camera");
       return;
     } else {
       //Serial.println("JPG image capture took " + String(millis() - tTimer) + " milliseconds");              // report time it took to capture an image
       Serial.println("Image resolution=" + String(fb->width) + "x" + String(fb->height));
       Serial.println("Image size=" + String(fb->len) + " bytes");
       Serial.println("Image format=" + String(fb->format));
       Serial.println("Free memory=" + String(ESP.getFreeHeap()) + " bytes");
     }
     void *ptrVal = NULL;                                                                                 // create a pointer for memory location to store the data
     uint32_t ARRAY_LENGTH = fb->width * fb->height * 3;                                                  // calculate memory required to store the RGB data (i.e. number of pixels in the jpg image x 3)
     if (heap_caps_get_free_size( MALLOC_CAP_SPIRAM) <  ARRAY_LENGTH) {
       Serial.println("error: not enough free psram to store the rgb data");
     }
     ptrVal = heap_caps_malloc(ARRAY_LENGTH, MALLOC_CAP_SPIRAM);                                          // allocate memory space for the rgb data
     uint8_t *rgb = (uint8_t *)ptrVal;                                                                    // create the 'rgb' array pointer to the allocated memory space
     Serial.println("Free psram after rgb data allocated = " + String(heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024) + "K");
     tTimer = millis();                                                                                   // store time that image conversion process started
     bool jpeg_converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, rgb);
     if (!jpeg_converted) {
       Serial.println("error: failed to convert image to RGB data");
       return;
     }
     Serial.println("Conversion from jpg to RGB took " + String(millis() - tTimer) + " milliseconds");// report how long the conversion took

    uint32_t resultsToShow = 38400;                                                                       // how much data to display
    Serial.println("R,G,B data for middle " + String(resultsToShow / 3) + " pixels of image");
    for (uint32_t i = 38370; i < resultsToShow-2; i+=3) 
    {
       Serial.println("Red = "+String(rgb[i+2]) + ", Green = " + String(rgb[i+1]) + ", Blue = " + String(rgb[i+0]));           // Red , Green , Blue
       // // calculate the x and y coordinate of the current pixel
          uint16_t x = (i / 3) % fb->width;
          uint16_t y = floor( (i / 3) / fb->width);
      Serial.println("Co-ordinates => x -> "+String(x)+" y -> "+String(y));
    }

    uint32_t aRed = 0;
    uint32_t aGreen = 0;
    uint32_t aBlue = 0;
    for (uint32_t i = 0; i < (ARRAY_LENGTH - 2); i+=3) 
    {                                               // go through all data and add up totals
      aBlue+=rgb[i];
      aGreen+=rgb[i+1];
      aRed+=rgb[i+2];
    }
    //aRed = aRed / (fb->width * fb->height);                                                            // divide total by number of pixels to give the average value
    //aGreen = aGreen / (fb->width * fb->height);
    //aBlue = aBlue / (fb->width * fb->height);
    Serial.println("Average Blue = " + String(aBlue));
    Serial.println("Average Green = " + String(aGreen));
    Serial.println("Average Red = " + String(aRed));
     
    esp_camera_fb_return(fb);   // camera frame buffer
    heap_caps_free(ptrVal);     // rgb data
}
