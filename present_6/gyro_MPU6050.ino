//Accelerometer code is from https://create.arduino.cc/projecthub/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19
//stuff for the accelerometer
#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//things for the display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
int x=32;
int y=16;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Accelerometer + Gyro");
  display.display();
  delay(1000);
  display.clearDisplay();
  
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
  Serial.print("Accelerometer: ");
  Serial.print("X = ");       Serial.print(AcX);
  Serial.print(" | Y = ");    Serial.print(AcY);
  Serial.print(" | Z = ");    Serial.println(AcZ); 
  
  Serial.print("Gyroscope    : ");
  Serial.print("X = ");       Serial.print(GyX);
  Serial.print("  | Y = ");   Serial.print(GyY);
  Serial.print("   | Z = ");  Serial.println(GyZ);
  Serial.println(" ");

// now lets place something on the screen and move it around upon accelerometer values 
// change coordinates upon acceleration
if(AcX >= 0) { x=x+1; } 
if(AcX <  0) { x=x-1; }
if(AcY >= 0) { y=y+1; }
if(AcY <  0) { y=y-1; }
// limit coordinates to the size of the screen
if(x<=0)  {x=0;}
if(x>=64) {x=64;}
if(y<=0)  {y=0;}
if(y>=32) {y=32;}

display.setCursor(x,y);
display.print("x");
display.display(); 
delay(100);

}

