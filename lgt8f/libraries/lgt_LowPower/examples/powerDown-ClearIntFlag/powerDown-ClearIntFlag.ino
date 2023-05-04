/*******************************************************************************
 * This example shows how an unhandled interrupt flag can distract a sleep.    *
 * If an event occurred that caused an interrupt flag(s) is set this flag(s)   *
 * must be cleared before any sleep mode called. Otherwise the CPU immediately *
 * wakes up.                                                                   *
 *                                                                             *
 * The case of the ADC interrupt flag shows how to proceed in such cases.      *
 *******************************************************************************
 * LGT8F328D cannot wake up with watchdog timer when mode is DPS0 or DPS1.     *
 *******************************************************************************/

#include "lgt_LowPower.h"

    int avalue;
    int measuredpin = A0;

void setup()
{
    // No setup is required for LowPower library

    analogReference(INTERNAL1V024);
//  analogReadResolution(12);
}

void loop() 
{
    // Doing something here.
    avalue = analogRead(measuredpin);
    if (avalue > 500)
        { digitalWrite(LED_BUILTIN, HIGH); }
    else	
        { digitalWrite(LED_BUILTIN, LOW); }

    // The previous analogRead() set the ADIF flag.
    // Clear ADC interrupt flag before sleep to avoid immediately wake-up.
    ADCSRA |=  (1 << ADIF);

    pinsettingsforSleep();
    LowPower.powerDown(SLEEP_8S, ADC_ON, BOD_ON);

    // Do something here. Example: Read sensor, data logging, data transmission.
    digitalToggle(LED_BUILTIN);
    delay(200);
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

