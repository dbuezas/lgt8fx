
uint16_t vcc5d1 = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(19200);

  // using internal 1.024V reference
  analogReference(INTERNAL1V024);
}

void loop() {
  // put your main code here, to run repeatedly:
  vcc5d1 = analogRead(VCCM);
  
  // convert to VCC (unit: mV);
  vcc5d1 *= 5;

  Serial.print("Analog Power Supply: = ");
  Serial.print(vcc5d1, DEC);
  Serial.println("mV");

  delay(500);
}
