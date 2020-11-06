/*
  EEPROM.h - EEPROM library
  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.
  Modifications from https://github.com/LGTMCU/Larduino_HSP
  New Modifications 20201106 by https://github.com/SuperUserNameMan/ :
	- add support for standard Arduino EEPROM API
	- add C-like lgt_eeprom_xxxx() API
	- put everything into a single EEPROM.h header file
	- old projects can keep using the original LGT EEPROM API by defining USE_LGT_EEPROM_API
	- add comments documenting how EEPROM emulation works with LGT8F328p

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
#include <Arduino.h>

/*
	README :

	3 API are available :
		- C-like lgt_eeprom_XXXX() API  (default)
		- standard Arduino EEPROM API (default)
		- old LGT EEPROM API

	The C-like lgt_eeprom_XXXX() API is always enabled.

	However, only one of the two EEPROM API can be used at a time.
	The standard Arduino EEPROM API is enabled by default.

	To choose the old LGT EEPROM API, you must :

		#define USE_LGT_EEPROM_API
		#include <EEPROM.h>

*/


// ------------------ LGT C-like lgt_eeprom_XXXX() API --------------------------

// ######## Note regarding EEPROM emulation on LGT8F328p ###########
// 
// On LGT8F328p, EEPROM is emulated using space from the main Flash
// memory. The emulation uses a technic of page swapping :
// - each time we write to the emulated EEPROM, the device erases the
// new page, copies the old data from the old page and updates the new 
// data on the fly on the new page, then it swaps the old and new page.
//
// Each page is a block of 1KB of the main Flash memory. This means
// that the emulation of a 1KB EEPROM actually uses 2KB from the main 
// Flash memory.
//
// To keep track of which page is new and which page is old, the device
// uses the 2 lasts bytes of each page to write a flag. This means that
// over 1024 bytes, only 1022 are theorically available to the user.
//
// Also, because the cells of the Flash memory are 32bits (4 bytes), 
// only 1020 bytes will be actually available.
//
// The size of the emulated EEPROM can be defined at runtime by setting
// bits 1 and 0 of the ECCR register ( see lgt_eeprom_init() ), or can
// be disabled by unsetting bit 6.
//
// However, we must keep in mind that the Flash memory required to emulate 
// the EEPROM is shared with the FLash memory used by the bootloader and 
// the sketch program, and that it will use twice the size of the emulated 
// EEPROM ! ( 8KB of EEPROM will use 16KB of Flash !).
//
// Also, it has to be noted that the content of the emulated EEPROM will
// be erased each time the sketch will be uploaded (even if it's an update).
//

#define lgt_eeprom_free_space_per_1KB_page() ((uint16_t)1020)

#define	lgt_eeprom_reset()    do { ECCR |= 0x20; } while(0)
#define	lgt_eeprom_SWM_ON()   do { ECCR = 0x80; ECCR |= 0x10; } while(0);
#define	lgt_eeprom_SWM_OFF()  do { ECCR = 0x80; ECCR &= 0xEF; } while(0);

void lgt_eeprom_init( uint8_t number_of_1KB_pages = 1 )
{
#ifdef __LGT8FX8P__ // TODO catch LGT8F32p only here
	switch( number_of_1KB_pages )
	{
		case 0: // disable EEPROM emulation
			ECCR = 0x80; 
			ECCR = 0x00;
		break;

		case 2: // 2KB EEPROM emulation (uses 4KB of data in main Flash memory )
			ECCR = 0x80; 
			ECCR = 0x4C | 0x01;
		break;

		case 4: // 4KB EEPROM emulation (uses 8KB of data in main Flash memory )
			ECCR = 0x80; 
			ECCR = 0x4C | 0x02;
		break;

		case 8: // 8KB EEPROM emulation (uses 16KB of data in main Flash memory )
			ECCR = 0x80; 
			ECCR = 0x4C | 0x03;
		break;
		
		default: // 1KB EEPROM emulation (uses 2KB of data in main Flash memory )
			ECCR = 0x80; 
			ECCR = 0x4C;
		break;
	}
#endif
}

int lgt_eeprom_size( bool theorical = false )
{
#ifdef __LGT8FX8P__ // TODO catch LGT8F32p only here
	if ( ECCR & 0x40 )
	{
		return theorical ?
			( 1024 << ( ECCR & 0x3 ) ) // thoerical size of the emulated EEPROM (see notes above)
			:
			( lgt_eeprom_free_space_per_1KB_page() << ( ECCR & 0x3 ) ) // actual number of bytes available to the user (see notes above)
			;
	}
#endif
	// TODO : detect LGT8F8  and return 2048
	// TODO : detect LGT8F16 and return 4096
	// TODO : detect LGT8F32d and return ???
	// TODO : detect LGT8F32e and return ???

	return 1024;
}

uint16_t lgt_eeprom_continuous_address_to_real_address( uint16_t address )
{
	// we recalculate the address so that we automatically skip 
	// every reserved two last bytes of every 1KB page (see notes above)

	if ( address >= lgt_eeprom_free_space_per_1KB_page() )
	{
		address = ( 1024 * ( address / lgt_eeprom_free_space_per_1KB_page() ) ) // selects the approriate 1KB page 
		        + ( address % lgt_eeprom_free_space_per_1KB_page() ) // selects the approriate byte on this 1KB page
		        ;
	}

	return address;
}

