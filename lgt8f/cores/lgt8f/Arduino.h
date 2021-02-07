/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "binary.h"

#ifdef __cplusplus
extern "C"{
#endif

void yield(void);

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define ANALOG 0x3

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  #define DEFAULT 0
  #define EXTERNAL 1
  #define INTERNAL1V1 2
  #define INTERNAL INTERNAL1V1
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define DEFAULT 0
  #define EXTERNAL 4
  #define INTERNAL1V1 8
  #define INTERNAL INTERNAL1V1
  #define INTERNAL2V56 9
  #define INTERNAL2V56_EXTCAP 13
#else  
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define INTERNAL1V1 2
#define INTERNAL2V56 3
#else
#define INTERNAL 3
#define INTERNAL1V25 3
#define INTERNAL2V56 2
#define INTERNAL1V024 3
#define INTERNAL2V048 2
#define INTERNAL4V096 4
#endif
#define DEFAULT 1
#define EXTERNAL 0
#endif

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void init(void);
void initVariant(void);

int atexit(void (*func)()) __attribute__((weak));

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
void digitalToggle(uint8_t pin);
int analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, int val);
void analogReadResolution(uint8_t res);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
__inline__ void  delayMicroseconds  (double us)             __attribute__ ((always_inline, unused)); // for variable case
__inline__ void _lgt8fx_delay_cycles(const uint32_t cticks) __attribute__ ((always_inline, unused)); // for constant case
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode);
void detachInterrupt(uint8_t interruptNum);

void setup(void);
void loop(void);

__inline__ void _lgt8fx_delay_cycles(const uint32_t cticks)
{
  uint32_t cticks_left = cticks;

  // loopcounter = 1               -> 7  clockticks    ( clockticks used = 3 + (loopcounter) * 5 - 1 )
  // loopcounter = 2               -> 12 clockticks
  // loopcounter = 16777215        -> 67108862 clocktics
  if (( cticks_left >= 262146L ) && ( cticks_left <= 83886077L ))
    {
      uint32_t lcount;
      lcount  = ( (cticks_left - 2) / 5);
      __builtin_avr_delay_cycles(lcount * 5 + 2);
      cticks_left -= ( lcount * 5 ) + 2;
    }

  // loopcounter = 1                 -> 4 clocktics    ( clockticks used = 2 + loopcounter * 3 - 1 )
  // loopcounter = 2                 -> 7 clocktics
  // loopcounter = 65535             -> 196606 clocktics
  if (( cticks_left >= 196607L ) && ( cticks_left <= 262145L ))
    {
      uint32_t lcount;
//      lcount = ((cticks_left - 196606L) / 3);  Avoid builtin_avr_delay_cycles value became lower than 768 here.
      lcount = ((cticks_left - 195826L) / 3);
      __builtin_avr_delay_cycles(lcount * 4 + 1);
      cticks_left -= lcount * 3 + 1;
    }
  if (( cticks_left >= 768L ) && ( cticks_left <= 196606L ))
    {
      uint32_t lcount;
      lcount = ((cticks_left - 1) / 3);
      __builtin_avr_delay_cycles(lcount * 4 + 1);
      cticks_left -= lcount * 3 + 1;
    }

  // loopcounter = 1                 -> 3 clocktics    ( clockticks used = 1 + loopcounter * 3 - 1 )
  // loopcounter = 2                 -> 6 clocktics
  // loopcounter = 255               -> 765 clocktics
  if (( cticks_left >= 6L ) && ( cticks_left <= 767L ))
    {
      uint32_t lcount;
      lcount =  cticks_left / 3;
      __builtin_avr_delay_cycles(lcount * 3);
      cticks_left -= lcount * 3;
    }
                                    // 6 clockticks:  asm( "breq  6f \n\t 6: brne  7f \n\t 7: breq  8f \n\t 8: brne  9f \n\t 9: \n\t" );
                                    // 6 clockticks:  asm( "brcs  7f \n\t 7: brcs  8f \n\t 8: brcs  9f \n\t 9: \n\t" );
                                    // 6 clockticks:  asm( "brcs .+0 \n\t    brcs .+0 \n\t    brcs .+0 \n\t" );    if carry is set
  while (cticks_left >= 3)
    {
      __asm__ __volatile__ (        // 2 + 1 clock cycles or 1 + 2 clock cycles
        "brcs .+0 \n\t"
        "brcc .+0 \n\t"
        : :
      );
      cticks_left -= 3;
    }

  if (cticks_left == 2)
    {
      __asm__ __volatile__ ("nop \n\t nop");
      cticks_left -= 2;
    }

  if (cticks_left == 1)
    {
      __asm__ __volatile__ ("nop");
      cticks_left -= 1;
    }
}

