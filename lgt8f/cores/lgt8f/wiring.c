/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "wiring_private.h"

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

#if defined(TIM0_OVF_vect)
ISR(TIM0_OVF_vect)
#else
ISR(TIMER0_OVF_vect)
#endif
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
#if defined(TCNT0)
	t = TCNT0;
#elif defined(TCNT0L)
	t = TCNT0L;
#else
	#error TIMER 0 not defined
#endif

#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
		m++;
#endif

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void delay(unsigned long ms)
{
	uint32_t start = micros();

	while (ms > 0) {
		yield();
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

/* Delay for the given number of microseconds.  Assumes a 1, 4, 8, 12, 16, 20, 24, 25 or 32 MHz clock. */
 
// In case of using LGT MCU need one more NOP in timing loop to compensate shorter executiom time.
#if defined(__LGT8F__)
  #define _ONENOP_ " nop \n\t"
#else
  #define _ONENOP_ ""	
#endif

#define _MORENOP_ ""

/* Link time optimization (LTO for short) has been supported by the IDE since v1.6.11.
 * In Arduino IDE 1.6.11 and newer LTO is enabled by default.  The LTO optimizes the code
 * at link time, making the code (often) significantly smaller without making it "slower"
 * and sometimes destroy acccurate software timings like delayMicroseconds() with lower values.
 * To avoid LTO optimization, the line of delayMicrosecons() definition in arduino.h must be:
 * void delayMicroseconds(unsigned int) __attribute__ ((noinline)) ;
 */
void delayMicroseconds(unsigned int us)
{
	// call = 4 cycles + 2 to 4 cycles to init us(2 for constant delay, 4 for variable)
	// ( call = 2 cycles + 2 to 4 cycles to init us when usin LGT MCU )
	// at the end of this function, return = 4 cycles ( 2 cycles in LGT MCU )

	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);
#if F_CPU >= 32000000L
	// for the 32 MHz clock e.g. LGT8Fx Arduino boards

	// no reason for zero delay fix
	// if (!us) return; // = 3 cycles, (4 when true) or

	// the following loop takes 1/4 of a microsecond (8 cycles with nops)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
    us <<= 2; // x4 us, = 4 cycles

	// 32 MHz is two times faster than 16 MHz so need more nop in the
	// wait cycle and keep the same delay capability than 16 MHz
#undef  _MORENOP_
#define _MORENOP_ " nop \n\t  nop \n\t  nop \n\t  nop \n\t"

	// account for the time taken in the preceeding commands.
#if defined(__LGT8F__)
	__asm__ __volatile__ (
		_MORENOP_
		"nop"); //just waiting 6 cycles
	// LGT burned 16 (18) cycles above, remove 2, (2*8=16)
	// us is at least 4 so we can substract 2
	us -= 2; // = 1 cycle
#else
	// we just burned 16 (18) cycles above, remove 2, (2*8=16)
	// us is at least 4 so we can substract 2
	us -= 2; // = 2 cycles
#endif

#elif F_CPU >= 25000000L
	// for the 25 MHz external clolck
	// Is there any reason for zero delay fix?
	// if (!us) return; // = 3 cycles, (4 when true)

	// the following loop takes a 1/5 of a microsecond (5 cycles)
	// per iteration, so execute it six times for each microsecond of
	// delay requested.
	us = (us << 2) + us; // x5 us, = 7 cycles

	// 25 MHz is faster so need 1 more nop in the wait cycle
#undef  _MORENOP_
#define _MORENOP_ " nop \n\t"

	// account for the time taken in the preceeding commands.
#if defined(__LGT8F__)
	// we just burned 14 (16) cycles above, remove 3, (3*5=20)
	// us is at least 5 so we can substract 3
	us -= 3; // = 1	 cycle
#else
	// we just burned 21 (23) cycles above, remove 4, (4*5=20)
	// us is at least 5 so we can substract 4
	us -= 4; // = 2 cycles
#endif

#elif F_CPU >= 24000000L
	// for the 24 MHz external clock if somebody is working with USB

	// is there any reason for zero delay fix?
	// if (!us) return; // = 3 cycles, (4 when true) (LGT same)

	// the following loop takes a 1/6 of a microsecond (4 cycles)
	// per iteration, so execute it six times for each microsecond of
	// delay requested.
	us *= 6; // x6 us, = 9 cycles ( = 7 cycles in LGT )

	// account for the time taken in the preceeding commands.
#if defined(__LGT8F__)
	_NOP(); //just waiting 1 cycle
	// LGT burned 15 (17) cycles above, remove 4, (4*4=16)
	// us is at least 6 so we can substract 4
	us -= 4; // = 1 cycle
#else
	// we just burned 21 (23) cycles above, remove 5, (5*4=20)
	// us is at least 6 so we can substract 5
	us -= 5; // = 2 cycles
#endif

#elif F_CPU >= 20000000L
	// for the 20 MHz clock on rare Arduino boards

#if defined(__LGT8F__)
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop"); //just waiting 2 cycles
	// no zero delay fix
#else
	// for a one-microsecond delay, simply return.  the overhead
	// of the function call takes 19 (21) cycles, which is 1us
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"); //just waiting 5 cycles
	if (us <= 1) return; //  = 3 cycles, (4 when true)
#endif

	// the following loop takes a 1/5 of a microsecond (4 cycles)
	// per iteration, so execute it five times for each microsecond of
	// delay requested.
	us = (us << 2) + us; // x5 us, = 7 cycles

	// account for the time taken in the preceeding commands.
#if defined(__LGT8F__)
	// we just burned 16 (18) cycles above, remove 4, (4*4=16)
	// us is at least 5 so we can substract 4
	us -= 4; // = 1 cycle
#else
	// we just burned 27 (29) cycles above, remove 7, (7*4=28)
	// to 2nd us is at least 10 so we can substract 7
	us -= 7; // = 2 cycles
#endif

#elif F_CPU >= 16000000L
	// for the 16 MHz clock on most Arduino boards

#if defined(__LGT8F__)
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"); // waiting 4 missing cycles in LGT
#endif
	// for a one-microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 1us
	if (us <= 1) return; // = 3 cycles, (4 when true) (LGT same)

	// the following loop takes 1/4 of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2; // x4 us, = 4 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 19 (21) cycles above, remove 5, (5*4=20)
	// to 2nd us is at least 8 so we can substract 5
#if defined(__LGT8F__)
	_NOP(); //compensate the missing cycle below
#endif
	us -= 5; // = 2 cycles,

#elif F_CPU >= 12000000L
	// for the 12 MHz clock if somebody is working with USB

#if defined(__LGT8F__)
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"); // waiting 4 missing cycles in LGT
#endif
	// for a 1 microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 1.5us
	if (us <= 1) return; // = 3 cycles, (4 when true) (LGT too)

	// the following loop takes 1/3 of a microsecond (4 cycles)
	// per iteration, so execute it three times for each microsecond of
	// delay requested.
	us = (us << 1) + us; // x3 us, = 5 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 20 (22) cycles above, remove 5, (5*4=20)
	// to 2nd us is at least 6 so we can substract 5
#if defined(__LGT8F__)
	_NOP(); //compensate the missing cycle below
#endif	us -= 5; //2 cycles

#elif F_CPU >= 8000000L
	// for the 8 MHz clock

#if defined(__LGT8F__)
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"); // waiting 4 missing cycles in LGT
#endif
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop"); //just waiting 2 cycles
	// for a 1 and 2 microsecond delay, simply return.  the overhead
	// of the function call takes 16 (18) cycles, which is 2us
	if (us <= 2) return; // = 3 cycles, (4 when true)

	// the following loop takes 1/2 of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.
	us <<= 1; //x2 us, = 2 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 19 (21) cycles above, remove 5, (5*4=20)
	// to 3rd us is at least 6 so we can substract 5
#if defined(__LGT8F__)
	_NOP(); //compensate the missing cycle below
#endif
	us -= 5; // = 2 cycles

#elif F_CPU >= 4000000L
	// for the 4 MHz clock

	__asm__ __volatile__ ("nop"); //just waiting 1 cycle
#if defined(__LGT8F__)
	// The overhead of the function call is 11 (13) cycles which is 3 us
	if (us <= 3) return;

	// Subtract microseconds that were wasted in this function
	_NOP(); //compensate the missing cycle below
	us -= 3;
#else
	// The overhead of the function call is 15 (17) cycles which is 4 us
	if (us <= 4) return;

	// Subtract microseconds that were wasted in this function
	us -= 4;
#endif
	// We don't need to multiply here because one request microsecond is exactly one loop cycle

#elif F_CPU >= 2000000L
	// for the 2 MHz clock

#if defined(__LGT8F__)
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"); // waiting 4 missing cycles in LGT
#endif
  // the overhead of the function call is 14 (16) cycles which is ~7 us
  // if someone request less than 11 us it will delay ~7 microseconds
  if (us <= 10) return;

  // Subtract microseconds (9) that were wasted in this function
  // so we can substract 9 ( every 2nd delay accurate 11 us and above)
#if defined(__LGT8F__)
	_NOP(); //compensate the missing cycle below
#endif
  us -= 9;  // = 2 cycles
  us >>= 1; // division by 2 = 2 cycles
  _NOP;     // waiting 1 cycle to adjust time
  
#else
	// for the 1 MHz (internal) clock (default settings for common Atmega microcontrollers)

	// the overhead of the function calls is 14 (16) cycles
	if (us <= 16) return; //= 3 cycles, (4 when true)
	if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)

	// compensate for the time taken by the preceeding and next commands (about 22 cycles)
	us -= 22; // = 2 cycles
	// the following loop takes 4 microseconds (4 cycles)
	// per iteration, so execute it us/4 times
	// us is at least 4, divided by 4 gives us 1 (no zero delay bug)
	us >>= 2; // us div 4, = 4 cycles

#endif

	// busy wait
	__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t"	// 2 cycles ( 1 cycle in LGT )
		    _ONENOP_		// 1 cycle if LGT
		    _MORENOP_		// 4 cycles if 32 MHz or 1 cycle if 25 MHz
		"   brne 1b" 		// 2 cycles ( 1 cycle when counter became 0 )
		: /* no outputs */
		: "w" (us)
	);
	// return = 4 cycles ( 2 cycles in LGT MCU )
}

