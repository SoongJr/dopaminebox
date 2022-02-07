/*
  Take input from analog pin and activate LEDs based on its value.

  The state of all switches (active/pressed or not, 1 or 0) is "encoded" into a single analog signal
  using an R-2R resistor ladder.

  The circuit:
  - analog input A0 is the output of above-mentioned resistor ladder
  - digital outputs used to activate LEDs (see macro definition section for details)

  created 07 Feb 2022
  by SoongJr

  This example code is in the public domain.

  https://github.com/SoongJr/dopaminebox
*/

#define DEBUG_MODE

// Pins
#define INPUT_PIN A0
#define LED0 2
#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define LED5 7
#define LED6 8
#define LED7 9
// signal levels for each switch:
// (switch 7 results in a difference of only 4 in the analog signal, that's probably already below the noise level and any more would be a waste)
#define SWITCH0_SIGNAL (1024 / (1 << 1))
#define SWITCH1_SIGNAL (1024 / (1 << 2))
#define SWITCH2_SIGNAL (1024 / (1 << 3))
#define SWITCH3_SIGNAL (1024 / (1 << 4))
#define SWITCH4_SIGNAL (1024 / (1 << 5))
#define SWITCH5_SIGNAL (1024 / (1 << 6))
#define SWITCH6_SIGNAL (1024 / (1 << 7))
#define SWITCH7_SIGNAL (1024 / (1 << 8))

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
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  // explicitly disable all LEDs
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
}

// checkSwitch compares the switch-specific value with the input values,
// determines whether the switch is pressed, and sets the LED accordingly
void checkSwitch(int &inputVal, int switchValue, uint8_t led)
{
#ifdef DEBUG_MODE
  // print out the measured value
  Serial.print("switch ");
  Serial.print(led - LED0);
  Serial.print(": ");
  Serial.print(inputVal);
  Serial.print(" > ");
  Serial.print(switchValue);
  Serial.println("?");
#endif
  // check if the signal exceeds the expected value for this switch (with a small margin for error)
  if (inputVal > switchValue - 8)
  {
    // switch-specific voltage detected, meaning the switch is pressed and we activate the LED
    digitalWrite(led, HIGH);
    // also deduct this value from the input value for further examination
    inputVal -= switchValue;
  }
  else
  {
    // deactivate the LED
    digitalWrite(led, LOW);
  }
}

void loop()
{
#ifdef DEBUG_MODE
  delay(1000); // delay to make serial output readable
  Serial.println("---");
#endif
  // read the analog value into a variable
  int inputVal = analogRead(INPUT_PIN);
  // from the analog value, determine which switches had been pressed
  checkSwitch(inputVal, SWITCH0_SIGNAL, LED0);
  checkSwitch(inputVal, SWITCH1_SIGNAL, LED1);
  checkSwitch(inputVal, SWITCH2_SIGNAL, LED2);
  checkSwitch(inputVal, SWITCH3_SIGNAL, LED3);
  checkSwitch(inputVal, SWITCH4_SIGNAL, LED4);
  checkSwitch(inputVal, SWITCH5_SIGNAL, LED5);
  checkSwitch(inputVal, SWITCH6_SIGNAL, LED6);
  checkSwitch(inputVal, SWITCH7_SIGNAL, LED7);
}
