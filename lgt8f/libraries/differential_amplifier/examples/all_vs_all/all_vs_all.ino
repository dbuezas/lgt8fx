//============================================
// Differential Amplifier demo for LGT8F328x
// by dbuezas
// Using new differential_amplifier library.
// Differential read from all analog pin combinations
//============================================
#include "differential_amplifier.h"

#define GAIN GAIN_1
// #define GAIN GAIN_8
// #define GAIN GAIN_16
// #define GAIN GAIN_32

void setup() {
  Serial.begin(230400);
  Serial.println("start");
  // analogReference(DEFAULT);  // 5v
}
void loop() {
  static byte list[] = {A0, A1, A2, A3, A4, A5, A6, A7};
  static const byte list_length(sizeof(list) / sizeof(list[0]));
  Serial.print("-\\+\t");
  for (int pos = 0; pos < list_length; pos++) {
    Serial.print("A");
    Serial.print(pos);
    Serial.print("\t");
  }
  Serial.println();
  for (int neg = 0; neg < list_length; neg++) {
    Serial.print("A");
    Serial.print(neg);
    Serial.print("\t");
    for (int pos = 0; pos < list_length; pos++) {
      int value = analogDiffRead(list[neg], list[pos], GAIN);
      if (neg == pos) {
        Serial.print("x");
      } else if (value == -1) {
        Serial.print(".");  // -1 means pin combination not available
      } else {
        Serial.print(value);
      }
      Serial.print("\t");
    }
    Serial.println();
  }
  Serial.println("--------");
  delay(100);
}
