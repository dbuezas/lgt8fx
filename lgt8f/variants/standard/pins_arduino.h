/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

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

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            20
#define NUM_ANALOG_INPUTS           6

#if defined(__LGT8FX8P48__)
#define analogInputToDigitalPin(p)  ((p < 12) ? (p) + 14 : -1)
#elif defined(__LGT8FX8P__) || defined(__LGT8FX8E__)
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 14 : -1)
#else
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)
#endif

#if defined(__LGT8FX8E__)
#define LGT_NOT_DACO(pin) ((pin != DAC0) && (pin != DAC1))
#elif defined(__LGT8FX8P__)
#define LGT_NOT_DACO(pin) (pin != DAC0)
#else
#define LGT_NOT_DACO(pin) 1
#endif

#if defined(__AVR_ATmega8__)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11)
#else
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)
#endif

#if defined(__LGT8F_SSOP20__)
#define PIN_SPI_SS    (9)          // Redirected in main.cpp at line 77
#else
#define PIN_SPI_SS    (10)
#endif
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (18)
#define PIN_WIRE_SCL        (19)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 	(13)

#define PIN_A0   (14)
#define PIN_A1   (15)
#define PIN_A2   (16)
#define PIN_A3   (17)
#define PIN_A4   (18)
#define PIN_A5   (19)
#define PIN_A6   (20)
#define PIN_A7   (21)

static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t A4 = 18;
static const uint8_t A5 = 19;
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
static const uint8_t A6 = 20;
static const uint8_t A7 = 21;
#if defined(__LGT8FX8P48__)
static const uint8_t A8 = 23;
static const uint8_t A9 = 24;
// static const uint8_t A10 = 25; LGT8FXP and LGT8FXP48 both have A10 pin
static const uint8_t A11 = 26;
#define PIN_A8   (23)
#define PIN_A9   (24)
#define PIN_A11  (26)
#endif
#if defined(__LGT8FX8P__)
static const uint8_t A10 = 25;
#define PIN_A10   (25)
#endif
#endif

#define D0	0	/* PD0 */
#define D1	1	/* PD1 */
#define D2	2	/* PD2 */
#define D3	3	/* PD3 */
#define D4	4	/* PD4 */
#define D5	5	/* PD5 */
#define D6	6	/* PD6 */
#define D7	7	/* PD7 */

#define D8	8	/* PB0 */
#define D9	9	/* PB1 */
#define D10	10	/* PB2 */
#define D11	11	/* PB3 */
#define D12	12	/* PB4 */
#define D13	13	/* PB5 */

#define C0	14
#define C1	15
#define C2	16
#define C3	17
#define C4	18
#define C5	19

#define D14	14	/* PC0 */
#define D15	15	/* PC1 */
#define D16	16	/* PC2 */
#define D17	17	/* PC3 */
#define D18	18	/* PC4 */
#define D19	19	/* PC5 */

#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
#define E1	20	/* A6 */
#define E3	21	/* A7 */
#define D20	20	/* A6 */
#define D21	21	/* A7 */
#if defined(__LGT8FX8P48__)
#define C7	23
#define	F0	24
#define	E6	25
#define	E7	26
#define D22 	22	/* PB6 */
#define D23 	23	/* A8 - PC7 */
#define D24 	24	/* A9 - PF0 */
#define D25 	25	/* A10 - PE6 */
#define D26 	26	/* A11 - PE7 */
#define	C6	28
#define E0	29 
#define E2	30
#define E4	31
#define E5	32
#define	F1	33
#define	F2	34
#define	F3	35
#define	F4	36
#define	F5	37
#define	F6	38
#define	F7	39

#define D27	27	/* PB7 */
#define	D28	28	/* PC6 */
#define D29	29 	/* PE0 */
#define D30	30	/* PE2 */
#define D31	31	/* PE4 */
#define D32	32	/* PE5 */
#define	D33	33	/* PF1 */
#define	D34	34	/* PF2 */
#define	D35	35	/* PF3 */
#define	D36	36	/* PF4 */
#define	D37	37	/* PF5 */
#define	D38	38	/* PF6 */
#define	D39	39	/* PF7 */
#else
#define E0	22
#define E2	23
#define E4	24
#define E5	26
#define E6	25

#define D22	22 	/* PE0 */
#define D23	23	/* PE2 */
#define D24	24	/* PE4 */
#define D25	26	/* PE5 */
#define D26	25	/* PE6 */
#define C6      27      /* PC6 */
#endif
#endif

#if defined(__LGT8FX8E__)
#define VCCM	22
#define	AOFF	24
#define	IVREF	28
#define	AGND	29
#elif defined(__LGT8FX8P__)
#define VCCM	22
#define V5D1	22
#define	IVREF	27
#define	V5D4	28
#define	AGND	29
#define	DACO	30
#define	PGAO	32
#endif

