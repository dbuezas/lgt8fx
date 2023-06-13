/* LGT8F328P variant of avr/sleep.h by Github/LaZSolt Copyright (c) 2023.
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


#ifndef _LGT8F_SLEEP_
#define _LGT8F_SLEEP_ 1

#if defined (__LGT8F__)

#include <avr/sleep.h>

#if defined(__LGT8FX8E__)
  // Sleep FLASH during sleep bit for 328D
  #ifndef FPDEN
    #define FPDEN 6
  #endif

#elif defined(__LGT8FX8P__)

  #define SLEEP_MODE_DPS2        (0x07<<1)

                                 /* SLEEP_128MS = 1, SLEEP_256MS = 2, SLEEP_512MS = 3, SLEEP_1S = 4 */
#define set_DPS2_timer(_period_)         ( DPS2R = (DPS2R & (uint8_t) (1 << DPS2E)) | (1 << LPRCE) | ( 0x03 & ~((uint8_t)(_period_))) )

#define DPS2_enable()                    ( DPS2R |= (uint8_t) (1<< DPS2E) )

#define DPS2_disable()                   ( DPS2R = 0 )

#define DPS2_enable_with_timer(_period_) ( DPS2R = (uint8_t) (1 << DPS2E) | (1 << LPRCE) | ( 0x03 & ~((uint8_t)(_period_))) )

#define DPS2_enable_timer_disable()      ( DPS2R = (uint8_t) (1 << DPS2E) )

#endif



#define sleep_lvd_disable()  \
  do {                       \
      uint8_t _tmp_sleep_lvd_disable_ = VDTCR & ~(1 << VDTEN);  \
      VDTCR = 0x80;          \
      VDTCR = _tmp_sleep_lvd_disable_; \
  } while (0)

#if defined(sleep_bod_disable)
  #undef sleep_bod_disable
#endif
#define sleep_bod_disable() sleep_lvd_disable()

#if defined(__LGT8FX8E__)

#define sleep_flash_disable()\
  do {                       \
	  MCUCR |= (uint8_t) (1 << FPDEN); \
  } while (0)
	  
#define sleep_flash_enable() \
  do {                       \
	  MCUCR &= (uint8_t)~(1 << FPDEN); \
  } while (0)

#elif defined(__LGT8FX8P__)

#define sleep_flash_disable()\
  do {                       \
      uint8_t _tmp_fe_ = MCUCR | (uint8_t) (1 << FPDEN); \
      MCUCR = 1 << MWCE;     \
      MCUCR = _tmp_fe_;      \
  } while (0)
	  
#define sleep_flash_enable() \
  do {                       \
      uint8_t _tmp_fd_ = MCUCR & (uint8_t)~(1 << FPDEN); \
      MCUCR = 1 << MWCE;     \
      MCUCR = _tmp_fd_;      \
  } while (0)

#endif

#endif

#endif

