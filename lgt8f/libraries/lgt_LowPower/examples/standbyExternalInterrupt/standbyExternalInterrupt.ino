/*******************************************************************************
 * This example shows how to use the external interrupt pins (INT0 and INT1)   *
 * wake-up capability. The LED state will change when int. pin low detected.   *
 * In sleep mode DPS0 (== powerStandby) both low level and level change        *
 * interruts are available on INT0 and INT1 pins.                              *
 *                                                                             *
 * Using LGT8F328x in Arduino envinronment LVD (BOD equivalent) not activated  *
 * by default. Therefore not needed to turn off it to sleep. In case of BOD_ON *
 * the generated code is shorter a bit.                                        *
 *                                                                             *
 * LGT8F328D cannot wake up with watchdog timer when mode is DPS0 or DPS1.     *
 * SLEEP_FOREVER is recommended.                                               *
 *******************************************************************************
 * In DPS1 (== powerDown) sleep mode the external interrupts can only work in  *
 * low level mode. In this case if the related ISR mode was set to something   *
 * other than LOW then interrupt will not called at wake up.                   *
 *******************************************************************************/

#include "lgt_LowPower.h"

const int wakeUpPin = D2;         // Digital pin 2 == Int 0
volatile boolean intflag;

void anExtISR(void)
{
    intflag = true;
}

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
    pinMode(wakeUpPin, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);// Built-in LED set to low level
	
	
    Serial.begin(9600);
    intflag = false;
    Serial.println();
    Serial.println(F("Sleep and external interrupt example started."));

  // External interrupt on pin (example: press of an active low button)
  // A pullup resistor is used to hold the signal high when no button press
  // In this example the internal resistor is used for to hold the signal high.
}

void loop() 
{
    Serial.println();
    Serial.println(F("Entering standby mode."));
    Serial.println(F("Apply low signal to wake the MCU."));
    Serial.flush();
	
    attachInterrupt(digitalPinToInterrupt(wakeUpPin), anExtISR, LOW); // or FALLING

  // Enter standby mode
  LowPower.powerStandby(SLEEP_32S, ADC_OFF, BOD_ON, TIMER2_OFF);  

    detachInterrupt(digitalPinToInterrupt(wakeUpPin));
    Serial.println(F("Awake!"));
    if (intflag) 
    {
        Serial.println(F("MCU was woken up via an external interrupt pin."));
        intflag = false;
        digitalToggle(LED_BUILTIN);
    }
    delay(4000);
}

