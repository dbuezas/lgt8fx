/*******************************************************************************
* lgt_LowPower Library
* Version: 2.82
* Date: 28-04-2023
* Author: Lim Phang Moh
* Company: Rocket Scream Electronics
* Website: www.rocketscream.com
* Latest maintainer=github.com/LaZsolt
*
* This is a low power library for LGT Arduino.
*
* This library is licensed under Creative Commons Attribution-ShareAlike 3.0
* Unported License.
*
* Revision  Description
* ========  ===========
* 2.82      28-04-2023. Adapted for LGT8F328x.
*           SLEEP_FOREVER now disabling WDT.
*           Removed other MCUs support for a bit transparent source.
*           Sorry. There is no working ISR available.
*           Restore PRR registers after wake-up.
* 1.81      Fixed timer 2 settings incorrectly restored after sleep.
*           Contributed by rwared11.
* 1.80      Added support for ATmega88 and ATmega168P. PowerExtStandby()
*           modified because not supported on Atmega88 / Atmega168
*           Contributed by mrguen.
* 1.70      Added support for ATmega644P and ATmega1284P.
*           Contributed by alexreinert.
* 1.60      Added support for ATmega256RFR2. Contributed by Rodmg.
* 1.50      Fixed compiler optimization (Arduino IDE 1.6.x branch) on BOD enable
*           function that causes the function to be over optimized.
* 1.40      Added support for ATSAMD21G18A.
*           Library format compliant with Arduino IDE 1.5.x.
* 1.30      Added support for ATMega168, ATMega2560, ATMega1280 & ATMega32U4.
*           Tested to work with Arduino IDE 1.0.1 - 1.0.4.
* 1.20      Remove typo error in idle method for checking whether Timer 0 was
*           turned off.
*           Remove dependecy on WProgram.h which is not required.
*           Tested to work with Arduino IDE 1.0.
* 1.10      Added #ifndef for sleep_bod_disable() for compatibility with future
*           Arduino IDE release.
* 1.00      Initial public release.
*******************************************************************************/


 #include <avr/sleep.h>
 #include <avr/wdt.h>
 #include <avr/power.h>
 #include <avr/interrupt.h>

#include "Arduino.h"
#include "lgt8fx_sleep.h"
#include "lgt8fx_power.h"
#include "lgt_LowPower.h"


#if defined (__AVR__)
// Only Pico Power devices can change BOD settings through software
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__) && ! defined(__LGT8F__)
#ifndef sleep_bod_disable
#define sleep_bod_disable()                                         \
do {                                                                \
  unsigned char tempreg;                                            \
  __asm__ __volatile__("in %[tempreg], %[mcucr]" "\n\t"             \
                       "ori %[tempreg], %[bods_bodse]" "\n\t"       \
                       "out %[mcucr], %[tempreg]" "\n\t"            \
                       "andi %[tempreg], %[not_bodse]" "\n\t"       \
                       "out %[mcucr], %[tempreg]"                   \
                       : [tempreg] "=&d" (tempreg)                  \
                       : [mcucr] "I" _SFR_IO_ADDR(MCUCR),           \
                         [bods_bodse] "i" (_BV(BODS) | _BV(BODSE)), \
                         [not_bodse] "i" (~_BV(BODSE)));            \
} while (0)
#endif
#endif

#if ! defined (__LGT8F__)

#define lowPowerBodOn(mode) \
do {                        \
      set_sleep_mode(mode); \
      cli();                \
      sleep_enable();       \
      sei();                \
      sleep_cpu();          \
      sleep_disable();      \
      sei();                \
} while (0)

// Only Pico Power devices can change BOD settings through software
  #if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168PB__) || defined (__AVR_ATmega328PB__)

#define lowPowerBodOff(mode)\
do {                        \
      set_sleep_mode(mode); \
      cli();                \
      sleep_enable();       \
        sleep_bod_disable();\
      sei();                \
      sleep_cpu();          \
      sleep_disable();      \
      sei();                \
} while (0)

  #endif
  
#else

#ifndef WDT_h
static __inline__ __attribute__ ((__always_inline__))
void wdt_ienable (const uint8_t value)
{
  __asm__ __volatile__ (
        "in     __tmp_reg__,__SREG__    \n\t"
        "cli                            \n\t"
        "wdr                            \n\t"
        "sts    %[wdtcsr], %[wdce_wde]  \n\t"
        "out    __SREG__,__tmp_reg__    \n\t"
        "sts    %[wdtcsr], %[wdie_val]  \n\t"
        : /* no outputs */
        : [wdtcsr]   "n" (_SFR_MEM_ADDR(WDTCSR)),
          [wdce_wde] "r" ((uint8_t)((1 << WDCE) | (1 << WDE))),
          [wdie_val] "r" ((uint8_t) ((1 << WDIE) | (value & 0x08 ? 0x20 : 0x00) | (value & 0x07)))
        : "r0"
  );
}
#endif

#define sleep_enable_m(mode)\
do {                        \
      SMCR = mode | 1;      \
} while (0)

#define lowPower_(mode)     \
do {                        \
      cli();                \
      sleep_enable_m(mode); \
      sleep_cpu();          \
      _NOP();               \
      _NOP();               \
      SMCR = 0x08; /*this is an LGT kind of sleep disable*/ \
/* No sei(); */             \
} while (0)

#define lowPowerLdoOff(mode)\
do {                        \
      cli();                \
      power2_ldo_disable(); \
      sleep_enable_m(mode); \
      sleep_cpu();          \
      _NOP();               \
      _NOP();               \
      SMCR = 0x08; /*this is an LGT kind of sleep disable*/ \
      power2_ldo_enable();  \
/* No sei(); */             \
} while (0)
/* Interrupts stay disabled due to the next few timed     *
 * register value change enablement and LGT's WDT faliure */

  #endif

