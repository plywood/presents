// this code read analog value and prints it.
// use serial plotter to see results
// try to touch A0 pin and V pin at the same time or move something electrostatically charged near arduino.

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(50);
}
