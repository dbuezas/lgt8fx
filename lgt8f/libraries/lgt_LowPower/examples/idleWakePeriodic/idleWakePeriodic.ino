/*******************************************************************************
 * This example shows how the Idle mode works. ( In short the Idle mode stops  *
 * the core clock for a while. )                                               *
 *                                                                             *
 * Other modules can be disabled with LowPower.idle() function but be aware    *
 * the state of the disabled peripheral modules may be lost. After waking up   *
 * the relevant modules should be reinitialized to ensure proper operation.    *
 *                                                                             *
 * - You should pay attention to the fact that when Timer 0 not disabled the   *
 *   MCU will wake up within one miliseconds due the Timer 0 interrupt. It's   *
 *   because in Arduino evonronment Timer 0 used for measure milliseconds.     *
 * - You should also pay attention to that after you read analalog input with  *
 *   ADC ( analogRead() ) the intrerrupt flag of the ADC ( ADIF ) is set and   *
 *   this is what causes the MCU wakes up immediately from any sleep mode. The *
 *   solution is to clear ADIF bit in ADCSRA.                                  *
 * - The FLASH controller is used to control the EEPROM read/write. (FLASHCTL) *
 *******************************************************************************/

#include <lgt_LowPower.h>

void setup()
{
    // No setup is required for LowPower library

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial.println("Started.");
    Serial.flush();
}

void loop() 
{
    // It is recommended to clear ADC interrupt and start conversion flag before sleep.
    ADCSRA |=  ( 1<<ADIF );
    ADCSRA &= ~( 1<<ADSC );

    // Enter idle state for 8 s with a lot of peripherals turned off
    // Don't forget to leave enabled the module you have in sleep to wait for.
    // LGT8Fx8P
    LowPower.idle(SLEEP_4S, ADC_ON, TIMER3_OFF, TIMER2_ON, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_ON, TWI_OFF, PCIC_OFF, FLASHCTL_OFF);

    // Do something here
    // Example: Read sensor, data logging, data transmission.
    digitalToggle(LED_BUILTIN);
}