/*******************************************************************************
* Name: idle
* Description: Putting ATmega328P/168 into idle state. Please make sure you
*                    understand the implication and result of disabling module.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_15MS - 15 ms sleep
*               (b) SLEEP_30MS - 30 ms sleep
*               (c) SLEEP_60MS - 60 ms sleep
*               (d) SLEEP_120MS - 120 ms sleep
*               (e) SLEEP_250MS - 250 ms sleep
*               (f) SLEEP_500MS - 500 ms sleep
*               (g) SLEEP_1S - 1 s sleep
*               (h) SLEEP_2S - 2 s sleep
*               (i) SLEEP_4S - 4 s sleep
*               (j) SLEEP_8S - 8 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. timer2     Timer 2 module disable control:
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - Leave Timer 2 module in its default state
*
* 4. timer1     Timer 1 module disable control:
*               (a) TIMER1_OFF - Turn off Timer 1 module
*               (b) TIMER1_ON - Leave Timer 1 module in its default state
*
* 5. timer0     Timer 0 module disable control:
*               (a) TIMER0_OFF - Turn off Timer 0 module
*               (b) TIMER0_ON - Leave Timer 0 module in its default state
*
* 6. spi        SPI module disable control:
*               (a) SPI_OFF - Turn off SPI module
*               (b) SPI_ON - Leave SPI module in its default state
*
* 7. usart0     USART0 module disable control:
*               (a) USART0_OFF - Turn off USART0  module
*               (b) USART0_ON - Leave USART0 module in its default state
*
* 8. twi        TWI module disable control:
*               (a) TWI_OFF - Turn off TWI module
*               (b) TWI_ON - Leave TWI module in its default state
*
*******************************************************************************/
#if ( defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega88__)) && ! defined(__LGT8F__)
void    LowPowerClass::idle(period_t period, adc_t adc, timer2_t timer2,
                            timer1_t timer1, timer0_t timer0,
                            spi_t spi, usart0_t usart0, twi_t twi)
{
    unsigned char clockSource = 0;         // Restore previous setting

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));

        power_timer2_disable();
    }

    if (adc == ADC_OFF)
    {
        power2_adc_disable();
        power_adc_disable();
    }

    if (timer1 == TIMER1_OFF)   power_timer1_disable();
    if (timer0 == TIMER0_OFF)   power_timer0_disable();
    if (spi == SPI_OFF)         power_spi_disable();
    if (usart0 == USART0_OFF)   power_usart0_disable();
    if (twi == TWI_OFF)         power_twi_disable();

    if (period != SLEEP_FOREVER)
    {
        wdt_enable(period);
        WDTCSR |= (1 << WDIE);
    }
    else
    {
        wdt_disable();
    }

    lowPowerBodOn(SLEEP_MODE_IDLE);

    if (adc == ADC_OFF)
    {
        power_adc_enable();
        power2_adc_enable();
    }

    if (timer2 == TIMER2_OFF)
    {
        TCCR2B = clockSource;              // Restore previous setting
        
        power_timer2_enable();
    }

    if (timer1 == TIMER1_OFF)   power_timer1_enable();
    if (timer0 == TIMER0_OFF)   power_timer0_enable();
    if (spi == SPI_OFF)         power_spi_enable();
    if (usart0 == USART0_OFF)   power_usart0_enable();
    if (twi == TWI_OFF)         power_twi_enable();
}
#endif

