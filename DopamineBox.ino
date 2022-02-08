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

// #define DEBUG_MODE

// Pins
#define INPUT0 A0
#define INPUT1 A1

class CLed
{
public:
  /// pin: number of digital IO pin
  CLed(uint8_t pin)
  {
    m_pin = pin;
  }

  void setup()
  {
    // configure digital pin as output
    pinMode(m_pin, OUTPUT);
    // explicitly disable the CLed
    digitalWrite(m_pin, LOW);
  }

  /// turns the LED on (HIGH) or off (LOW)
  void set(uint8_t val)
  {
    digitalWrite(m_pin, val);
  }

private:
  uint8_t m_pin;
};
class CSwitch
{
public:
  /// switchPos: position (zero-based) of associated switch in R-2R ladder
  /// led: CLed instance triggered by this switch
  CSwitch(uint8_t position, CLed &led)
  {
    m_led = &led;
    // from the switch position we can calculate the approximate voltage that closing this switch would cause in the output of the R-2R ladder:
    m_switchValue = 1024 / (1 << (position + 1));
  }

  /// compares the switch-specific value with the input values,
  /// determines whether the switch is pressed, and sets the CLed accordingly
  /// warning: this method modifies the inputVal, so it must be called for the different LEDs in the correct order
  void checkInput(int &inputVal)
  {
#ifdef DEBUG_MODE
    // print out the measured value
    Serial.print(inputVal);
    Serial.print(" > ");
    Serial.print(m_switchValue);
    Serial.println("?");
#endif
    // check if the signal exceeds the expected value for this switch (with a small margin for error; on my breadboard it was accurate to within a value of 2, but this may actually be different when the final circuit is soldered)
    if (inputVal + 8 > m_switchValue)
    {
      // switch-specific voltage detected, meaning the switch is pressed and we activate the CLed
      m_led->set(HIGH);
      // also deduct this value from the input value for further examination
      inputVal -= m_switchValue;
    }
    else
    {
      // deactivate the CLed
      m_led->set(LOW);
    }
  }

private:
  CLed *m_led;
  int m_switchValue;
};

// define LEDs and which pin they use
CLed led0(2);
CLed led1(3);
CLed led2(4);
CLed led3(5);
CLed led4(6);
CLed led5(7);
CLed led6(8);
CLed led7(9);
// define switches and which LED they trigger
//      we split up switches over multiple ladders (restarting the position count from 0)
//      because the voltages quickly become so small they fall below the noise level.
//      Depending on circuit and the actual resistors used, 7 switches per input may be possible
//      (but you'd have to adjust the margin that is applied when evaluating the value)
CSwitch switch0(0, led0);
CSwitch switch1(1, led1);
CSwitch switch2(2, led2);
CSwitch switch3(3, led3);
CSwitch switch4(0, led4);
CSwitch switch5(1, led5);
CSwitch switch6(2, led6);
CSwitch switch7(3, led7);

void setup()
{
#ifdef DEBUG_MODE
  // start serial connection
  Serial.begin(115200);
#endif
  // configure input pin as an input without pullup resistor
  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
  // set up LEDs:
  led0.setup();
  led1.setup();
  led2.setup();
  led3.setup();
  led4.setup();
  led5.setup();
  led6.setup();
  led7.setup();
}

void loop()
{
#ifdef DEBUG_MODE
  delay(1000); // delay to make serial output readable
  Serial.println("---");
#endif
  // read the analog value into a variable
  int inputVal;
  inputVal = analogRead(INPUT0);
  // from the analog value, determine which switches are pressed
  switch0.checkInput(inputVal);
  switch1.checkInput(inputVal);
  switch2.checkInput(inputVal);
  switch3.checkInput(inputVal);
  // the second set of four switches is wired to a separate input:
  inputVal = analogRead(INPUT1);
  switch4.checkInput(inputVal);
  switch5.checkInput(inputVal);
  switch6.checkInput(inputVal);
  switch7.checkInput(inputVal);
}
