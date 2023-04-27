#include "lgt_LowPower.h"

void setup()
{
    // No setup is required for LowPower library
	
    DDRB  = 0x00;                 // Set all pins to input
    DDRC  = 0x00;
    DDRD  = 0x00;
    DDRE  = 0x00;
    PORTB = 0xFF;                 // Set all input with internal pull up resistors
    PORTC = 0xFF;
    PORTD = 0xFF;
    PORTE = 0xFF;
    pinMode(LED_BUILTIN, OUTPUT); // Visual effect to show program status
    digitalWrite(LED_BUILTIN, HIGH);
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