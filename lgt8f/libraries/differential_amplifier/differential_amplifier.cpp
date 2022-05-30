/*
  DAP.cpp - Differential Amplifier library
  Copyright (c) 2019 David Buezas.  All right reserved.

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

#include "differential_amplifier.h"
#include "wiring_analog.c"
/** DAPCR Differential Amplifier Control Register **/
// enable DAP |   gain  |    inverting     | non-inverting
// DAPEN      | GA1 GA0 | DNS2  DNS1  DNS0 | DPS1 DPS0

#define INVERTING_ADC2 0b000 << 2
#define INVERTING_ADC3 0b001 << 2
#if defined(__LGT8FX8P48__)
#define INVERTING_ADC8 0b010 << 2
#define INVERTING_ADC9 0b011 << 2
#endif
#define INVERTING_APN4 0b100 << 2  // PE0/APN4/SCWD, not normally exposed
#define INVERTING_MXER 0b101 << 2
#define INVERTING_GRND 0b110 << 2
#define INVERTING_CLOSE 0b111 << 2

#define NONINVERTING_MXER 0b00 << 0
#define NONINVERTING_ADC0 0b01 << 0
#define NONINVERTING_ADC1 0b10 << 0
#define NONINVERTING_GRND 0b11 << 0

/** ADCSRC ADC Control Status Register C **/
/*
| offset compensation | n/a | calibration | auto monitoring |
|        OFEN         |  x  |     SPN     |      AMEN       |
...
|  x  | SPD  |      DIFS         |  ADTM     |
| n/a | fast | 0=MXER 1=DIFF-AMP | test mode |
*/

#define DIFS_MXER (0UL << (1))
#define DIFS_DIFFAMP bit(DIFS)

int analogDiffRead(uint8_t negativePin, uint8_t positivePin, uint8_t gain) {
  uint8_t inverting;
  uint8_t muxedPin = A0;
  switch (negativePin) {
    case A2:
      inverting = INVERTING_ADC2;
      break;
    case A3:
      inverting = INVERTING_ADC3;
      break;
#if defined(__LGT8FX8P48__)
    case A8:
      inverting = INVERTING_ADC8;
      break;
    case A9:
      inverting = INVERTING_ADC9;
      break;
#endif
    default:
      inverting = INVERTING_MXER;
      muxedPin = negativePin;
  }
  uint8_t noninverting;
  switch (positivePin) {
    case A0:
      noninverting = NONINVERTING_ADC0;
      break;
    case A1:
      noninverting = NONINVERTING_ADC1;
      break;
    default:
      noninverting = NONINVERTING_MXER;
      muxedPin = positivePin;
  }
  if ( (negativePin == positivePin) || ((inverting == INVERTING_MXER) && (noninverting == NONINVERTING_MXER)) ) {
    // combination not allowed
    return -1;
  }
  // enable |   gain  |    inverting     | non-inverting
  // DAPEN  | GA1 GA0 | DNS2  DNS1  DNS0 | DPS1 DPS0
  DAPCR = bit(DAPEN) | gain | inverting | noninverting;  // get right side
  ADCSRC |= DIFS_DIFFAMP;  // set Diff. Amp. as source for the ADC
  // Note: analogRead() will configure the muxer even if the multiplexer is not
  // used by neither the inverting nor the noninverting pin, but that's fine
  // because the ADC won't read it
  int res = analogRead(muxedPin);
  DAPCR &= ~bit(DAPEN);     // disable Diff. Amp.
  ADCSRC &= ~DIFS_DIFFAMP;  // set back the Muxer as source for the ADC
  return res;
}
