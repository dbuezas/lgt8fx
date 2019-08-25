//============================================
// Larduino w/ 328D
// DAC1 output demo 
// DAC1 output ==> PE5 on board
//============================================

unsigned char value = 0;

void setup() {
  // put your setup code here, to run once:
  analogReference(INTERNAL2V56);
  pinMode(DAC1, ANALOG);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(DAC1, value);
  delay(10);
  value += 10;
}