#if defined(__LGT8F__)
#define XCK_DDR   DDRD
#define XCK_PORT  PORTD
#define XCK_PIN   PIND
#define XCK_BIT   (4)
#endif

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
	(uint16_t) &DDRE,
#endif
#if defined(__LGT8FX8P48__)
	(uint16_t) &DDRF,
#endif
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
	(uint16_t) &PORTE,
#endif
#if defined(__LGT8FX8P48__)
	(uint16_t) &PORTF,
#endif
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
	(uint16_t) &PINE,
#endif
#if defined(__LGT8FX8P48__)
	(uint16_t) &PINF,
#endif
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PB, /* 8 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PC, /* 14 */
	PC,
	PC,
	PC,
	PC,
	PC,
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
	PE, /* 20 */
	PE, /* 21 */
#if defined(__LGT8FX8P48__)
	PB, /* 22 */
	PC, /* 23 */
	PF, /* 24 */
	PE, /* 25 */
	PE, /* 26 */
	PB, /* 27 */
	PC, /* 28 */
	PE, /* 29 */
	PE, /* 30 */
	PE, /* 31 */
	PE, /* 32 */
	PF, /* 33 */
	PF, /* 34 */
	PF, /* 35 */
	PF, /* 36 */
	PF, /* 37 */
	PF, /* 38 */
	PF, /* 39 */
#else
	PE, /* 22 */
	PE, /* 23 */
	PE, /* 24 */
	PE, /* 25 */
	PE, /* 26 */
	PC, /* 27 */
#endif
#endif
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(0), /* 14, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
	_BV(1), /* 20, port E1 */
	_BV(3), /* 21, port E3 */
#if defined(__LGT8FX8P48__)
	_BV(6), /* 22, port B6 */
	_BV(7), /* 23, port C7 */
	_BV(0), /* 24, port F0 */ 
	_BV(6), /* 25, port E6 */
	_BV(7), /* 26, port E7 */
	_BV(7), /* 27, port B7 */
	_BV(6), /* 28, port C6 */
	_BV(0), /* 29, port E0 */
	_BV(2), /* 30, port E2 */
	_BV(4), /* 31, port E4 */
	_BV(5), /* 32, port E5 */
	_BV(1), /* 33, port F1 */
	_BV(2), /* 34, port F2 */
	_BV(3), /* 35, port F3 */
	_BV(4), /* 36, port F4 */
	_BV(5), /* 37, port F5 */
	_BV(6), /* 38, port F6 */
	_BV(7), /* 39, port F7 */
#else
	_BV(0), /* 22, port E0 */
	_BV(2), /* 23, port E2 */ 
	_BV(4), /* 24, port E4 */
	_BV(6), /* 25, port E6 */
	_BV(5), /* 26, port E5 */
	_BV(6), /* 27, port C6 */
#endif
#endif
};

#if defined(__LGT8FX8P__)
const uint8_t PROGMEM port_to_PCMSK_PGM[] = { 0, 0, 0xFF & (uint16_t)&PCMSK0, 0xFF & (uint16_t)&PCMSK1, 0xFF & (uint16_t)&PCMSK2, 0xFF & (uint16_t)&PCMSK3, 0xFF & (uint16_t)&PCMSK4 };
#else
const uint8_t PROGMEM port_to_PCMSK_PGM[] = { 0, 0, 0xFF & (uint16_t)&PCMSK0, 0xFF & (uint16_t)&PCMSK1, 0xFF & (uint16_t)&PCMSK2, 0xFF & (uint16_t)&PCMSK3 };
#endif

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	// on the ATmega168, digital pin 3 has hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
#else
	TIMER2B,
#endif
#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
	LGTDAO0,
#else
	NOT_ON_TIMER,
#endif
	// on the ATmega168, digital pins 5 and 6 have hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
	NOT_ON_TIMER,
#else
	TIMER0B,
	TIMER0A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 8 - port B */
	TIMER1A,
	TIMER1B,
#if defined(__AVR_ATmega8__)
	TIMER2,
#else
	TIMER2A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 14 - port C */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
#if defined(__LGT8FX8E__)
	LGTDAO1,
#endif
};

#endif

#define digitalPinToPCMSK(p) (uint8_t*)pgm_read_byte(port_to_PCMSK_PGM+pgm_read_byte(digital_pin_to_port_PGM+(p)))
#define digitalPinToPCICRbit(p) (pgm_read_byte(digital_pin_to_port_PGM+(p))-2)
#define digitalPinToPCMSKbit(n) __builtin_ctz(pgm_read_byte(digital_pin_to_bit_mask_PGM+(n)))
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 39) ? (&PCICR) : ((uint8_t *)0))

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
