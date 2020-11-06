/*
 * EEPROM Clear
 *
 * Sets all of the bytes of the EEPROM to 0.
 * This example code is in the public domain.

 */
#define USE_LGT_EEPROM_API
#include <EEPROM.h>

void setup()
{
  
  // write a 0 to all bytes of the EEPROM
  for (int i = 0; i < EEPROM.size(); i++)
    EEPROM.write(i, 0);
    
  // turn the LED on when we're done
  digitalWrite(13, HIGH);
}

void loop()
{
}
