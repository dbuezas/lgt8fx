void setup() {
  // put your setup code here, to run once:
  pinMode(F0, OUTPUT);
  pinMode(F1, OUTPUT);
  pinMode(F2, OUTPUT);
  pinMode(F3, OUTPUT);
  pinMode(F4, OUTPUT);
  pinMode(F5, OUTPUT);
  pinMode(F6, OUTPUT);
  pinMode(F7, OUTPUT);    

  pinMode(B6, OUTPUT);
  pinMode(C7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalToggle(B6);
  digitalToggle(C7);    
  digitalToggle(F0);
  digitalToggle(F1);
  digitalToggle(F2);
  digitalToggle(F3);
  digitalToggle(F4);
  digitalToggle(F5);
  digitalToggle(F6);
  digitalToggle(F7);
}