/* Delay for the given number of microseconds.  Assumes a 1, 2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 25, 30, 32 MHz clock. */
/* In January 2021, delayMicroseconds() for LGT8Fx series MCUs by LaZsolt (github.com/LaZsolt/delayMicroseconds)         */
#ifndef _NOP8_
  #define _NOP0_ ""
  #define _NOP1_ "nop      \n\t"
  #define _NOP2_ "nop      \n\t nop      \n\t"
  #define _NOP3_ "brcc .+0 \n\t brcs .+0 \n\t"
  #define _NOP4_ "brcc .+0 \n\t brcs .+0 \n\t nop      \n\t"
  #define _NOP5_ "brcc .+0 \n\t brcs .+0 \n\t nop      \n\t nop      \n\t"
  #define _NOP6_ "brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0 \n\t"
  #define _NOP7_ "brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0 \n\t nop \n\t"
  #define _NOP8_ "brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0 \n\t nop \n\t nop      \n\t"
#endif

// Multiply by two takes 2 clock cycles
#define _MULby2_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" 

// Multiply by three takes 4 clock cycles
#define _MULby3_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A1      \n\t" \
          "adc    %B0,%B1      \n\t"

// Multiply by four takes 4 clock cycles
#define _MULby4_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" 

// Multiply by five takes 6 clock cycles
#define _MULby5_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A1      \n\t" \
          "adc    %B0,%B1      \n\t"
    
#if   F_CPU >= 32000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP3_
  #define _NOPSinCYCLE_  _NOP5_

//lif F_CPU >= 30000000L
#elif F_CPU >= 29400000L

  #define _CCALCULATE_   _MULby5_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_   _NOP5_
  #define _NOPSinCYCLE_  _NOP3_

// Ethernet crystal frequency
#elif F_CPU >= 25000000L

  #define _CCALCULATE_   _MULby5_    \
         "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_   _NOP3_
  #define _NOPSinCYCLE_  _NOP2_

#elif F_CPU >= 24000000L

  #define _CCALCULATE_   _MULby4_    \
         "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP1_
  #define _NOPSinCYCLE_  _NOP3_

#elif F_CPU >= 22000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP8_
  #define _NOPSinCYCLE_  _NOP8_

/* 3 * 7 MHz
#elif F_CPU >= 21000000L
  #define _CCALCULATE_   _MULby3_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP2_
  #define _NOPSinCYCLE_  _NOP4_
*/

#elif F_CPU >= 20000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP0_
  #define _NOPSinCYCLE_  _NOP2_

#elif F_CPU >= 18000000L

  #define _CCALCULATE_  _MULby3_     \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP3_

#elif F_CPU >= 16000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP3_
  #define _NOPSinCYCLE_ _NOP1_

//lif F_CPU >= 15000000L
#elif F_CPU >= 14700000L

  #define _CCALCULATE_   _MULby3_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP2_

#elif F_CPU >= 14000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP4_
  #define _NOPSinCYCLE_ _NOP4_

// 12 MHz resonator on Wavgat board or for USB communication
#elif F_CPU >= 12000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 11000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP8_

#elif F_CPU >= 10000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP2_
  #define _NOPSinCYCLE_ _NOP2_

