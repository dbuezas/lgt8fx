//============================================
// ADC demo for Larduino w/ LGT8F328D
// Using new added internal 2.56V reference
//============================================

uint16_t value;

void setup() {
  // put your setup code here, to run once:
  analogReference(INTERNAL2V56);

  Serial.begin(19200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(VCCM);

  Serial.println(value);
  delay(1);
}
