#include <Arduino.h>
#include <Wire.h>
#include <math.h> 
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
// include the SD library:
#include <SPI.h>
#include <SD.h>

/* SD card code is from SD library Datalogger
 ** MOSI - pin 11      Master Out Slave In
 ** MISO - pin 12      Master In Slave Out
 ** CLK  - pin 13      Clock
 ** CS   - pin 10      Chip Select
 ** VCC  - RED - 5V
 ** GND - BLACK - GND

Data can be processed and plots created using gnuplot https://sourceforge.net/projects/gnuplot/files/gnuplot/5.2.5/
gnuplot> load 'gnu_script.txt'

*/
const int chipSelect = 10;

// HUMIDITY + TEMP + BAROMETRIC PRESSURE sensor BME280
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

float temperature;
unsigned long delayTime;

// RTC CLOCK
#include "RTClib.h"
RTC_DS1307 rtc;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Serial OK");
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif

  // make sure that the default chip select pin is set to be an output, even if you don't use it:
  pinMode(    10, OUTPUT);
  //Initialize CRTC CLOCK  
  rtc.begin();
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running. time will be adjusted");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("RTC OK");
  
  //########################
  // Set up SD card
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
    Serial.println(F("BME280 test"));

    unsigned status;
    
    // default settings
    status = bme.begin(0x76);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    
    Serial.println("-- BME OK  --");
    delayTime = 1000;
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
  //TEMPPERATURE
  //#############################
  Serial.print("Temperature = ");
  temperature = bme.readTemperature();
  Serial.println(temperature);
  dataString += String(temperature);
  //##############################
  // Lets log it to the card
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
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
  delay(10000);
}

//The Arduino Map function modified for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
