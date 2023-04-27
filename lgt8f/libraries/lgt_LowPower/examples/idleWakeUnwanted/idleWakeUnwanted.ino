/******************************************************************************
 * This example shows how to avoid waking up due to unwanted events in Idle   *
 * or Noise Reduction sleep modes.                                            *
 * At first the periodic wake-up happens more often than we would like due to *
 * serial port activity. After uncommenting lines containing Serial.flush()   *
 * sleep will periodically wait 4 seconds.                                    *
 ******************************************************************************/

#include <lgt_LowPower.h>

uint16_t z;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial.println();
    Serial.println(F("The sketch started."));
//  Serial.flush();
}

void loop() 
{

    LowPower.idle(SLEEP_4S, ADC_ON, TIMER3_OFF, TIMER2_ON, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_ON, TWI_OFF, PCIC_OFF, FLASHCTL_OFF);

    digitalToggle(LED_BUILTIN);
    Serial.print(F(" Wake-up number: "));
    Serial.println(++z);
//  Serial.flush();
}