void init()
{
	// this needs to be called before setup() or some functions won't
	// work there
	sei();
	
	// on the ATmega168, timer 0 is also used for fast hardware pwm
	// (using phase-correct PWM would mean that timer 0 overflowed half as often
	// resulting in different millis() behavior on the ATmega8 and ATmega168)
#if defined(TCCR0A) && defined(WGM01)
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);
#endif  

	// set timer 0 prescale factor to 64
#if defined(__AVR_ATmega128__)
	// CPU specific: different values for the ATmega128
	sbi(TCCR0, CS02);
#elif defined(TCCR0) && defined(CS01) && defined(CS00)
	// this combination is for the standard atmega8
	sbi(TCCR0, CS01);
	sbi(TCCR0, CS00);
#elif defined(TCCR0B) && defined(CS01) && defined(CS00)
	// this combination is for the standard 168/328/1280/2560
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);
#elif defined(TCCR0A) && defined(CS01) && defined(CS00)
	// this combination is for the __AVR_ATmega645__ series
	sbi(TCCR0A, CS01);
	sbi(TCCR0A, CS00);
#else
	#error Timer 0 prescale factor 64 not set correctly
#endif

	// enable timer 0 overflow interrupt
#if defined(TIMSK) && defined(TOIE0)
	sbi(TIMSK, TOIE0);
