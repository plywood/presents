// This is a minimalistic code to read and save value off of an analog sensor. 
 
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;   // variable to store the value coming from the sensor

void setup() {
}

void loop() {
  // read the value from the sensor and store it in to a variable:
  sensorValue = analogRead(sensorPin);
}