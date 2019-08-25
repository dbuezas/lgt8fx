/*
  EEPROM.cpp - EEPROM library
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

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Arduino.h"
#include "EEPROM.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

/******************************************************************************
 * User API
 ******************************************************************************/

uint8_t EEPROMClass::read(uint16_t address)
{
	 EEARL = address & 0xff;
	 EEARH = (address >> 8) & 0x1;
	 
	 EECR |= (1 << EERE);
	 __asm__ __volatile__ ("nop" ::);
	 __asm__ __volatile__ ("nop" ::);
	 
	 return EEDR;
	
}

void EEPROMClass::write(uint16_t address, uint8_t value)
{
	uint8_t	__bk_sreg = SREG;

	// set address & data
	EEARL = address & 0xff;
	EEARH = (address >> 8) & 0x1;
	EEDR = value;
	 
	cli();	
	EECR = 0x04;
	EECR = 0x02;
	SREG = __bk_sreg;
}

void EEPROMClass::read_block(uint8_t *pbuf, uint16_t address, uint8_t len)
{
	uint8_t i;

	uint8_t *p = pbuf;

	for(i = 0; i < len; i++) {
		*p++ = read(address+i);
	}
}

void EEPROMClass::write_block(uint8_t *pbuf, uint16_t address, uint8_t len)
{
	uint8_t i;

	uint8_t *p = pbuf;

	for(i = 0; i < len; i++) {
		write(address+i, *p++);
	}
}

#if defined(__LGT8FX8P__)
uint32_t EEPROMClass::read32(uint16_t address)
{
	uint32_t dwTmp;

	 EEARL = address & 0xff;
	 EEARH = (address >> 8) & 0x1;
	 
	 EECR |= (1 << EERE);
	
	 __asm__ __volatile__ ("nop" ::);
	 __asm__ __volatile__ ("nop" ::);
	
	dwTmp = E2PD0;
	dwTmp |= ((uint32_t)E2PD1 << 8);
	dwTmp |= ((uint32_t)E2PD2 << 16);
	dwTmp |= ((uint32_t)E2PD3 << 24);

	// return data from data register
	return dwTmp;	
}

void EEPROMClass::write32(uint16_t address, uint32_t value)
{
	uint8_t __bk_sreg = SREG;

	EEARL = 0;
	EEDR = value;
	EEARL = 1;
	EEDR = value >> 8;
	EEARL = 2;
	EEDR = value >> 16;
	EEARL = 3;
	EEDR = value >> 24;

	EEARH = address >> 8;
	EEARL = address & 0xff;

	// Program Mode
	cli();
	EECR = 0x44;
	EECR = 0x42;
	
	SREG = __bk_sreg;	
}

// ----------------------------------------------------------------------
// public: write bundle of data to E2PROM with SWM mode enable
// ----------------------------------------------------------------------
void EEPROMClass::writeSWM(uint16_t address, uint32_t *pData, uint8_t length)
{
	uint8_t i;
	uint8_t __bk_sreg;

	e2pReset();
	e2pSWMON();

	EEARH = address >> 8;
	EEARL = address;

	for(i = 0; i < length; i++) {
		if(i == (length - 1)) // the last word
			e2pSWMOFF();

		E2PD0 = (uint8_t)pData[i];
		E2PD1 = (uint8_t)(pData[i] >> 8);
		E2PD2 = (uint8_t)(pData[i] >> 16);
		E2PD3 = (uint8_t)(pData[i] >> 24);

		__bk_sreg = SREG;
		cli();

		EECR = 0x44;
		EECR = 0x42;
		SREG = __bk_sreg;	
	}
}

// ----------------------------------------------------------------------
// public: read bundle of data to E2PROM with SWM mode enable
// ----------------------------------------------------------------------
void EEPROMClass::readSWM(uint16_t address, uint32_t *pData, uint8_t length)
{
	uint8_t i;

	e2pReset();
	e2pSWMON();

	EEARH = address >> 8;
	EEARL = address;

	for(i = 0; i < length; i++) {
		if(i == (length - 1)) // the last word
			e2pSWMOFF();

		EECR |= (1 << EERE);
	 	
		__asm__ __volatile__ ("nop" ::);
	 	__asm__ __volatile__ ("nop" ::);

		pData[i] = (uint32_t)E2PD0;
		pData[i] |= ((uint32_t)E2PD1 << 8);
		pData[i] |= ((uint32_t)E2PD2 << 16);
		pData[i] |= ((uint32_t)E2PD3 << 24);
	}
}
#endif

EEPROMClass EEPROM;