#elif defined(TIMSK0) && defined(TOIE0)
	sbi(TIMSK0, TOIE0);
#else
	#error	Timer 0 overflow interrupt not set correctly
#endif

	// timers 1 and 2 are used for phase-correct hardware pwm
	// this is better for motors as it ensures an even waveform
	// note, however, that fast pwm mode can achieve a frequency of up
	// 8 MHz (with a 16 MHz clock) at 50% duty cycle

#if defined(TCCR1B) && defined(CS11) && defined(CS10)
	TCCR1B = 0;

	// set timer 1 prescale factor to 64
	sbi(TCCR1B, CS11);
#if F_CPU >= 8000000L
	sbi(TCCR1B, CS10);
#endif
#elif defined(TCCR1) && defined(CS11) && defined(CS10)
	sbi(TCCR1, CS11);
#if F_CPU >= 8000000L
	sbi(TCCR1, CS10);
#endif
#endif
	// put timer 1 in 8-bit phase correct pwm mode
#if defined(TCCR1A) && defined(WGM10)
	sbi(TCCR1A, WGM10);
#endif

	// set timer 2 prescale factor to 64
#if defined(TCCR2) && defined(CS22)
	sbi(TCCR2, CS22);
#elif defined(TCCR2B) && defined(CS22)
	sbi(TCCR2B, CS22);
