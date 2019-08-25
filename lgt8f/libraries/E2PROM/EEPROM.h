/*
  EEPROM.h - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

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

#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>

#define	e2pReset()	do { ECCR |= 0x20; } while(0)
#define	e2pSWMON()	do { ECCR = 0x80; ECCR |= 0x10; } while(0);
#define	e2pSWMOFF()	do { ECCR = 0x80; ECCR &= 0xEF; } while(0);

class EEPROMClass
{
  public:
	EEPROMClass() { ECCR = 0x80; ECCR = 0x4C; };

	uint8_t read(uint16_t);
	void write(uint16_t, uint8_t);
	void read_block(uint8_t *, uint16_t, uint8_t);
	void write_block(uint8_t *, uint16_t, uint8_t);
	uint32_t read32(uint16_t);
	void write32(uint16_t, uint32_t);
	void writeSWM(uint16_t, uint32_t *, uint8_t);
	void readSWM(uint16_t, uint32_t *, uint8_t);
};

extern EEPROMClass EEPROM;

#endif

