// =======================================================
// Larduino HSP for LGT8FX8D/P series
// Fast.IO demostration
// =======================================================
// Important note:
//  fast_io interface is implemented by macro definition
//  Compare to arduino's digital i/o interface, fast_io 
//  is small and fast. 
//  But fast_io has some limitations:
//    1. only support digital mode i/o operation;
//    2. fastio DO NOT support avriable as arguments!
//       e.g, you can not use fastio as following:
//          int led = 10;
//          fastioMode(led, OUTPUT);
//       but you can use it like this:
//          #define led 10
//          fastioMode(led, OUTPUT);
//      or  fastioMode(10, OUTPUT); 
//      or  fastioMode(D10, OUTPUT);
// ========================================================

void setup() {
  // put your setup code here, to run once:
  fastioMode(D2, INPUT_PULLUP);

  fastioMode(D5, OUTPUT);
  fastioMode(D6, OUTPUT);

  fastioWrite(D5, HIGH);
  fastioWrite(D6, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  fastioWrite(D5, HIGH);
  fastioToggle(D6);
  fastioWrite(D5, LOW);

  // stop here while will pull D2 to ground!
  while (fastioRead(D2) == LOW);
}
