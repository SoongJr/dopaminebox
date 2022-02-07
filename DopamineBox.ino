/*
  Take input from analog pin and activate LEDs based on its value.

  The idea is that the analog input is connected to the output of a voltage divider with 1M on one side
  and multiple resistors in series on the other side.
  Each of those resistors can be bypassed by a different switch, and each resistor's value is
  chosen so that we can determine from the resulting voltage which of them have been bypassed and which haven't.
  So the resistor for switch 0 is chosen so that it, and only it, brings the voltage over half the input voltage (e.g. 1M)
  Then the resistor of switch 1 is chosen so that it, and only it, brings the voltage over half the remaining voltage (e.g. 500k)
  Et cetera, with the caveat that analog values are not accurate, and we also don't have every value of resistor at hand,
  so the resistor values were chosen differently, see the macro definition sector.

  The circuit:
  - analog input A0 is the output of abovementioned voltage divider
  - digital outputs used to activate LEDs (see macro definition section for details)

  created 07 Feb 2022
  by SoongJr

  This example code is in the public domain.

  https://github.com/SoongJr/dopaminebox
*/

#define DEBUG_MODE

#define INPUT_PIN A0
#define SWITCH0_R 1000000 // 1 mega-Ohm
#define SWITCH1_R 470000  // 470 kilo-Ohm
// voltages for each switch are calculated via the voltage divider formula:
#define SWITCH0_VOLT (1024 * (1000000+SWITCH0_R)/SWITCH0_R))
#define SWITCH1_VOLT (1024 * (1000000+SWITCH1_R)/SWITCH1_R))
#define LED0 2
#define LED1 3

void setup()
{
#ifdef DEBUG_MODE
  // start serial connection
  Serial.begin(115200);
#endif
  // configure input pin as an input without pullup resistor
  pinMode(INPUT_PIN, INPUT);
  // configure digital pins for LEDs as output
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  // explicitly disable all LEDs
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
}

// checkSwitch compares the switch-specific value with the input values,
// determines whether the switch is pressed, and sets the LED accordingly
void checkSwitch(int &inputVal, int switchValue, uint8_t led)
{
#ifdef DEBUG_MODE
  // print out the measured value
  Serial.print(inputVal);
#endif
  // check if the voltage is over the expected value for this switch (with a small margin for error)
  if (inputVal >= switchValue - 8)
  {
    digitalWrite(led, HIGH);
    inputVal -= switchValue; // deduct the voltage caused by switch 0 fromt eh input value for further examination
  }
  else
  {
    digitalWrite(led, LOW);
  }
}

void loop()
{
#ifdef DEBUG_MODE
  delay(1000); // delay to make serial output readable
#endif
  // read the analog value into a variable
  int inputVal = analogRead(INPUT_PIN);
  // from the analog value, determine which switches had been pressed
  // (each of them bypasses one identifiable part of the voltage divider circuit, resulting in a voltage between 0V and 2.5V or 0-512)
  // e.g.: switch 0 is responsible for half the possible voltage (so half of 512), and so are all other switches combined.
  //       If it is activated, the voltage cannot be over 1.25V.
  checkSwitch(inputVal, SWITCH0_VOLT, LED0);
  checkSwitch(inputVal, SWITCH1_VOLT, LED1);
}
