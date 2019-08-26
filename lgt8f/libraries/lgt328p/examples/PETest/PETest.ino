
void setup() {
  // put your setup code here, to run once:
  pinMode(E0, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(E3, OUTPUT);
  pinMode(E4, OUTPUT);
  pinMode(E5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalToggle(E0);
  digitalToggle(E1);
  digitalToggle(E2);
  digitalToggle(E3);
  digitalToggle(E4);
  digitalToggle(E5);     
}

