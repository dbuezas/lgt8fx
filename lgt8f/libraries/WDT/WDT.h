/**************************************************************************
 * WDT.cpp - Watchdog library for LogicGreen LGT8F328x microcontrollers   *
 *                                                                        *
 * This library is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU Lesser General Public             *
 * License as published by the Free Software Foundation; either           *
 * version 2.1 of the License, or (at your option) any later version.     *
 *                                                                        *
 * This library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 * Lesser General Public License for more details.                        *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with this library; if not, write to the Free Software    *
 * Foundation, Inc., 51 Franklin St,Fifth Floor,Boston, MA 02110-1301 USA *
 *                                                                        *
 * Updated on August 2023 by gpb01 to add the capability to use the       *
 * internal HFRC oscillator for the WTD. Default is the standard LFRC.    *
 **************************************************************************/

#ifndef WDT_h
#define WDT_h

#include "Arduino.h"
#include <avr/wdt.h>

#define WTOH_32MHZ 0
#define WTO_32KHZ 1

#define WTOH_1MS 0
#define WTOH_2MS 1
#define WTOH_4MS 2
#define WTOH_8MS 3
#define WTOH_16MS 4
#define WTOH_32MS 5
#define WTOH_64MS 6
#define WTOH_128MS 7
#define WTOH_256MS 8
#define WTOH_512MS 9

#define WTO_64MS 0
#define WTO_128MS 1
#define WTO_256MS 2
#define WTO_512MS 3
#define WTO_1S 4
#define WTO_2S 5
#define WTO_4S 6
#define WTO_8S 7
#define WTO_16S 8
#define WTO_32S 9

class LGTWDT {
public:
  static void begin(uint8_t clockSource = WTO_32KHZ)
      __attribute__((always_inline)) {
    uint8_t btmp = 0;

    if (clockSource == WTOH_32MHZ) {
      // enable 32MHz RC for WDT
      btmp = PMCR | (1 << RCMEN);  // 0000 0001 (enable RCMEN)
      btmp = btmp & ~(1 << WCLKS); // 1110 1111 (select HFRC)
      PMCR = (1 << PMCE);          // 1000 0000
      PMCR = btmp;
    } else {
      // enable 32KHz RC for WDT
      btmp = PMCR | (1 << RCKEN); // 0000 0010 (enable RCKEN)
      btmp = btmp | (1 << WCLKS); // 0001 0000 (select LFRC)
      PMCR = (1 << PMCE);         // 1000 0000
      PMCR = btmp;
    }
  }
};

static void __inline__ __attribute__((__always_inline__))
wdt_ienable(const uint8_t value) {
  __asm__ __volatile__(
      "in     __tmp_reg__,__SREG__    \n\t"
      "cli                            \n\t"
      "wdr                            \n\t"
      "sts    %[wdtcsr], %[wdce_wde]  \n\t"
      "out    __SREG__,__tmp_reg__    \n\t"
      "sts    %[wdtcsr], %[wdie_val]  \n\t"
      : /* no outputs */
      : [wdtcsr] "n"(_SFR_MEM_ADDR(WDTCSR)),
        [wdce_wde] "r"((uint8_t)((1 << WDCE) | (1 << WDE))),
        [wdie_val] "r"((uint8_t)((1 << WDIE) | (value & 0x08 ? 0x20 : 0x00) |
                                 (value & 0x07)))
      : "r0");
}

volatile uint8_t o_sreg __attribute__((section(".noinit")));
volatile uint8_t s_tempreg __attribute__((section(".noinit")));

void softRepairedWdtIsr(void) __attribute__((naked)) __attribute__((used))
__attribute__((section(".init0")));
void softRepairedWdtIsr(void) {
  __asm__ __volatile__(
      "sts    (s_tempreg), r24\n\t" // s_tempreg = uint8_t register temp_reg;
                                    // Save a register
      "in     r24, __SREG__   \n\t" // o_sreg = SREG;
      "sts    (o_sreg)  , r24 \n\t"
      "lds    r24, %[wdtcsr]  \n\t" // temp_reg = WDTCSR;
#if defined(__LGT8FX8P__)
      "sbrs   r24, %[wdif]    \n\t" // if (temp_reg & (1 << WDIF))
#else
      "sbrs   r24, %[wdie]    \n\t" // if (temp_reg & (1 << WDIE))
#endif
      "rjmp   1f              \n\t"
      // WDT interrupt
      "andi   r24, %[nwdie]   \n\t" // WDTCSR = temp_reg & ~(1 << WDIE);
      "sts    %[wdtcsr] , r24 \n\t"
      "lds    r24, (o_sreg)   \n\t" // SREG = o_sreg;
      "out    __SREG__  , r24 \n\t"
      "lds    r24, (s_tempreg)\n\t" // temp_reg = s_tempreg;
      "jmp    __vector_6      \n\t"
      "1:                     \n\t"

      : /* No outputs */
      : [wdtcsr] "n"(_SFR_MEM_ADDR(WDTCSR)), [wdif] "i"(WDIF), [wdie] "i"(WDIE),
        [nwdie] "i"(~(1 << WDIE)) );

  /*
      o_sreg = SREG;

      uint8_t temp_reg = WDTCSR;

      if (temp_reg & (1 << WDIF)) {
        // WDT interrupt
          WDTCSR = temp_reg & ~(1 << WDIE);
          SREG = o_sreg;
          __asm__ __volatile__ ("jmp __vector_6 \n");
      }
  */
}

extern LGTWDT Lgtwdt;

#endif
