/*******************************************************************************
 * This example shows how to use the DPS1 (powerDown) sleep mode and how to    *
 * set up input pins for the best low consumption and how to set up the LVD.   *
 * In this example if LVD enabled with BOD_ON parameter the consumption will   *
 * increase about 12.5 microamps during sleep. The LED will turned on for      *
 * 2 seconds every time it wakes up.                                           *
 *                                                                             *
 * Attention! The input pins voltage level without pullup (or pulldown)        *
 * resistors will be float. At numerous floating input pins can cause the      *
 * sleeping consumption increase up to 900 microamps.                          *
 *                                                                             *
 * Using LGT8F328x in Arduino envinronment LVD (BOD equivalent) not activated  *
 * by default. Therefore in case of unused LVD not needed to turn if off. In   *
 * this case (parameter BOD_ON) the generated code is a bit shorter.           *
 *******************************************************************************
 * LGT8F328D cannot wake up with watchdog timer when mode is DPS0 or DPS1.     *
 *******************************************************************************/

#include "lgt_LowPower.h"

void setup()
{
  // Pin settings for low consumption in sleep mode.
    DDRB  = 0x00;                 // Set all pins to input
    DDRC  = 0x00;
    DDRD  = 0x00;
    DDRE  = 0x00;
    PORTB = 0xFF;                 // Set all input with internal pull up resistors
    PORTC = 0xFF;
    PORTD = 0xFF;
    PORTE = 0xFF;
    pinMode(LED_BUILTIN, OUTPUT); // Visual effect to show program status
    digitalWrite(LED_BUILTIN, LOW);
    VDTCR = 0x80;
    VDTCR = 0x4B;                 // LVD (BOD) enable to 2.5 V for demonstration purpose 
}

void loop() 
{
    delay(2000);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
	
    // Enter power down state for 8 s with ADC and BOD (LVD) modules disabled
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    
    // Do something here
    // Example: Read sensor, data logging, data transmission.
    digitalWrite(LED_BUILTIN, HIGH);
}