uint8_t lgt_eeprom_read_byte( uint16_t address, bool real_address_mode = false )
{
#ifdef __LGT8FX8P__ // TODO catch LGT8F32p only here
	if ( ! real_address_mode )
	{
		address = lgt_eeprom_continuous_address_to_real_address( address );
	}
#endif

	if ( address >= (uint16_t)lgt_eeprom_size( real_address_mode ) ) return 0;

	EEARL = address & 0xff;
	EEARH = (address >> 8); 
	 
	EECR |= (1 << EERE);
	__asm__ __volatile__ ("nop" ::);
	__asm__ __volatile__ ("nop" ::);
	
	return EEDR;	
}


void lgt_eeprom_write_byte( uint16_t address, uint8_t value, bool real_address_mode = true )
{
#ifdef __LGT8FX8P__ // TODO catch LGT8F32p only here
	if ( ! real_address_mode )
	{
		address = lgt_eeprom_continuous_address_to_real_address( address );
	}
#endif

	if ( address >= (uint16_t)lgt_eeprom_size( real_address_mode ) ) return;

	uint8_t	__bk_sreg = SREG;

	// set address & data
	EEARL = address & 0xff;
	EEARH = (address >> 8);
	EEDR = value;
	 
	cli();	
	EECR = 0x04;
	EECR = 0x02;
	SREG = __bk_sreg;
}

void lgt_eeprom_read_block( uint8_t *pbuf, uint16_t address, uint8_t len, bool real_address_mode = false )
{
	uint8_t i;

	uint8_t *p = pbuf;

	for(i = 0; i < len; i++) {
		*p++ = lgt_eeprom_read_byte(address+i, real_address_mode );
	}
}

void lgt_eeprom_write_block( uint8_t *pbuf, uint16_t address, uint8_t len, bool real_address_mode = false )
{
	uint8_t i;

	uint8_t *p = pbuf;

	for(i = 0; i < len; i++) {
		lgt_eeprom_write_byte( address+i, *p++, real_address_mode );
	}
}


