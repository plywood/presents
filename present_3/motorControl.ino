/*
  Motor Control by Brendan Russell
  Something is wrong with this program, the motor is almost always on.
  I want the motor to only turn on when I move the joystick away from the center.
  Can you help me fix it?

  Hint: you may want to open the serial monitor to see what is happening.
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // start serial communication so you can see the values in the serial monitor
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  // set the integer joyStickValue to the value from the joystick
  int joyStickValue = analogRead(A0);
  // if it is greater than 450 turn on the motor
  if (joyStickValue > 450) {
    digitalWrite(LED_BUILTIN, HIGH); 
    // if it is not greater than 450 turn off the motor
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  // send the joystick value over usb to serial monitor
  Serial.println(joyStickValue);
  // delay for 10 miliseconds
  delay(10);
}