/*******************************************************************************
* Name: idle
* Description: Putting LGT8F328D into idle state. Please make sure you
*        understand the implication and result of disabling modules.
*        -Take note that there are extra analog modules.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. timer2     Timer 2 module disable control:
*               (a) TIMER1_OFF - Turn off Timer 1 module
*               (b) TIMER1_ON - Leave Timer 1 module in its default state
*
* 4. timer1     Timer 1 module disable control:
*               (a) TIMER1_OFF - Turn off Timer 1 module
*               (b) TIMER1_ON - Leave Timer 1 module in its default state
*
* 5. timer0     Timer 0 module disable control:
*               (a) TIMER0_OFF - Turn off Timer 0 module
*               (b) TIMER0_ON - Leave Timer 0 module in its default state
*
* 6. spi        SPI module disable control:
*               (a) SPI_OFF - Turn off SPI module
*               (b) SPI_ON - Leave SPI module in its default state
*
* 7. usart0     USART0 module disable control:
*               (a) USART0_OFF - Turn off USART1  module
*               (b) USART0_ON - Leave USART1 module in its default state
*
* 8. twi        TWI module disable control:
*               (a) TWI_OFF - Turn off TWI module
*               (b) TWI_ON - Leave TWI module in its default state
*
* 9. pcic      Pin Change and Interrupt module disable control:
*               (a) PCIC_OFF - Turn off PCI module
*               (b) PCIC_ON - Leave PCI module in its previous state
*
* 10. flashctl  Flash Controller (E2PCTL) module disable control:
*               (a) FLASHCTL_OFF - Turn off E2PCTL module
*               (b) FLASHCTL_ON - Leave E2PCTL module in its previous state
*
*******************************************************************************/
#if defined (__LGT8FX8E__)
void    LowPowerClass::idle(period_t period, adc_t adc,
                            timer2_t timer2, timer1_t timer1, timer0_t timer0,
                            spi_t spi, usart0_t usart0, twi_t twi,
                            pcic_t pcic, flashctl_t flashctl)
{
    // Temporary variables
    unsigned char clockSource, o_prr, o_prr1, o_adcsra, tmp_prr;

    o_prr  = PRR;
    o_prr1 = PRR1;
    tmp_prr = o_prr;                       // For shorter code

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));

        tmp_prr |= (1 << PRTIM2);                           // power_timer2_disable();
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;  // Its default value
        tmp_prr |= (1 << PRADC);                            // power_adc_disable();
    }

    if (timer1 == TIMER1_OFF)   tmp_prr |= (1 << PRTIM1);   // power_timer1_disable();
    if (timer0 == TIMER0_OFF)   tmp_prr |= (1 << PRTIM0);   // power_timer0_disable();
    if (spi    == SPI_OFF)      tmp_prr |= (1 << PRSPI);    // power_spi_disable();
    if (usart0 == USART0_OFF)   tmp_prr |= (1 << PRUSART0); // power_usart0_disable();
    if (twi    == TWI_OFF)      tmp_prr |= (1 << PRTWI);    // power_twi_disable();
    PRR = tmp_prr;

    tmp_prr = o_prr1;
    if (pcic == PCIC_OFF)       tmp_prr |= (1 << PRPCI);    // power_pci_disable();
    if (flashctl==FLASHCTL_OFF) tmp_prr |= (1 << PREFL);    // power_flashctrl_disable();
    PRR1 = tmp_prr;

    if (period != SLEEP_FOREVER)
    {
        // enable 32KRC for WDT
        s_unlockWrite(PMCR,PMCR | 0x10);
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
        power_wdtcc_disable();
    }

    cli();
    sleep_flash_disable();                 // Stop FLASH clock during sleep. Timed value change sequence.

    lowPower_(SLEEP_MODE_IDLE);        // Interrupts disabled until wdt_disable() because of defective wdt

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }
    else
    {
        power_wdtcc_enable();
    }

    sei();

    PRR  = o_prr;                          // Restore modules power/clock
    PRR1 = o_prr1;

    if (adc == ADC_OFF)
    {                                  // If PRR is restored before the ADC is reenabled,
        ADCSRA = o_adcsra;             //   unwated conversion start can be avoided
    }

    if (timer2 == TIMER2_OFF) TCCR2B = clockSource; // Restore timer2 previous setting
}
#endif

/*******************************************************************************
* Name: idle
* Description: Putting LGT8F328P into idle state. Please make sure you
*        understand the implication and result of disabling modules.
*        -Take note that extra Timer 3 compared to an ATmega328P.
*        -Also take note that there are extra analog modules.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. timer3     Timer 3 module disable control:
*               (a) TIMER3_OFF - Turn off Timer 3 module
*               (b) TIMER3_ON - Leave Timer 3 module in its default state
*
* 4. timer2     Timer 2 module disable control:
*               (a) TIMER1_OFF - Turn off Timer 1 module
*               (b) TIMER1_ON - Leave Timer 1 module in its default state
*
* 5. timer1     Timer 1 module disable control:
*               (a) TIMER1_OFF - Turn off Timer 1 module
*               (b) TIMER1_ON - Leave Timer 1 module in its default state
*
* 6. timer0     Timer 0 module disable control:
*               (a) TIMER0_OFF - Turn off Timer 0 module
*               (b) TIMER0_ON - Leave Timer 0 module in its default state
*
* 7. spi        SPI module disable control:
*               (a) SPI_OFF - Turn off SPI module
*               (b) SPI_ON - Leave SPI module in its default state
*
* 8. usart0     USART0 module disable control:
*               (a) USART0_OFF - Turn off USART1  module
*               (b) USART0_ON - Leave USART1 module in its default state
*
* 9. twi        TWI module disable control:
*               (a) TWI_OFF - Turn off TWI module
*               (b) TWI_ON - Leave TWI module in its default state
*
* 10. pcic      Pin Change and Interrupt module disable control:
*               (a) PCIC_OFF - Turn off PCI module
*               (b) PCIC_ON - Leave PCI module in its previous state
*
* 11. flashctl  Flash Controller (E2PCTL) module disable control:
*               (a) FLASHCTL_OFF - Turn off E2PCTL module
*               (b) FLASHCTL_ON - Leave E2PCTL module in its previous state
*
*******************************************************************************/
#if defined (__LGT8FX8P__)
void    LowPowerClass::idle(period_t period, adc_t adc, timer3_t timer3,
                            timer2_t timer2, timer1_t timer1, timer0_t timer0,
                            spi_t spi, usart0_t usart0, twi_t twi,
                            pcic_t pcic, flashctl_t flashctl)
{
    // Temporary variables
    unsigned char clockSource, o_prr, o_prr1, o_adcsra, tmp_prr;

    o_prr  = PRR;
    o_prr1 = PRR1;
    tmp_prr = o_prr;                       // For shorter code

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));

        tmp_prr |= (1 << PRTIM2);                           // power_timer2_disable();
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;  // Its default value
        tmp_prr |= (1 << PRADC);                            // power_adc_disable();
    }

    if (timer1 == TIMER1_OFF)   tmp_prr |= (1 << PRTIM1);   // power_timer1_disable();
    if (timer0 == TIMER0_OFF)   tmp_prr |= (1 << PRTIM0);   // power_timer0_disable();
    if (spi    == SPI_OFF)      tmp_prr |= (1 << PRSPI);    // power_spi_disable();
    if (usart0 == USART0_OFF)   tmp_prr |= (1 << PRUSART0); // power_usart0_disable();
    if (twi    == TWI_OFF)      tmp_prr |= (1 << PRTWI);    // power_twi_disable();
    PRR = tmp_prr;

    tmp_prr = o_prr1;
    if (timer3 == TIMER3_OFF)   tmp_prr |= (1 << PRTC3);    // power_timer3_disable;
    if (pcic == PCIC_OFF)       tmp_prr |= (1 << PRPCI);    // power_pci_disable();
    if (flashctl==FLASHCTL_OFF) tmp_prr |= (1 << PREFL);    // power_flashctrl_disable();
    PRR1 = tmp_prr;

    if (period != SLEEP_FOREVER)
    {
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
        power_wdtcc_disable();
    }

    cli();
    sleep_flash_disable();                 // Stop FLASH clock during sleep. Timed value change sequence.

    lowPower_(SLEEP_MODE_IDLE);        // Interrupts disabled until wdt_disable() because of defective wdt

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }
    else
    {
        power_wdtcc_enable();
    }

    sei();

    PRR  = o_prr;                          // Restore modules power/clock
    PRR1 = o_prr1;

    if (adc == ADC_OFF)
    {                                  // If PRR is restored before the ADC is reenabled,
        ADCSRA = o_adcsra;             //   unwated conversion start can be avoided
    }

    if (timer2 == TIMER2_OFF) TCCR2B = clockSource; // Restore timer2 previous setting
}
#endif

