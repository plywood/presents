/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

/*
HOW TO CHANGE RESOLUTION
the symptom is that on your OLED every other line is not lit.
1. Download Nedit or Notepad++
2. Find a file named Adafruit_SSD1306.h
It should be in one of the adafruit libraries. On windows it will be here: 
C:\Users\YOURUSERNAME\Documents\Arduino\libraries\Adafruit_SSD1306
3. Download Nedit programming editor.
4. With Nedit open Adafruit_SSD1306.h
5. uncomment line 73 #define SSD1306_128_64
6. Comment out other resolutions
It should look like this:

#define SSD1306_128_64
//   #define SSD1306_128_32
//   #define SSD1306_96_16

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// http://forum.hobbycomponents.com/viewtopic.php?f=73&t=1781&hilit=hcmodu0079
/* Used to store the key state */
int Key;
#define SCL_PIN 8   // that is for the keypad
#define SDO_PIN 9   // that is for the keypad
#define SHAKE  2    // that is for the motor

#define trigPin 13  // that is for distance sensor HC-SR04
#define echoPin 12  // that is for distance sensor 

long duration, distance;

void setup()   {                
  //Serial.begin(9600);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello World" );
  display.display();
  delay(2000);
  display.clearDisplay();
  // init display done
  delay(1000);
  pinMode(SCL_PIN, OUTPUT); 
  pinMode(SDO_PIN, INPUT);

  pinMode(SHAKE, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  
  
}


void loop() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  // Read the current state of the keypad
  Key = Read_Keypad();
  // If a key has been pressed output it to the serial port 
  if (Key) {
    
    if (Key == 1)  { display.println("Key 1" ); }
    if (Key == 2)  { display.println("Key 2" ); }
    if (Key == 3)  { display.println("Key 3" ); }
    if (Key == 4)  { display.println("Key 4" ); }
    if (Key == 5)  { display.println("Key 5" ); } 
    if (Key == 6)  { display.println("Key 6" ); }
    if (Key == 7)  { display.println("Key 7" ); }
    if (Key == 8)  { display.println("Key 8" ); }
    if (Key == 9)  { display.println("Key 9" ); }
    if (Key == 10) { display.println("Key 10"); }
    if (Key == 11) { display.println("Key 11"); }
    if (Key == 12) { display.println("Key 12"); }
    if (Key == 13) { display.println("Key 13"); }
    if (Key == 14) { display.println("Key 14"); }
    if (Key == 15) { display.println("Key 15"); }
    if (Key == 16) { display.println("Key 16"); }
    digitalWrite(SHAKE, HIGH);
    delay(100);
    digitalWrite(SHAKE, LOW);
  }

  // distance measure portion
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  display.setCursor(0,30);
  display.setTextSize(2);
  display.print("L = ");
  display.print(distance);
  display.display();
  delay(100);
}
/* Read the state of the keypad */
int Read_Keypad(void) {
  int Count;
  int Key_State = 0;
  /* Pulse the clock pin 16 times (one for each key of the keypad) and read the state of the data pin on each pulse */
  for(Count = 1; Count <= 16; Count++) {
    digitalWrite(SCL_PIN, LOW);
    /* If the data pin is low (active low mode) then store the current key number */
    if (!digitalRead(SDO_PIN)) {
      Key_State = Count;
    }
    digitalWrite(SCL_PIN, HIGH);
  } 
  return Key_State;
}


