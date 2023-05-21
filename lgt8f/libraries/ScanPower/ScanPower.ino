/* Scanning on-chip devices under power (before sleep) for LGT8F328P or ATmega328P */

#include "ScanPower.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(8000);                     //wait 8 sec
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);                      //wait more 1/2 sec before...

  detectpowered();
}

void loop() {}

void detectpowered() {

  uint8_t o_prr, o_ucsrb;
  uint8_t c;
  uint16_t regaddr;
  boolean on;

  o_prr = PRR;
  o_ucsrb = UCSR0B;

  // Holtek USB serial works with this speed if Serial.begin called several seconds after reset.
  // Need to reset the speed after done?
  PRR &= ~ (1 << PRUART0);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Detecting powered modules..."));
  Serial.println();
  c = 0;
  while (( regaddr = prw(&cdevices[c].raddress) ) > 0) {
    on = false;
    if ((regaddr == &PRR ) && (prb(&cdevices[c].pmask) == 0x02)) {
      on = (o_prr & prb(&cdevices[c].pmask)) == 0 ;
    } else {
      on = ((*((unsigned char*)regaddr)) & prb(&cdevices[c].pmask)) == 0 ;
	  }
    if (on) {
      Serial.print((__FlashStringHelper*)cdevices[c].rname);
      Serial.println(F(" clock is on."));
    }
    c++;
  }
  Serial.println();
  c = 0;
  while (( regaddr = prw(&devices[c].raddress) ) > 0) {
    on = false;
    if (regaddr != &UCSR0B) {
      on = (*((unsigned char*)regaddr)) & prb(&devices[c].pmask);
      on = ((regaddr == &ACSR) || (regaddr == &C1SR)) ? !on : on ;
    } else {
      on = o_ucsrb & prb(&devices[c].pmask);
    }
    if (on) {
      Serial.print((__FlashStringHelper*)devices[c].rname);
      Serial.println(F(" is on."));
    }
    c++;
  }
  Serial.println();
  Serial.println("Done.");
  Serial.println();

  delay(500);       //Wait for empty USART buff
  UCSR0B = o_ucsrb;
  PRR = o_prr;
  
}

