// demo for system clock switch
// using function "sysClock(mode)" to switch
// system clock between internal or external oscillator
// e.g:
//  sysClock(INT_OSC) for internal oscillator
//  sysClock(EXT_OSC) for external crystal 

void setup() {
  // put your setup code here, to run once:
  sysClock(EXT_OSC);

  pinMode(E4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalToggle(E4);
}
