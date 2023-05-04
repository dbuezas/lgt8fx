/*******************************************************************************
 * This example shows how to use this library together with WDT.               *
 *                                                                             *
 * The lgt_LowPower library is using WDT for the timing of the sleep period.   *
 * This is the reason why required to reinitialize the WDT after waking up.    *
 *******************************************************************************
 * LGT8F328D cannot wake up with watchdog timer when mode is DPS0 or DPS1.     *
 *******************************************************************************/
 
#include "WDT.h"
#include "lgt_LowPower.h"

void setup()
{
    // Your setup code 

    wdtstart();
}

void loop() 
{
    // Do something here what you need for (which takes a long time)
    int n = 0;
    do {
        delay(999);
        wdt_reset();     // Restart WDT timer regurarly
    } while( n++ < 30 );

    pinsettingsforSleep();

    // Enter power down state for 8 s with ADC and BOD (LVD) modules disabled
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    // Restart WDT after wake-up event happened
    wdtstart();
    
    // Do something here too
    // like read sensors, data logging, data transmission or something else.
}

void wdtstart()
{
    wdt_enable(WTO_1S);
}

void pinsettingsforSleep()
{
  // Example pin settings for low consumption in sleep mode.
    DDRB  = 0x00;                 // Set all pins to input
    DDRC  = 0x00;
    DDRD  = 0x00;
    DDRE  = 0x00;
    PORTB = 0xFF;                 // Set all input with internal pull up resistors
    PORTC = 0xFF;
    PORTD = 0xFF;
    PORTE = 0xFF;
    pinMode(LED_BUILTIN, OUTPUT); // Set LED level to reduce sleep consumption
    digitalWrite(LED_BUILTIN, LOW);
}

