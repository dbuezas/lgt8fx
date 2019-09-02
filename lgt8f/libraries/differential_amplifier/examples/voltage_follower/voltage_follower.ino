//============================================
// Differential Amplifier demo for LGT8F328x
// by dbuezas
// Using new differential_amplifier library.
// Connect A1 to the voltage source (0..5v) and D4 to A0 with a jumper wire
// Open the Serial Plotter and you'll see how the DAC in D4 will follow the
// voltage read by A1
//============================================
#include "differential_amplifier.h"

#define INPUT_VOLTAGE_PIN A1
#define DAC_PIN DAC0  // D4 in the silk screen
#define DAC_READER_PIN A0

void setup() {
  Serial.begin(230400);
  Serial.println("start");
  analogReference(DEFAULT);  // 5v. If not set, the DAC only gets to 1v
  pinMode(DAC0, ANALOG);     // DAC0 = D4
}

float dac_value = 0;
void loop() {
  if (dac_value < 0) dac_value = 0;
  if (dac_value > 255) dac_value = 255;
  analogWrite(DAC_PIN, (int)dac_value);

  int minus = analogDiffRead(INPUT_VOLTAGE_PIN, DAC_READER_PIN, GAIN_1);
  int plus = analogDiffRead(DAC_READER_PIN, INPUT_VOLTAGE_PIN, GAIN_1);

  float error = minus > plus ? minus : -plus;
  int actual = analogRead(INPUT_VOLTAGE_PIN);
  int followed = analogRead(DAC_READER_PIN);
  Serial.print("\tactual: ");
  Serial.print(actual);
  Serial.print("\tfollowed: ");
  Serial.print(followed);
  Serial.println();

  dac_value -= error / 1000;
  delay(10);
  return;
}