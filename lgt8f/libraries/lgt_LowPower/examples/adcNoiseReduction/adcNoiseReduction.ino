/*******************************************************************************
 * This example shows how to use Noise Reduction mode. ( In short Noise        *
 * Reduction mode stops the core and most of the pheripherials clocks and      *
 * an ADC conversion starts automatically. )                                   *
 * The example periodically read A0 input. The sleep time is shorter than the  *
 * specified because the ADC wakes it up at the end of the conversion. This    *
 * library clears the WDT immediately after waking up.                         *
 * This example also show how to clear ADIF flag before entering sleep mode.   *
 *                                                                             *
 * - You should pay attention to that reading any analalog input with ADC      *
 *  ( analogRead() ) the intrerrupt flag of the ADC ( ADIF ) is set.           *
 *   This is causes the MCU wakes up immediately from any sleep mode. To avoid *
 *   waking up immediately must clear the ADIF bit in ADCSRA before sleep.     *
 *******************************************************************************/

#include "lgt_LowPower.h"

    int avalue;
    int measuredpin = A0;

void setup()
{
    // No setup is required for LowPower library
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(measuredpin,INPUT);
    analogReference(INTERNAL1V024);
//  analogReadResolution(12);
    avalue = analogRead(measuredpin);  // Sets the ADMUX for the later use
}

void loop() 
{

    // Clear ADC interrupt flag before sleep guarantees waiting until the
    // conversion is completed.
    ADCSRA |=  (1 << ADIF);

    // Enter idle state for 8 s with the rest of peripherals turned off
    // LGT8Fx8P
    LowPower.adcNoiseReduction(SLEEP_8S, ADC_ON, TIMER2_OFF);

    // Do something here. Example: Read sensor, data logging, data transmission.
    digitalToggle(LED_BUILTIN);

    // Reading directly the ADC register the resolution will always 12 bits
    // regardless of what was set with the analogReadResolution().

    avalue = ADC; // Reading the result made while sleeping.
    avalue -= (avalue >> 7); // Gain-error correction of LGT8F328P

    Serial.print(F("Measued value: "));
    Serial.print(avalue/4);
    Serial.println(F(" mV"));

    // Waiting for the end of data transmission. The Noise Reduction sleep mode
    // stops serial port during next ADC conversion.
    Serial.flush();
}
