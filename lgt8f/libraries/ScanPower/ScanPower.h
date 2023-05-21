/* Scanning on-chip devices under power (before sleep) */

#ifndef _AVR_SCANPOWER_H_
#define _AVR_SCANPOWER_H_ 1

#include <avr/io.h>
#include <stdint.h>

#define prw(something) pgm_read_word(something)
#define prb(something) pgm_read_byte(something)

#if ! defined(__LGT8F__)

  #define PRR1 _SFR_MEM8(0x65)

#endif

#if ! defined(__LGT8FX8P__)

  #define C1SR _SFR_MEM8(0x2F)

#endif

typedef struct {
  uint16_t raddress;
  uint8_t pmask;
  char   rname[38];
} adevice_t;

const adevice_t cdevices[] PROGMEM = {
  {0x64,0x01,"PRR ADC" },
  {0x64,0x02,"PRR USART" },
  {0x64,0x04,"PRR SPI interface" },
  {0x64,0x08,"PRR Timer/Counter1 module" },
  {0x64,0x20,"PRR Timer/Counter0 module" },
  {0x64,0x40,"PRR Timer/Counter2 module" },
  {0x64,0x80,"PRR TWI module" },
#if defined(__LGT8FX8P__)
  {0x65,0x02,"PRR1 Pin change logic" },
  {0x65,0x04,"PRR1 FLASH contoller interface" },
  {0x65,0x08,"PRR1 Timer/Counter3 module" },
  {0x65,0x20,"PRR1 Watchdog counter" },
#endif
  {0,0,""}
};

const adevice_t devices[] PROGMEM = {
  {0x4C,0x40,"SPCR SPI interface" },
  {0x50,0x80,"ACSR/C0SR analog comparator 0" },
  {0x60,0x48,"WDTCSR Watchdog module" },
  {0x7A,0x01,"ADCSRA ADC module" },
  {0x7C,0x80,"ADMUX Internal Vref. selected" },
  {0xBC,0x04,"TWCR TWI module" },
  {0xC1,0x08,"UCSRB USART Receiver module" },
  {0xC1,0x10,"UCSRB USART Transmitter module" },
#if defined(__LGT8FX8P__)
  {0x20,0x80,"DSCR uDSC module" },
  {0x2F,0x80,"C1SR analog comparator 1" },
  {0x54,0x80,"MCUSR SWD port" },
  {0x55,0x40,"MCUCR Flash under sleep" },
  {0x62,0x01,"VDTCR LVD module" },
  {0xA0,0x08,"DACON DAC module" },
  {0xAD,0x80,"ADCSRD Internal voltage reference" },
  {0xAD,0x40,"ADCSRD Int. Vref 4.096V selected" },
  {0xDC,0x80,"DAPCR Differential amplifier" },
  {0xEC,0x04,"TCKSCR 32 MHz freq. doubling output" },
  {0xF2,0x01,"PMCR Internal 32 MHz osc" },
  {0xF2,0x02,"PMCR Internal 32 kHz osc" },
  {0xF2,0x04,"PMCR External High frequecy osc" },
  {0xF2,0x08,"PMCR External low frequecy osc" },
#endif
  {0,0,""}
};
#endif