/*******************************************************************************
* Name: adcNoiseReduction
* Description: Putting microcontroller into ADC noise reduction state. This is
*              a very useful state when using the ADC to achieve best and low
*              noise signal.
*              Stops the working PWM timers during sleep.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*     ATmega328P:
*               (a) SLEEP_15MS - 15 ms sleep
*               (b) SLEEP_30MS - 30 ms sleep
*               (c) SLEEP_60MS - 60 ms sleep
*               (d) SLEEP_120MS - 120 ms sleep
*               (e) SLEEP_250MS - 250 ms sleep
*               (f) SLEEP_500MS - 500 ms sleep
*               (g) SLEEP_1S - 1 s sleep
*               (h) SLEEP_2S - 2 s sleep
*               (i) SLEEP_4S - 4 s sleep
*               (j) SLEEP_8S - 8 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*     LGT8F328x:
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control. Turning off the ADC module is
*               basically removing the purpose of this low power mode.
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - It does not change the ADC module state
*
* 3. timer2     Timer 2 module disable control:
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - It does not change the Timer 2 module state
*
*******************************************************************************/
#if ! defined (__LGT8F__)
void    LowPowerClass::adcNoiseReduction(period_t period, adc_t adc, timer2_t timer2)
{
    // Temporary clock source variable
    unsigned char clockSource = 0;

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF) power2_adc_disable();

    if (period != SLEEP_FOREVER)
    {
        wdt_enable(period);
        WDTCSR |= (1 << WDIE);
    }
    else
    {
        wdt_disable();
    }

    lowPowerBodOn(SLEEP_MODE_ADC);

    if (adc == ADC_OFF) power2_adc_enable();
    if (timer2 == TIMER2_OFF) TCCR2B = clockSource;    // Restore previous setting
}
#else
    // LGT8Fx
void    LowPowerClass::adcNoiseReduction(period_t period, adc_t adc, timer2_t timer2)
{
    // Temporary clock source variable
    unsigned char clockSource = 0;

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF) power2_adc_disable();

    if (period != SLEEP_FOREVER)
    {
    #if defined (__LGT8FX8E__)
        // enable 32KRC for WDT
        s_unlockWrite(PMCR,PMCR | 0x10);
    #endif
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
        power_wdtcc_disable();
    }

    cli();
    sleep_flash_disable();                 // Stop FLASH clock during sleep. Timed value change sequence.

    lowPower_(SLEEP_MODE_ADC);

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }
    else
    {
        power_wdtcc_enable();
    }

    sei();

    if (adc == ADC_OFF) power2_adc_enable();
    if (timer2 == TIMER2_OFF) TCCR2B = clockSource; // Restore previous setting
}
#endif

/*******************************************************************************
* Name: powerDown
* Description: Putting microcontroller into power down state. This is
*              the lowest current consumption state. Use this together with
*              external pin interrupt to wake up through external event
*              triggering (example: RTC clockout pin, SD card detect pin).
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_15MS - 15 ms sleep
*               (b) SLEEP_30MS - 30 ms sleep
*               (c) SLEEP_60MS - 60 ms sleep
*               (d) SLEEP_120MS - 120 ms sleep
*               (e) SLEEP_250MS - 250 ms sleep
*               (f) SLEEP_500MS - 500 ms sleep
*               (g) SLEEP_1S - 1 s sleep
*               (h) SLEEP_2S - 2 s sleep
*               (i) SLEEP_4S - 4 s sleep
*               (j) SLEEP_8S - 8 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Does not change the ADC module state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off BOD module
*               (b) BOD_ON - Does not change the BOD module state
*
*******************************************************************************/
#if ! defined (__LGT8F__)
void    LowPowerClass::powerDown(period_t period, adc_t adc, bod_t bod)
{
    // Temporary variable
    unsigned char o_adcsra;

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x00;  // Its default value
        // If ADMUX selecting any internal reference while in sleep, this will increase current consumption.
    }

    if (period != SLEEP_FOREVER)
    {
        wdt_enable(period);
        WDTCSR |= (1 << WDIE);
    }
    else
    {
        wdt_disable();
    }

    if (bod == BOD_OFF)
    {
        #if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PB__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168PB__)
            lowPowerBodOff(SLEEP_MODE_PWR_DOWN);
        #else
            lowPowerBodOn(SLEEP_MODE_PWR_DOWN);
        #endif
    }
    else
    {
        lowPowerBodOn(SLEEP_MODE_PWR_DOWN);
    }

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
}
#endif

