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
*/
// the following is for SD card
const int chipSelect = 10;

// RTC CLOCK
#include "RTClib.h"
RTC_DS1307 rtc;

//FSR thin film force sensor
unsigned int fsr_value;



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

  dataString += String(now.year(),   DEC);
  dataString += String('-');
  dataString += String(now.month(),  DEC);
  dataString += String('-');
  dataString += String(now.day(),    DEC);
  dataString += String(',');
  dataString += String(now.hour(),   DEC);
  dataString += String(':');
  dataString += String(now.minute(), DEC);
  dataString += String(':');
  dataString += String(now.second(), DEC);
  dataString += String(',');
  
  //#############################
  // Thin film force sensor
  //#############################
  
  fsr_value = analogRead(0);
  
  dataString += String(fsr_value);
  
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
  delay(1000);
}
