
uint16_t adc_value;

void setup() {
  //sysClock(EXT_OSC);
  
  // put your setup code here, to run once:
  pinMode(E4, OUTPUT);

  // set analog reference to internal 1.024V
  analogReference(INTERNAL1V024);
  
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  // toggle PE0 
  digitalToggle(E4);

  // read ADC channel 11 (ADC11/PE7)
  adc_value = analogRead(A11);
  
  Serial.print("ADC channel 11 := ");
  Serial.print(adc_value, DEC);
  Serial.println("mV");
  
}
