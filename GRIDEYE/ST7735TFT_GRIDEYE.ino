//This is an implementation of a grideye with ST7735 1.8 display 
//We need two libraries: 
//1. Go to Library Manager and install "SparkFun_GridEYE_Arduino_Library"
//2. In the Library manger search and install "Adafruit ST7735 and ST7789 Library"
//3. Open new file delete everything and paste the code from 


// using example code from https://learn.sparkfun.com/tutorials/qwiic-grid-eye-infrared-array-amg88xx-hookup-guide
#include <SparkFun_GridEYE_Arduino_Library.h>
#include <Wire.h>

// Use these values (in degrees C) to adjust the contrast
#define HOT  30
#define COLD 20

// This table can be of type int because we map the pixel 
// temperature to 0-3. Temperatures are reported by the 
// library as floats
int pixelTable[64];
GridEYE grideye;

//#include <UTFT.h>  //for setColor

// grabbed it from https://steemit.com/utopian-io/@pakganern/arduino-1-8-tft-st7735-lcd-tutorial
// He got pin 8 and 9 in reverse.
// Correct connections:
// VCC   5V
// GND   GND
// CS    D10
// RESET D8
// AO    D9
// SDA   D11
// SCK   D13
// LED   3.3V

#include <Adafruit_GFX.h>    // include Core graphics library
#include <Adafruit_ST7735.h> // include Hardware-specific library
#include <SPI.h>

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    8  // you can also connect this to the Arduino reset                      
#define TFT_DC     9  // in which case, set this #define pin to -1!

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
float p = 3.1415926;

//#####################
int p_width      = 14;
int p_height     = p_width;
int row_height   = p_width +1;
int column_width = p_height+1;
int x_start      = 5;
int y_start      = 15;
int x = x_start;
int y = y_start;

int r =0;
int g =0;
int b =0;
int p_color =0;
int t       =0;
int t_max   =100;
int t_min   =0;
//#####################
uint16_t color = 0;

void setup() {
  // Start your preferred I2C object 
  Wire.begin();
  // Library assumes "Wire" for I2C but you can pass something else with begin() if you like
  grideye.begin();
  Serial.begin(115200);

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);
  
  my_drawtext("IR Grideye camera", ST7735_WHITE);
  delay(500);
  
}

void loop() {

  // loop through all 64 pixels on the device and map each float value to a number
  // between 0 and 3 using the HOT and COLD values we set at the top of the sketch
  for(unsigned char i = 0; i < 64; i++){
    pixelTable[i] = map(grideye.getPixelTemperature(i), COLD, HOT, 0, 3);
    int temp = grideye.getPixelTemperature(i);
    Serial.println(temp);
  }
 
  y=y_start;
  x=x_start;
  
  for (unsigned char i = 0; i < 64; i++) {
    if (i > 0 && i % 8 == 0) {
      x=x_start;
      y=y+row_height;
    }

         if(pixelTable[i]==0) {color = 0x0000;}
    else if(pixelTable[i]==1) {color = 0x00FF;}
    else if(pixelTable[i]==2) {color = 0xFF00;}
    else if(pixelTable[i]==3) {color = 0xFFFF;}

 
    tft.fillRect(x, y, p_width, p_height, color );
    x=x+column_width;
  }

  delay(300);
}


void my_drawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
