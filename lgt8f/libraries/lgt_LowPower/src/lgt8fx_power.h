/* LGT8F328P variant of avr/power.h by Github/LaZSolt Copyright (c) 2023.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */


#ifndef _LGT8F_POWER2_
#define _LGT8F_POWER2_ 1

#include <avr/power.h>

/* ATmega328P compatibile funcions */
#define power2_spi_enable()  ( SPCR  |= (uint8_t) (1 << SPE) )
#define power2_spi_disable() ( SPCR  &= (uint8_t)~(1 << SPE) )

#define power2_usart0_enable()  ( UCSR0B |= (uint8_t) ((1<<RXEN0)|(1<<TXEN0)) )
#define power2_usart0_disable() ( UCSR0B &= (uint8_t)~((1<<RXEN0)|(1<<TXEN0)) )
#define power2_usart_enable()  power2_usart0_enable()
#define power2_usart_disable() power2_usart0_disable()

#define power2_twi_enable()  ( TWCR  |= (uint8_t) (1 << TWEN) )
#define power2_twi_disable() ( TWCR  &= (uint8_t)~(1 << TWEN) )

#define power2_adc_enable()  ( ADCSRA|= (uint8_t) (1 << ADEN) )
#define power2_adc_disable() ( ADCSRA&= (uint8_t)~(1 << ADEN) )

#if ! defined(power_usart_enable)
  #define power_usart_enable() power_usart0_enable()
#endif

#if ! defined(power_usart_disable)
  #define power_usart_disable() power_usart0_disable()
#endif

/* LGT8F328x compatibile funcions */
#if defined (__LGT8F__)

#if   defined(__LGT8FX8P__)
  #define LGT_PRR1_ALL_OFF 0x2E
#elif defined(__LGT8FX8E__)
  #define LGT_PRR1_ALL_OFF 0x26
#endif

#if defined(power_all_enable)
  #undef power_all_enable
#endif
#define power_all_enable() \
do {                       \
  __power_all_enable();    \
  PRR1 = 0;                \
} while (0)

#if defined(power_all_disable)
  #undef power_all_disable
#endif
#define power_all_disable()\
do {                       \
  __power_all_disable();   \
  PRR1 = LGT_PRR1_ALL_OFF; \
} while (0)

#if   defined(__LGT8FX8P__)
#define power_timer3_enable()          ( PRR1 &= (uint8_t)~(1 << PRTC3) )
/* Shuts down the Timer/Counter3 module by stopping the clock to the module.  */
#define power_timer3_disable()         ( PRR1 |= (uint8_t) (1 << PRTC3) )
#endif

#define power_pci_enable()             ( PRR1 &= (uint8_t)~(1 << PRPCI) )
/* Shuts down the external pin change and external interrupt module by stopping the clock to the modules.  */
#define power_pci_disable()            ( PRR1 |= (uint8_t) (1 << PRPCI) )

#define power_flashctrl_enable()       ( PRR1 &= (uint8_t)~(1 << PREFL) )
/* Shuts down the FLASH controller interface by stopping the clock to the module.  */
#define power_flashctrl_disable()      ( PRR1 |= (uint8_t) (1 << PREFL) )

#define power_wdtcc_enable()           ( PRR1 &= (uint8_t)~(1 << PRWDT) )
/* Shuts down the WDT Counter by stopping the clock to the module.  */
#define power_wdtcc_disable()          ( PRR1 |= (uint8_t) (1 << PRWDT) )

#define power2_swd_enable()  \
  do {                       \
      uint8_t _tmp_p2_swd_e_ = MCUSR | (uint8_t)~(1 << SWDD);   \
      MCUSR = 0x80;          \
      MCUSR = _tmp_p2_swd_e_;          \
  } while (0)

#define power2_swd_disable() \
  do {                       \
      MCUSR = 0x80;          \
      MCUSR = 0x80;          \
  } while (0)

#ifndef LDOEN
  #if   defined(__LGT8FX8P__)
    #define LDOEN PDEN
  #elif defined(__LGT8FX8E__)
    #define LDOEN PDE
  #endif
#endif

#define power2_ldo_disable() \
  do {                       \
      uint8_t _tmp_p2_ldo_d_ = LDOCR & (uint8_t)~(1 << LDOEN);  \
      LDOCR = 0x80;          \
      LDOCR = _tmp_p2_ldo_d_; \
  } while (0)

#define power2_ldo_enable()  \
  do {                       \
      uint8_t _tmp_p2_ldo_e_ = LDOCR | (uint8_t)(1 << LDOEN);  \
      LDOCR = 0x80;          \
      LDOCR = _tmp_p2_ldo_e_; \
  } while (0)

#if defined (__LGT8FX8P__)

#define power2_ac0_enable()  ( C0SR  &= (uint8_t)~(1 << C0D) )
  //When changing the C0D(ACD) bit, the analog comparator interrupt must be disabled by
  //clearing the C0IE bit in C0SR. Otherwise, an interrupt can occur when the bit is changed.
#define power2_ac0_disable() ( C0SR  |= (uint8_t) (1 << C0D) )

#define power2_ac1_enable()  ( C1SR  &= (uint8_t)~(1 << C1D) )
  //When changing the C0D(ACD) bit, the analog comparator interrupt must be disabled by
  //clearing the C1IE bit in C1SR. Otherwise, an interrupt can occur when the bit is changed.
#define power2_ac1_disable() ( C1SR  |= (uint8_t) (1 << C1D) )

#define power2_dac_enable()  ( DACON |= (uint8_t) (1 << DACEN) )
#define power2_dac_disable() ( DACON &= (uint8_t)~(1 << DACEN) )

#define power2_dap_enable()  ( DAPCR |= (uint8_t) (1 << DAPEN) )
#define power2_dap_disable() ( DAPCR &= (uint8_t)~(1 << DAPEN) )

#define power2_uDSC_enable()  ( DSCR |= (uint8_t) (1 << DSUEN) )
#define power2_uDSC_disable() ( DSCR &= (uint8_t)~(1 << DSUEN) )

#endif  // LGT8F328P

#if defined (__LGT8FX8E__)

#define power2_ac0_enable()   ( ACSR   &= (uint8_t)~(1 << ACD) )
#define power2_ac0_disable()  ( ACSR   |= (uint8_t) (1 << ACD) )

#define power2_ac1_enable()   ( AC1SR  &= (uint8_t)~(1 << ACD) )
#define power2_ac1_disable()  ( AC1SR  |= (uint8_t) (1 << ACD) )

#define power2_opa0_enable()  ( OP0CRA |= (uint8_t) (1 << OPAEN) )
#define power2_opa0_disable() ( OP0CRA &= (uint8_t)~(1 << OPAEN) )

#define power2_opa1_enable()  ( OP1CRA |= (uint8_t) (1 << OPAEN) )
#define power2_opa1_disable() ( OP1CRA &= (uint8_t)~(1 << OPAEN) )

#endif  // LGT8F328D

#endif  // LGT8F

#endif