#if defined(__LGT8F__)

#define hfosc_down()                   \
do  {                                  \
 /* Switch main clock (and WDT) to internal 32kHz */ \
      o_pmcr =  PMCR;                  \
      pmtmp1 = (o_pmcr & 0x1F) | 0x50; \
      pmtmp2 = (o_pmcr & 0x12) | 0x50; \
      PMCR = 0x80;                     \
      PMCR = pmtmp1;                   \
      _NOP();                          \
      _NOP();                          \
 /* Stop high frequency oscillators */ \
      PMCR = 0x80;                     \
      PMCR = pmtmp2;                   \
 /* No prescale */                     \
      o_clkpr = CLKPR;                 \
      CLKPR = 0x80;                    \
      CLKPR = 0x00;                    \
} while (0)

#define hfosc_up()                     \
do  {                                  \
 /* Restart high frequecy oscs */      \
      PMCR = 0x80;                     \
      PMCR = pmtmp1;                   \
 /* Waiting for crystal stable ~220 microsec on 32kHz = 7 clocks */ \
      _lgt8fx_delay_cycles(4);         \
 /* Prescaler recovery */              \
      CLKPR = 0x80;                    \
      CLKPR = o_clkpr;                 \
 /* Restore system clock settings */   \
      PMCR = 0x80;                     \
      PMCR = o_pmcr;                   \
      _NOP();                          \
      _NOP();                          \
} while (0)
/*  1 / 32768 = 30.517578125 microsec  */

#endif

/*******************************************************************************
* Name: powerDown
* Description: Putting microcontroller into power down state. In case of LGT8Fx8x
*              this is NOT the lowest current consumption state. Use this together
*              with external pin interrupt to wake up through external event
*              triggering (example: RTC clockout pin, SD card detect pin).
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Does not change the ADC module state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off LVD module
*               (b) BOD_ON - Does not change the LVD module state
*******************************************************************************/
#if defined (__LGT8FX8E__)
void    LowPowerClass::powerDown(period_t period, adc_t adc, bod_t bod)
{
    // Temporary variables
    unsigned char o_adcsra, o_vdtcr;

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;
//      If ADMUX not zero, the internal voltage ref. may reamain powered. -> Test it!
    }

    if (period != SLEEP_FOREVER)
    {
        // enable 32KRC for WDT
        s_unlockWrite(PMCR,PMCR | 0x10);
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }

    cli();
//    sleep_flash_disable();                 // Stop FLASH clock during sleep. // Timed value change sequence.
//    power2_swd_disable();                  // Timed sequence.

    if (bod == BOD_OFF)
    {
        o_vdtcr = VDTCR;
        s_unlockWrite(VDTCR,0x40);
    }

    lowPowerLdoOff(SLEEP_MODE_PWR_DOWN);

    if (bod == BOD_OFF) s_unlockWrite(VDTCR,o_vdtcr);

//    power2_swd_enable();                   // Timed sequence.

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }

    sei();

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
}
#endif

/*******************************************************************************
* Name: powerDown
* Description: Putting LGT8F328P microcontroller into power down (DPS1) state.
*              All internal and external oscillators are turned off. Use this
*              together with external pin interrupt to wake up through external
*              event triggering (example: RTC clockout pin, SD card detect pin).
*            - This is not the lowest current consumption state.
*            - Since all clocks are stopped, the ADC unusable. So no reason to
*              leave ACD powered during sleep.
*             - Watchdog timer can run only on 32kHz LFRC
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Does not change the ADC module state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off LVD module
*               (b) BOD_ON - Does not change the LVD module state
*******************************************************************************/
#if defined (__LGT8FX8P__)
void    LowPowerClass::powerDown(period_t period, adc_t adc, bod_t bod)
{
    // Temporary variables
    unsigned char o_adcsra, o_vdtcr;

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;  // Its default value
//      If ADMUX not zero, the internal voltage ref. may reamain powered. -> Test it!
    }

    if (period != SLEEP_FOREVER)
    {
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }

    cli();
//    sleep_flash_disable();                 // Stop FLASH clock during sleep. // Timed value change sequence.
//    power2_swd_disable();                  // Timed sequence.

    if (bod == BOD_OFF)
    {
        o_vdtcr = VDTCR;
        s_unlockWrite(VDTCR,0x40);
    }
 
    lowPowerLdoOff(SLEEP_MODE_PWR_DOWN);

    if (bod == BOD_OFF) s_unlockWrite(VDTCR,o_vdtcr);

//    power2_swd_enable();                   // Timed sequence.

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }

    sei();

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
}
#endif

