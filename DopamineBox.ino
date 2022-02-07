/*
  Clone of Input Pull-up Serial example

  This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital
  input on digital pin 13 and sets the built-in LED accordingly (also sending result via serial port).

  The circuit:
  - jumper from pin 13 to ground
  - built-in LED on pin 13

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
  20K-ohm resistor is pulled to 5V. This configuration causes the input to read
  HIGH when the switch is open, and LOW when it is closed.

  created 14 Mar 2012
  by Scott Fitzgerald

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/InputPullupSerial
*/

#define INPUT_PIN 13

void setup()
{
  // start serial connection
  Serial.begin(115200);
  // configure pin 13 as an input and enable the internal pull-up resistor
  pinMode(INPUT_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Starting program...");
}

void loop()
{
  // delay(1000);
  // read the pushbutton value into a variable
  // (for some reason this scwitches between HIGH and LOW on every read, maybe something is iterfereing with Pin 13)
  int sensorVal = digitalRead(INPUT_PIN);
  // print out the value of the pushbutton
  Serial.print(sensorVal);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  digitalWrite(LED_BUILTIN, sensorVal == HIGH ? LOW : HIGH);
}
