#include <Arduino.h>
#include <Wire.h>
#include <math.h> 
// include the SD library:
#include <SPI.h>
#include <SD.h>

/* SD card code is from SD library Datalogger
 ** MOSI - pin 11      Master Out Slave In
 ** MISO - pin 12      Master In Slave Out
 ** CLK  - pin 13      Clock
 ** CS   - pin 10      Chip Select
Data can be processed and plots created using gnuplot https://sourceforge.net/projects/gnuplot/files/gnuplot/5.2.5/
gnuplot> load 'gnu_script.txt'
*/
const int chipSelect = 10;

// Light Sensors
//BH1750 I2C Address
int Sen1 = 0x23;
//Define the string for datastream ID
char sensor1[] = "Sen1";
byte buff[2];

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
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
  //Luminosity 
  //#############################
   int i;
  uint16_t val1=0, val2=0;
  //send signal to sensor 1
  BH1750_Init(Sen1);
  delay(100);
  
  if(2==BH1750_Read(Sen1)){
    val1 = ((buff[0]<<8)|buff[1])/1.2;
  }
  delay(100);
  Serial.print("Lumens:");
  Serial.println(val1);
  
  dataString += String(val1);
  
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

//SUBROUTINES BELOW
//custom parameter for request values from sensor
int BH1750_Read(int address) {
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address,2);
  while(Wire.available()) {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  return i;
}
//custom parameter for receive values from sensor
void BH1750_Init(int address) {
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}

//The Arduino Map function modified for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
