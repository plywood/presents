// This sketch is for 8 pixel led strip
// It demonstrates conversion of analog input in to a bar graph display

#include <Adafruit_NeoPixel.h>

#define PIN       6 // Pin on the Arduino that is connected to the NeoPixels
#define NUMPIXELS 8 // How many NeoPixels are attached to the Arduino?

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
////// 
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;   // variable to store the value coming from the sensor
int led_index = 0; 

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
   pixels.clear(); // Set all pixel colors to 'off'
}

void loop() {
    // analog AD converter is 0 - 1023 
    // read the value from the sensor and store it in to a variable:
    sensorValue = analogRead(sensorPin);
    
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // we are going to light up the LED corresponding to analog output
    led_index = map(sensorValue, 0, 1023, 0, 7);
    
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      if (led_index == i ) {
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      } else {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
    }
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop

}
