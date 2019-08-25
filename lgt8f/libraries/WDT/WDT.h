/*
  WDT.h - WDT library
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

#ifndef WDT_h
#define WDT_h

#include <avr/wdt.h>
#include "Arduino.h"

#if defined(__LGT8FX8P__)
#define	WTO_64MS	0	
#define WTO_128MS	1
#define	WTO_256MS	2
#define	WTO_512MS	3
#define	WTO_1S		4
#define	WTO_2S		5
#define	WTO_4S		6
#define	WTO_8S		7
#define	WTO_16S		8
#define	WTO_32S		9
#else
#define	WTO_1MS		0
#define	WTO_2MS		1
#define WTO_4MS		2
#define WTO_8MS		3
#define WTO_16MS	4
#define	WTO_64MS	6	
#define WTO_128MS	7
#define	WTO_256MS	8
#define	WTO_512MS	9
#endif

#if defined(__LGT8FX8P__)
class LGTWDT
{
	public:
		static void begin() __attribute__((always_inline)) { 
			uint8_t btmp = PMCR | 0x10;
			PMCR = 0x80;
			PMCR = btmp;
		}
};
#endif

#endif