#if defined(__LGT8FX8P__) // TODO catch LGT8F32p only here
uint32_t lgt_eeprom_read32( uint16_t address )
{
	uint32_t dwTmp;

	 EEARL = address & 0xff;
	 EEARH = (address >> 8);
	 
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

void lgt_eeprom_write32( uint16_t address, uint32_t value )
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
void lgt_eeprom_writeSWM( uint16_t address, uint32_t *pData, uint8_t length )
{
	uint8_t i;
	uint8_t __bk_sreg;

	lgt_eeprom_reset();
	lgt_eeprom_SWM_ON();

	EEARH = address >> 8;
	EEARL = address;

	for( i = 0; i < length; i++ ) 
	{
		if( i == (length - 1) )  // the last word
		{
			lgt_eeprom_SWM_OFF();
		}

		E2PD0 = (uint8_t) pData[i];
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
void lgt_eeprom_readSWM( uint16_t address, uint32_t *pData, uint8_t length )
{
	uint8_t i;

	lgt_eeprom_reset();
	lgt_eeprom_SWM_ON();

	EEARH = address >> 8;
	EEARL = address;

	for(i = 0; i < length; i++) 
	{
		if(i == (length - 1)) // the last word
		{
			lgt_eeprom_SWM_OFF();
		}

		EECR |= (1 << EERE);
	 	
		__asm__ __volatile__ ("nop" ::);
	 	__asm__ __volatile__ ("nop" ::);

		pData[i]  =  (uint32_t)E2PD0;
		pData[i] |= ((uint32_t)E2PD1 << 8);
		pData[i] |= ((uint32_t)E2PD2 << 16);
		pData[i] |= ((uint32_t)E2PD3 << 24);
	}
}
#endif

// ------------------ OLD LGT EEPROM API ---------------------------

#ifdef USE_LGT_EEPROM_API

#define	e2pReset()	do { ECCR |= 0x20; } while(0)
#define	e2pSWMON()	do { ECCR = 0x80; ECCR |= 0x10; } while(0);
#define	e2pSWMOFF()	do { ECCR = 0x80; ECCR &= 0xEF; } while(0);

class EEPROMClass
{
  public:
	EEPROMClass() { lgt_eeprom_init(); };

	uint8_t read( uint16_t address, bool real_address_mode = false ) { return lgt_eeprom_read_byte( address, real_address_mode ); }
	void   write( uint16_t address, uint8_t value, bool real_address_mode = false ) { lgt_eeprom_write_byte( address, value, real_address_mode ); }

	void  read_block( uint8_t *pbuf, uint16_t address, uint8_t len, bool real_address_mode = false ) { lgt_eeprom_read_block( pbuf, address, len, real_address_mode ); }
	void write_block( uint8_t *pbuf, uint16_t address, uint8_t len, bool real_address_mode = false ) { lgt_eeprom_write_block( pbuf, address, len, real_address_mode ); }

	uint32_t read32( uint16_t address ) { return lgt_eeprom_read32( address ); }
	void    write32( uint16_t address, uint32_t data ); { kgt_eeprom_write32( address, data ); }

	void writeSWM( uint16_t address, uint32_t *pdata, uint8_t len) { lgt_eeprom_writeSWM( address, pdata, len ); }
	void  readSWM( uint16_t address, uint32_t *pdata, uint8_t len) { lgt_eeprom_readSWM( address, pdata, len ); }
	
	int size() { return lgt_eeprom_size( false ); }
};

#else
// ------------------ ARDUINO EEPROM API ---------------------------

/*
  EEPROM.h - EEPROM library
  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.

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


/***
    EERef class.
    
    This object references an EEPROM cell.
    Its purpose is to mimic a typical byte of RAM, however its storage is the EEPROM.
    This class has an overhead of two bytes, similar to storing a pointer to an EEPROM cell.
***/

struct EERef{

    EERef( const int index )
        : index( index )                 {}
    
    //Access/read members.
    uint8_t operator*() const            { return lgt_eeprom_read_byte( index, false ); }
    operator uint8_t() const             { return **this; }
    
    //Assignment/write members.
    EERef &operator=( const EERef &ref ) { return *this = *ref; }
    EERef &operator=( uint8_t in )       { return lgt_eeprom_write_byte( index, in, false ), *this;  }
    EERef &operator +=( uint8_t in )     { return *this = **this + in; }
    EERef &operator -=( uint8_t in )     { return *this = **this - in; }
    EERef &operator *=( uint8_t in )     { return *this = **this * in; }
    EERef &operator /=( uint8_t in )     { return *this = **this / in; }
    EERef &operator ^=( uint8_t in )     { return *this = **this ^ in; }
    EERef &operator %=( uint8_t in )     { return *this = **this % in; }
    EERef &operator &=( uint8_t in )     { return *this = **this & in; }
    EERef &operator |=( uint8_t in )     { return *this = **this | in; }
    EERef &operator <<=( uint8_t in )    { return *this = **this << in; }
    EERef &operator >>=( uint8_t in )    { return *this = **this >> in; }
    
    EERef &update( uint8_t in )          { return  in != *this ? *this = in : *this; }
    
    /** Prefix increment/decrement **/
    EERef& operator++()                  { return *this += 1; }
    EERef& operator--()                  { return *this -= 1; }
    
    /** Postfix increment/decrement **/
    uint8_t operator++ (int){ 
        uint8_t ret = **this;
        return ++(*this), ret;
    }

    uint8_t operator-- (int){ 
        uint8_t ret = **this;
        return --(*this), ret;
    }
    
    int index; //Index of current EEPROM cell.
};

/***
    EEPtr class.
    
    This object is a bidirectional pointer to EEPROM cells represented by EERef objects.
    Just like a normal pointer type, this can be dereferenced and repositioned using 
    increment/decrement operators.
***/

struct EEPtr{

    EEPtr( const int index )
        : index( index )                {}
        
    operator int() const                { return index; }
    EEPtr &operator=( int in )          { return index = in, *this; }
    
    //Iterator functionality.
    bool operator!=( const EEPtr &ptr ) { return index != ptr.index; }
    EERef operator*()                   { return index; }
    
    /** Prefix & Postfix increment/decrement **/
    EEPtr& operator++()                 { return ++index, *this; }
    EEPtr& operator--()                 { return --index, *this; }
    EEPtr operator++ (int)              { return index++; }
    EEPtr operator-- (int)              { return index--; }

    int index; //Index of current EEPROM cell.
};

/***
    EEPROMClass class.
    
    This object represents the entire EEPROM space.
    It wraps the functionality of EEPtr and EERef into a basic interface.
    This class is also 100% backwards compatible with earlier Arduino core releases.
***/

struct EEPROMClass{

	EEPROMClass() { lgt_eeprom_init(1); }

    //Basic user access methods.
    EERef operator[]( const int idx )    { return idx; }
    uint8_t read( int idx )              { return EERef( idx ); }
    void write( int idx, uint8_t val )   { (EERef( idx )) = val; }
    void update( int idx, uint8_t val )  { EERef( idx ).update( val ); }
    
    //STL and C++11 iteration capability.
    EEPtr begin()                        { return 0x00; }
    EEPtr end()                          { return length(); } //Standards requires this to be the item after the last valid entry. The returned pointer is invalid.
    uint16_t length()                    { return E2END + 1; }
    
    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T &get( int idx, T &t ){
        EEPtr e = idx;
        uint8_t *ptr = (uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  *ptr++ = *e;
        return t;
    }
    
    template< typename T > const T &put( int idx, const T &t ){
        EEPtr e = idx;
        const uint8_t *ptr = (const uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  (*e).update( *ptr++ );
        return t;
    }

	int size() { return lgt_eeprom_size( false ); }

	// TODO : add a set_size() method ?
};


// -----------------------------------------------------------------

static EEPROMClass EEPROM;
#endif
#endif