//#else
	// Timer 2 not finished (may not be present on this CPU)
#endif

	// configure timer 2 for phase correct pwm (8-bit)
#if defined(TCCR2) && defined(WGM20)
	sbi(TCCR2, WGM20);
#elif defined(TCCR2A) && defined(WGM20)
	sbi(TCCR2A, WGM20);
//#else
	// Timer 2 not finished (may not be present on this CPU)
#endif

#if defined(TCCR3B) && defined(CS31) && defined(WGM30)
	sbi(TCCR3B, CS31);		// set timer 3 prescale factor to 64
	sbi(TCCR3B, CS30);
	sbi(TCCR3A, WGM30);		// put timer 3 in 8-bit phase correct pwm mode
#endif

#if defined(TCCR4A) && defined(TCCR4B) && defined(TCCR4D) /* beginning of timer4 block for 32U4 and similar */
	sbi(TCCR4B, CS42);		// set timer4 prescale factor to 64
	sbi(TCCR4B, CS41);
	sbi(TCCR4B, CS40);
	sbi(TCCR4D, WGM40);		// put timer 4 in phase- and frequency-correct PWM mode	
	sbi(TCCR4A, PWM4A);		// enable PWM mode for comparator OCR4A
	sbi(TCCR4C, PWM4D);		// enable PWM mode for comparator OCR4D
#else /* beginning of timer4 block for ATMEGA1280 and ATMEGA2560 */
#if defined(TCCR4B) && defined(CS41) && defined(WGM40)
	sbi(TCCR4B, CS41);		// set timer 4 prescale factor to 64
	sbi(TCCR4B, CS40);
	sbi(TCCR4A, WGM40);		// put timer 4 in 8-bit phase correct pwm mode
#endif
#endif /* end timer4 block for ATMEGA1280/2560 and similar */	

#if defined(TCCR5B) && defined(CS51) && defined(WGM50)
	sbi(TCCR5B, CS51);		// set timer 5 prescale factor to 64
	sbi(TCCR5B, CS50);
	sbi(TCCR5A, WGM50);		// put timer 5 in 8-bit phase correct pwm mode
#endif

#if defined(ADCSRA)
	// set a2d prescaler so we are inside the desired 300-3000 kHz range.
	#if F_CPU >= 32000000 // 32 MHz / 16 = 2000 kHz : prescaler = 4
		sbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#elif F_CPU >= 16000000 // 16 MHz / 8 = 2000 kHz : prescaler = 3
		cbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#elif F_CPU >= 8000000 // 8 MHz / 4 = 2000 kHz : prescaler = 2
		cbi(ADCSRA, ADPS2);
		sbi(ADCSRA, ADPS1);
		cbi(ADCSRA, ADPS0);
	#else // Smallest prescaler is 1 (/2) : 4MHz=2000kHz, 2MHz=1000kHz, 1MHz=500kHz
		cbi(ADCSRA, ADPS2);
		cbi(ADCSRA, ADPS1);
		sbi(ADCSRA, ADPS0);
	#endif	
	// enable a2d conversions
	sbi(ADCSRA, ADEN);
#endif

	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()
#if defined(UCSRB)
	UCSRB = 0;
#elif defined(UCSR0B)
	UCSR0B = 0;
#endif
}
