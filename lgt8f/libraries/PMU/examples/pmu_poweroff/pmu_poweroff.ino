#include <PMU.h>

uint8_t sleepPin = 7;
uint8_t vTmp = 0;

void setup()
{
  Serial.begin(9600);
    
  attachInterrupt(0, donothing, LOW);  // enable EXINT0 for wakeup
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);  // internall pullup

  pinMode(sleepPin, INPUT);
  digitalWrite(sleepPin, HIGH);  // internal pullup 
}

void loop()
{
  Serial.write("hello, I am working!\n");
  delay(10);
  
  // enable power/off 
  if(digitalRead(sleepPin) == 0)
    PMU.sleep(PM_POFFS0);  
}

void donothing()
{
  // dummy function
}