#elif F_CPU >= 9000000L

  #define _CCALCULATE_  _MULby3_     \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 8000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP1_

#elif F_CPU >= 7000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP4_

// 6 MHz for 12 MHz resonator divided by 2 on Wavgat board
#elif F_CPU >= 6000000L

  #define _CCALCULATE_  _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 5000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP2_

//lif F_CPU >= 4000000L
#elif F_CPU >= 3660000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

// 3 MHz for 12 MHz resonator divided by 4 on Wavgat board
#elif   F_CPU >= 3000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

//#elif F_CPU >= 2000000L
#elif   F_CPU >= 1800000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

// 1.5 MHz for 12 MHz resonator divided by 8 on Wavgat board
#elif   F_CPU >= 1500000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#elif   F_CPU >= 1000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

// 0.75 MHz for 12 MHz resonator divided by 16 on Wavgat board
#elif   F_CPU >= 750000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#elif   F_CPU >= 500000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

#else

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

#endif

#if     F_CPU >= 3000000L
  #define _SBnBR_                    \
          "sbiw   %A0,1        \n\t" \
          "brne   "
//#elif F_CPU >= 2000000L
//#elif F_CPU >= 1800000L
#elif   F_CPU >= 1500000L
  #define _SBnBR_                    \
          "sbiw   %A0,2        \n\t" \
          "brcc   "
// 0.75 MHz for 12 MHz resonator divided by 16 on Wavgat board and 1 MHz on a normal board
#elif   F_CPU >= 750000L
  #define _SBnBR_                    \
          "sbiw   %A0,4        \n\t" \
          "brcc   "
#else
  #define _SBnBR_                    \
          "sbiw   %A0,8        \n\t" \
          "brcc   "
#endif

__inline__ void delayMicroseconds(double us)
{
    if ( __builtin_constant_p(us)) 
    { 
        _lgt8fx_delay_cycles((uint32_t)(F_CPU/1000000.0)*((double)us));
        return;
    }

    uint16_t tmp0;
    uint16_t tmp1 = us;
    __asm__ volatile(
                // Movw compensates for the missing cycle in
                // the last brne below, and also prevents the
                // compiler from inserting this one. Both operands
                // might actually be the same register, but that's
                // ok.
      "movw   %A0, %A1     \n\t"  // 1 ( tmp = us; )
      _CCALCULATE_                // Cycle counter multiplication and subtraction if needed
      _TUNINGNOPS_
      "1:                  \n\t"
      _NOPSinCYCLE_
      _SBnBR_ "1b          \n\t"  // 4 (3 on last)
      : "=w" (tmp0)
      : "r"  (tmp1)
    );
}

#undef _CCALCULATE_
#undef _TUNINGNOPS_
#undef _NOPSinCYCLE_
#undef _SBnBR_

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.

#define analogInPinToBit(P) (P)

// On the ATmega1280, the addresses of some of the port registers are
// greater than 255, so we can't store them in uint8_t's.
extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
// extern const uint8_t PROGMEM digital_pin_to_bit_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];
extern const uint8_t PROGMEM port_to_PCMSK_PGM[];

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
// 
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_AN_INTERRUPT -1

#ifdef ARDUINO_MAIN
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12
#endif

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8

#define TIMER3A 9
#define TIMER3B 10
#define TIMER3C 11
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
#define TIMER4D 15
#define TIMER5A 16
#define TIMER5B 17
#define TIMER5C 18
#define LGTDAO0 80
#define LGTDAO1 81

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"
#include "HardwareSerial.h"
#include "USBAPI.h"
#if defined(HAVE_HWSERIAL0) && defined(HAVE_CDCSERIAL)
#error "Targets with both UART0 and CDC serial not supported"
#endif

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

#endif

#include "pins_arduino.h"

#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
#include "fastio_digital.h"

#define	INT_OSC	0
#define	EXT_OSC	1
void sysClock(uint8_t mode);
#endif

#ifndef nop
#define	nop()	__asm__ __volatile__ ("nop" ::)
#endif

#endif
