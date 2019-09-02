/*
  DAP.h - Differential Amplifier library
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

#include <inttypes.h>

#define GAIN_1 0b00 << 5
#define GAIN_8 0b01 << 5
#define GAIN_16 0b10 << 5
#define GAIN_32 0b11 << 5

/*
Reads the amplified difference between two pins.

 \param negativePin The pin (A0-A9) with the lowest voltage.
 \param positiveePin The pin (A0-A9) with the highest voltage.
 \param gain The gain. Use constants GAIN_1, GAIN_8, GAIN_16, GAIN_32
 \returns  int: (positivePin - negativePin) * gain if combination is available
 \returns  -1 if combination is NOT available

 Available combinations:
| -\+ | A0  | A1  | A2  | A3  | A4  | A5  | A6  | A7  | A8  | A9  |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| A0  |     | ✓   |     |     |     |     |     |     |     |     |
| A1  | ✓   |     |     |     |     |     |     |     |     |     |
| A2  | ✓   | ✓   |     | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   |
| A3  | ✓   | ✓   | ✓   |     | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   |
| A4  | ✓   | ✓   |     |     |     |     |     |     |     |     |
| A5  | ✓   | ✓   |     |     |     |     |     |     |     |     |
| A6  | ✓   | ✓   |     |     |     |     |     |     |     |     |
| A7  | ✓   | ✓   |     |     |     |     |     |     |     |     |
| A8  | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   |     | ✓   |
| A9  | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   | ✓   |     |
*/
int analogDiffRead(uint8_t negativePin, uint8_t positivePin, uint8_t gain);