/*******************************************************************************
* Name: powerSave
* Description: Putting microcontroller into power save state. This is
*              the lowest current consumption state after power down.
*              Use this state together with an external 32.768 kHz crystal (but
*              8/16 MHz crystal/resonator need to be removed) to provide an
*              asynchronous clock source to Timer 2. Please take note that
*              Timer 2 is also used by the Arduino core for PWM operation.
*              Please refer to wiring.c for explanation. Removal of the external
*              8/16 MHz crystal/resonator requires the microcontroller to run
*              on its internal RC oscillator which is not so accurate for time
*              critical operation.
*
*   ----->     It is not implemented for LGT8Fx microcontrollers.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_15MS - 15 ms sleep
*               (b) SLEEP_30MS - 30 ms sleep
*               (c) SLEEP_60MS - 60 ms sleep
*               (d) SLEEP_120MS - 120 ms sleep
*               (e) SLEEP_250MS - 250 ms sleep
*               (f) SLEEP_500MS - 500 ms sleep
*               (g) SLEEP_1S - 1 s sleep
*               (h) SLEEP_2S - 2 s sleep
*               (i) SLEEP_4S - 4 s sleep
*               (j) SLEEP_8S - 8 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off BOD module
*               (b) BOD_ON - Leave BOD module in its default state
*
* 4. timer2     Timer 2 module disable control:
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - Leave Timer 2 module in its default state
*
*******************************************************************************/
#if ! defined (__LGT8F__)
void    LowPowerClass::powerSave(period_t period, adc_t adc, bod_t bod, timer2_t timer2)
{
    // Temporary variables
    unsigned char clockSource, o_adcsra;

    if (timer2 == TIMER2_OFF)
    {
        // Store current setting
        clockSource = TCCR2B;

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x00;  // Its default value
    }

    if (period != SLEEP_FOREVER)
    {
        wdt_enable(period);
        WDTCSR |= (1 << WDIE);
    }
    else
    {
        wdt_disable();
    }

    if (bod == BOD_OFF)
    {
        #if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PB__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168PB__)
            lowPowerBodOff(SLEEP_MODE_PWR_SAVE);
        #else
            lowPowerBodOn(SLEEP_MODE_PWR_SAVE);
        #endif
    }
    else
    {
        lowPowerBodOn(SLEEP_MODE_PWR_SAVE);
    }

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
    if (timer2 == TIMER2_OFF) TCCR2B = clockSource;
}
#endif

/*******************************************************************************
* Name: powerStandby
* Description: Putting microcontroller into power standby state.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_15MS - 15 ms sleep
*               (b) SLEEP_30MS - 30 ms sleep
*               (c) SLEEP_60MS - 60 ms sleep
*               (d) SLEEP_120MS - 120 ms sleep
*               (e) SLEEP_250MS - 250 ms sleep
*               (f) SLEEP_500MS - 500 ms sleep
*               (g) SLEEP_1S - 1 s sleep
*               (h) SLEEP_2S - 2 s sleep
*               (i) SLEEP_4S - 4 s sleep
*               (j) SLEEP_8S - 8 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off BOD module
*               (b) BOD_ON - Leave BOD module in its default state
*
*******************************************************************************/
#if ! defined (__LGT8F__)
void    LowPowerClass::powerStandby(period_t period, adc_t adc, bod_t bod)
{
    unsigned char o_adcsra;

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x00;  // Its default value
    }

    if (period != SLEEP_FOREVER)
    {
        wdt_enable(period);
        WDTCSR |= (1 << WDIE);
    }
    else
    {
        wdt_disable();
    }

    if (bod == BOD_OFF)
    {
        #if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)
            lowPowerBodOff(SLEEP_MODE_STANDBY);
        #else
            lowPowerBodOn(SLEEP_MODE_STANDBY);
        #endif
    }
    else
    {
        lowPowerBodOn(SLEEP_MODE_STANDBY);
    }

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
}
#endif

/*******************************************************************************
* Name: powerStandby
* Description: Putting LGT8F328D microcontroller into Power / Off S0 mode.
*            - Since clocks are stopped, the ADC unusable. So no reason to
*              leave ACD powered during sleep.
*            - During this sleep mode basically the HF oscillator(s) stay
*              powered but disconnected.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Does not change the ADC module state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off LVD module
*               (b) BOD_ON - Does not change the LVD module state
*
* 4. timer2     Timer 2 module disable control:
*                   However timer 2 can not enabled in LGT8F328D during
*                   sleep. This argument is only for code portability.
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - Does not change the Timer 2 module state
*
* 5. hfosc      High frequency oscillator control:
*               (a) HFOSC_OFF - Turn off internal high freqency and
*                   external oscillators during sleep
*               (b) HFOSC_ON - Leave run all oscillators during sleep
*                   (faster wake up, higher consumption)
*******************************************************************************/
#if defined (__LGT8FX8E__)
void    LowPowerClass::powerStandby(period_t period, adc_t adc, bod_t bod, timer2_t timer2)
{
    // Temporary variables
    unsigned char clockSource, o_adcsra, o_vdtcr;

    // Timer 2 disabled in LGT8F328D during sleep ... so this code not necessary
    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;
    }

    if (period != SLEEP_FOREVER)
    {
        // enable 32KRC for WDT
        s_unlockWrite(PMCR,PMCR | 0x10);
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
        power_wdtcc_disable();
    }

    cli();
//    sleep_flash_disable();                 // Stop FLASH clock during sleep.
//    if (hfosc == HFOSC_OFF) hfosc_down();  // Does not reduce consumption

    if (bod == BOD_OFF)
    {
        o_vdtcr = VDTCR;
        s_unlockWrite(VDTCR,0x40);
    }

    lowPowerLdoOff(SLEEP_MODE_STANDBY);

//    if (hfosc == HFOSC_OFF) hfosc_up();
    if (bod == BOD_OFF) s_unlockWrite(VDTCR,o_vdtcr);

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }
    else
    {
        power_wdtcc_enable();
    }

    sei();

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
    if (timer2 == TIMER2_OFF) TCCR2B = clockSource;
}
#endif

