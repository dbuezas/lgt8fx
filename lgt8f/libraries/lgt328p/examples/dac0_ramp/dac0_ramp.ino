//============================================
// Larduino w/ 328D
// DAC0 output demo
// DACO output ==> D4 on board
//============================================

unsigned char value = 0;

void setup() {
  // put your setup code here, to run once:
  analogReference(INTERNAL2V56);
  pinMode(4, ANALOG);

  analogWrite(4, 255);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(4, value);
  delay(10);
  value += 10;
}
