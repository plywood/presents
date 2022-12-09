
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

// Barometric sensor BMP280
Adafruit_BMP280 bmp; // I2C

// RTC CLOCK
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// SD card
#include <SPI.h>
#include <SD.h>
// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
// change this to match your SD shield or module
const int chipSelect = 4;


void setup() {
  //BMP280 setup
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
//DS3231
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //SD Card
  Serial.print("\nInitializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";
  //#####################################
  // get time and print it out to searial monitor
  //#####################################
  DateTime now = rtc.now();
  delay(20);
  Serial.print(now.year(),   DEC);
  Serial.print('/');
  Serial.print(now.month(),  DEC);
  Serial.print('/');
  Serial.print(now.day(),    DEC);
  Serial.print(' ');
  Serial.print(now.hour(),   DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  dataString += String(now.year(),   DEC);
  dataString += String('-');
  dataString += String(now.month(),  DEC);
  dataString += String('-');
  dataString += String(now.day(),    DEC);
  dataString += String(' ');
  dataString += String(now.hour(),   DEC);
  dataString += String(':');
  dataString += String(now.minute(), DEC);
  dataString += String(':');
  dataString += String(now.second(), DEC);
  dataString += String(' ');
  
  //#############################
  //PRESSURE
  //#############################
  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  
  dataString += String(' ');
  dataString += String(bmp.readPressure());
  dataString += String(' ');
  dataString += String('P');
  dataString += String('a');

  
  //##############################
  // Lets log it to the card
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("mydata.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(1000);
}



//The Arduino Map function modified for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
