// Analog Input can be used to detect a touch of our finger.
// and it works wery well with a charged plastic pen


int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;       // select the pin for the LED
int sensorValue = 0;   // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  
  // turn on and off led upon sensor value
  if (sensorValue > 400) {
    digitalWrite(ledPin, HIGH);
  } else {
    // turn the ledPin off:
    digitalWrite(ledPin, LOW);
  }
  delay(50);
}