/*******************************************************************************
* Name: powerStandby
* Description: Putting LGT8F328P microcontroller into DPS0 mode. 
*            - Since clocks are stopped, the ADC unusable. So no reason to
*              leave ACD powered during sleep.
*            - During this sleep mode basically the HF oscillator(s) stay
*              powered but disconnected.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Does not change the ADC module state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off LVD module
*               (b) BOD_ON - Does not change the LVD module state
*
* 4. timer2     Timer 2 module disable control:
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - Does not change the Timer 2 module state
*
* 5. hfosc      High frequency oscillator control:
*               (a) HFOSC_OFF - Turn off internal high freqency and
*                   external oscillators during sleep
*               (b) HFOSC_ON - Leave run all oscillators during sleep
*                   (faster wake up, higher consumption)
*******************************************************************************/
#if defined (__LGT8FX8P__)
void    LowPowerClass::powerStandby(period_t period, adc_t adc, bod_t bod, timer2_t timer2)
{
    // Temporary variables
    unsigned char clockSource, o_adcsra, o_vdtcr;

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;  // Its default value
    }

    if (period != SLEEP_FOREVER)
    {
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
        power_wdtcc_disable();
    }

    cli();
//    sleep_flash_disable();                 // Stop FLASH clock during sleep.
//    if (hfosc == HFOSC_OFF) hfosc_down();  // Does not reduce consumption

    if (bod == BOD_OFF)
    {
        o_vdtcr = VDTCR;
        s_unlockWrite(VDTCR,0x40);
    }

    lowPowerLdoOff(SLEEP_MODE_STANDBY);

//    if (hfosc == HFOSC_OFF) hfosc_up();
    if (bod == BOD_OFF) s_unlockWrite(VDTCR,o_vdtcr);

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }
    else
    {
        power_wdtcc_enable();
    }

    sei();

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
    if (timer2 == TIMER2_OFF) TCCR2B = clockSource;
}
#endif

/*******************************************************************************
* Name: powerExtStandby
* Description: Putting microcontroller into power extended standby state. This
*              is different from the power standby state as it has the
*              capability to run Timer 2 asynchronously.
*              Not implemented on Atmega88 and Atmega168.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_15MS - 15 ms sleep
*               (b) SLEEP_30MS - 30 ms sleep
*               (c) SLEEP_60MS - 60 ms sleep
*               (d) SLEEP_120MS - 120 ms sleep
*               (e) SLEEP_250MS - 250 ms sleep
*               (f) SLEEP_500MS - 500 ms sleep
*               (g) SLEEP_1S - 1 s sleep
*               (h) SLEEP_2S - 2 s sleep
*               (i) SLEEP_4S - 4 s sleep
*               (j) SLEEP_8S - 8 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off BOD module
*               (b) BOD_ON - Leave BOD module in its default state
*
* 4. timer2     Timer 2 module disable control:
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - Leave Timer 2 module in its default state
*
*******************************************************************************/
#if ! ( defined (__LGT8F__) || defined (__AVR_ATmega88__) || defined(__AVR_ATmega168__) )
void    LowPowerClass::powerExtStandby(period_t period, adc_t adc, bod_t bod, timer2_t timer2)
{
    // Temporary variables
    unsigned char clockSource, o_adcsra;

    if (timer2 == TIMER2_OFF)
    {
        // Store current setting
        clockSource = TCCR2B;

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x00;  // Its default value
    }

    if (period != SLEEP_FOREVER)
    {
        wdt_enable(period);
        WDTCSR |= (1 << WDIE);
    }
    else
    {
        wdt_disable();
    }

    if (bod == BOD_OFF)
    {
        #if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PB__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168PB__)
            lowPowerBodOff(SLEEP_MODE_EXT_STANDBY);
        #else
            lowPowerBodOn(SLEEP_MODE_EXT_STANDBY);
        #endif
    }
    else
    {
        lowPowerBodOn(SLEEP_MODE_EXT_STANDBY);
    }

    if (adc == ADC_OFF)       ADCSRA = o_adcsra;

    if (timer2 == TIMER2_OFF) TCCR2B = clockSource;
}
#endif

/*******************************************************************************
* Name: powerExtStandby
* Description: Putting LGT8F328D microcontroller into power extended standby state.
*            - Since clocks of all modules are stopped, the ADC unusable. So no
*              reason to leave ACD powered during sleep.
*            - During this sleep mode the HF oscillator(s) stay powered.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off BOD module
*               (b) BOD_ON - Leave BOD module in its default state
*
* 4. timer2     Timer 2 module disable control:
*                   However timer 2 can not enabled in LGT8F328D during
*                   sleep. This argument is only for code portability.
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - Does not change the Timer 2 module state
*
* 5. hfosc      High frequency oscillator control:
*               (a) HFOSC_OFF - Turn off internal high freqency and
*                   external oscillators during sleep
*               (b) HFOSC_ON - Leave run all oscillators during sleep
*                   (faster wake up, higher consumption)
*
*******************************************************************************/
#if defined (__LGT8FX8E__)
void    LowPowerClass::powerExtStandby(period_t period, adc_t adc, bod_t bod, timer2_t timer2)
{
    // Temporary variables
    unsigned char clockSource, o_adcsra, o_vdtcr;

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;
    }

    if (period != SLEEP_FOREVER)
    {
        // enable 32KRC for WDT
        s_unlockWrite(PMCR,PMCR | 0x10);
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
        power_wdtcc_disable();
    }

    cli();
//    sleep_flash_disable();                 // Stop FLASH clock during sleep.
//    if (hfosc == HFOSC_OFF) hfosc_down();  // Does not reduce consumption

    if (bod == BOD_OFF)
    {
        o_vdtcr = VDTCR;
        s_unlockWrite(VDTCR,0x40);
    }
	
    lowPowerLdoOff(SLEEP_MODE_EXT_STANDBY);

//    if (hfosc == HFOSC_OFF) hfosc_up();
    if (bod == BOD_OFF) s_unlockWrite(VDTCR,o_vdtcr);

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }
    else
    {
        power_wdtcc_enable();
    }

    sei();

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
    if (timer2 == TIMER2_OFF) TCCR2B = clockSource;
}
#endif

/*******************************************************************************
* Name: powerExtStandby
* Description: Putting LGT8F328P microcontroller into power extended standby state.
*            - Since clocks of all modules are stopped, the ADC unusable. So no
*              reason to leave ACD powered during sleep.
*            - During this sleep mode the HF oscillator(s) stay powered.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_64MS  - 64 ms sleep
*               (b) SLEEP_128MS - 128 ms sleep
*               (c) SLEEP_256MS - 256 ms sleep
*               (d) SLEEP_512MS - 512 ms sleep
*               (e) SLEEP_1S    - 1 s sleep
*               (f) SLEEP_2S    - 2 s sleep
*               (g) SLEEP_4S    - 4 s sleep
*               (h) SLEEP_8S    - 8 s sleep
*               (i) SLEEP_16S  - 16 s sleep
*               (j) SLEEP_32S  - 32 s sleep
*               (k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc        ADC module disable control:
*               (a) ADC_OFF - Turn off ADC module
*               (b) ADC_ON - Leave ADC module in its default state
*
* 3. bod        Brown Out Detector (BOD) module disable control:
*               (a) BOD_OFF - Turn off BOD module
*               (b) BOD_ON - Leave BOD module in its default state
*
* 4. timer2     Timer 2 module disable control:
*               (a) TIMER2_OFF - Turn off Timer 2 module
*               (b) TIMER2_ON - Leave Timer 2 module in its default state
*
* 5. hfosc      High frequency oscillator control:
*               (a) HFOSC_OFF - Turn off internal high freqency and
*                   external oscillators during sleep
*               (b) HFOSC_ON - Leave run all oscillators during sleep
*                   (faster wake up, higher consumption)
*
*******************************************************************************/
#if defined (__LGT8FX8P__)
void    LowPowerClass::powerExtStandby(period_t period, adc_t adc, bod_t bod, timer2_t timer2)
{
    // Temporary variables
    unsigned char clockSource, o_adcsra, o_vdtcr;

    if (timer2 == TIMER2_OFF)
    {
        clockSource = TCCR2B;              // Store current setting

        // Remove the clock source to shutdown Timer2
        TCCR2B &= (uint8_t) ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    }

    if (adc == ADC_OFF)
    {
        o_adcsra = ADCSRA;
        ADCSRA = 0x05;  // Its default value
    }

    if (period != SLEEP_FOREVER)
    {
        wdt_ienable(period);
    }
    else
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
        power_wdtcc_disable();
    }

    cli();
//    sleep_flash_disable();                 // Stop FLASH clock during sleep.
//    if (hfosc == HFOSC_OFF) hfosc_down();  // Does not reduce consumption

    if (bod == BOD_OFF)
    {
        o_vdtcr = VDTCR;
        s_unlockWrite(VDTCR,0x40);
    }
	
    lowPowerLdoOff(SLEEP_MODE_EXT_STANDBY);

//    if (hfosc == HFOSC_OFF) hfosc_up();
    if (bod == BOD_OFF) s_unlockWrite(VDTCR,o_vdtcr);

    if (period != SLEEP_FOREVER)
    {
        WDTCSR = 0x10;
        WDTCSR = 0x10;
        wdt_disable();
    }
    else
    {
        power_wdtcc_enable();
    }

    sei();

    if (adc == ADC_OFF) ADCSRA = o_adcsra;
    if (timer2 == TIMER2_OFF) TCCR2B = clockSource;
}
#endif

/*******************************************************************************
* Name: deepSleep2
* Description: Putting LGT8F328P microcontroller into deep sleep.
*              Wake up sources are available by port D pin level change ( INT0,
*              INT1, RXD, TXD, etc) or using Low Power RC timer or with Reset pin.
*              If you would like to enable port D pin level change as a wake-up
*              source, don't forget to set IOCWK before activate DPS2 sleep mode.
*              After waking up from deep sleep mode, the program execution
*              start with Reset vector. The software can read the MCUSR
*              register to identify a reset condition.
*
* Argument      Description
* =========     ===========
* 1. period     Duration of low power mode (Other wake-up sources may wake it
*               up earlier) :
*               (a) SLEEP_128MS ~ 128 ms sleep
*               (b) SLEEP_256MS ~ 256 ms sleep
*               (c) SLEEP_512MS ~ 512 ms sleep
*               (d) SLEEP_1S    ~ 2,5 s sleep
*               (e) SLEEP_FOREVER - Sleep without waking up through LPRC
*
*******************************************************************************/
#if defined (__LGT8FX8P__)
void    LowPowerClass::deepSleep2(period_t period)
{
    if (period != SLEEP_FOREVER)
    {
        DPS2_enable_with_timer(period-1);
    }
    else
    {
        DPS2_enable_timer_disable();
    }

    lowPower_(SLEEP_MODE_DPS2);

    //Code execution will not continue in this mode
}
#endif

#if ! defined (__LGT8F__)
/*******************************************************************************
* Name: ISR (WDT_vect)
* Description: Watchdog Timer interrupt service routine. This routine is
*		       required to allow automatic WDIF and WDIE bit clearance in
*			   hardware.
*
*******************************************************************************/
ISR (WDT_vect, __attribute__((weak)) )
{
	// WDIE & WDIF is cleared in hardware upon entering this ISR
	wdt_disable();
}
#endif

#else
    #error "Processor architecture is not supported."
#endif

LowPowerClass LowPower;
