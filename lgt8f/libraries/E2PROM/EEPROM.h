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

#if !defined( __LGT8F__ )
	#error !!! ERROR : This EEPROM library is designed for LGT8Fx microcontrolers !!!
#endif

#if !defined( __LGT8FX8P__ )
	#warning !!! WARNING : This EEPROM library was only tested with LGT8F328-P. 
	#warning If you find bugs and want to improve compatibility, please, join the GitHub repository.
#endif

#if defined( __LGT8FX8E__ ) && defined( __LGT8F_SSOP20__ ) || defined( ARDUINO_AVR_LARDU_88DS20 )
	#warning untested LGT8F88D-SSOP20 detected - which contains 2KB of seperated EEPROM : ( 32bits access and SWM access will be emulated )
	#define __LGT_EEPROM_LIB_FOR_88D__
#endif

#if defined( __LGT8FX8E__ ) && ! defined( __LGT8F_SSOP20__ ) && ! defined( ARDUINO_AVR_LARDU_88DS20 )
	#warning untested LGT8F328D  detected - which contains shared EEPROM with 16bits cells : ( 32bits access and SWM access will be emulated )
	
	#define __LGT_EEPROM_LIB_FOR_328D__
#endif


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

#if defined( __LGT8FX8P__ ) || defined( __LGT_EEPROM_LIB_FOR_328D__ )
	#define	lgt_eeprom_SWM_ON()   do { ECCR = 0x80; ECCR |= 0x10; } while(0);
	#define	lgt_eeprom_SWM_OFF()  do { ECCR = 0x80; ECCR &= 0xEF; } while(0);
	#define	lgt_eeprom_reset()    do { ECCR |= 0x20; } while(0)
#else
	#define lgt_eeprom_SWM_ON()
	#define lgt_eeprom_SWM_OFF()
	#define	lgt_eeprom_reset()
#endif

#if defined( __LGT8FX8P__ ) || defined( __LGT_EEPROM_LIB_FOR_328D__ )

	#define lgt_eeprom_free_space_per_1KB_page() ((uint16_t)1020)
	
	void lgt_eeprom_init( uint8_t number_of_1KB_pages = 1 );
	
	int lgt_eeprom_size( bool theoretical = false );
	uint16_t lgt_eeprom_continuous_address_to_real_address( uint16_t address );

	uint8_t lgt_eeprom_read_byte( uint16_t address, bool real_address_mode = false );
	void lgt_eeprom_write_byte( uint16_t address, uint8_t value, bool real_address_mode = false );

	void lgt_eeprom_read_block( uint8_t *pbuf, uint16_t address, uint16_t len, bool real_address_mode = false );
	void lgt_eeprom_write_block( uint8_t *pbuf, uint16_t address, uint16_t len, bool real_address_mode = false );
	
	// ----------------------------------------------------------------------
	// read/write native 32 bits data from/to E2PROM
	// ----------------------------------------------------------------------
	uint32_t lgt_eeprom_read32( uint16_t address );
	void lgt_eeprom_write32( uint16_t address, uint32_t value );

	// ----------------------------------------------------------------------
	// read/write bundle of data from/to E2PROM with SWM mode enable
	// ----------------------------------------------------------------------
	void lgt_eeprom_writeSWM( uint16_t address, uint32_t *pData, uint16_t length );
	void lgt_eeprom_readSWM( uint16_t address, uint32_t *pData, uint16_t length );
	
#else
	
	#define lgt_eeprom_init(x)
	
	int lgt_eeprom_size( bool theoretical = false );
	
	#define lgt_eeprom_continuous_address_to_real_address( address ) ((uint16_t)address)
	
	uint8_t lgt_eeprom_read_byte( uint16_t address );
	void lgt_eeprom_write_byte( uint16_t address, uint8_t value );

	void lgt_eeprom_read_block( uint8_t *pbuf, uint16_t address, uint16_t len );
	void lgt_eeprom_write_block( uint8_t *pbuf, uint16_t address, uint16_t len );
	
	// ----------------------------------------------------------------------
	// /!\ emulated - read/write native 32 bits data from/to E2PROM
	// ----------------------------------------------------------------------
	uint32_t lgt_eeprom_read32( uint16_t address );
	void lgt_eeprom_write32( uint16_t address, uint32_t value );

	// ----------------------------------------------------------------------
	// /!\ emualted - read/write bundle of data from/to E2PROM with SWM mode enable
	// ----------------------------------------------------------------------
	void lgt_eeprom_writeSWM( uint16_t address, uint32_t *pData, uint16_t length );
	void lgt_eeprom_readSWM( uint16_t address, uint32_t *pData, uint16_t length );
	
#endif



#if defined(__LGT8F88A__)
	#undef E2END
	#define E2END (504-1)
	
#elif defined(__LGT_EEPROM_LIB_FOR_88D__ )
	#undef E2END
	#define E2END (2048-1)
	
#elif defined( __LGT8FX8P__ ) || defined( __LGT_EEPROM_LIB_FOR_328D__ )
	#undef E2END
	#define E2END (lgt_eeprom_size()-1)
	
#endif


// ------------------ OLD LGT EEPROM API ---------------------------

#ifdef USE_LGT_EEPROM_API

	#if defined( __LGT8FX8P__ ) || defined( __LGT_EEPROM_LIB_FOR_328D__ )
		#define	e2pReset()	do { ECCR |= 0x20; } while(0)
		#define	e2pSWMON()	do { ECCR = 0x80; ECCR |= 0x10; } while(0);
		#define	e2pSWMOFF()	do { ECCR = 0x80; ECCR &= 0xEF; } while(0);
	#else
		#define	e2pReset()
		#define	e2pSWMON()
		#define	e2pSWMOFF()
	#endif


	#if defined( __LGT8FX8P__ ) || defined( __LGT_EEPROM_LIB_FOR_328D__ )
	
		class EEPROMClass
		{
		  public:
			EEPROMClass() { lgt_eeprom_init(); };

			uint8_t read( uint16_t address, bool real_address_mode = false ) { return lgt_eeprom_read_byte( address, real_address_mode ); }
			void   write( uint16_t address, uint8_t value, bool real_address_mode = false ) { lgt_eeprom_write_byte( address, value, real_address_mode ); }

			void  read_block( uint8_t *pbuf, uint16_t address, uint16_t len, bool real_address_mode = false ) { lgt_eeprom_read_block( pbuf, address, len, real_address_mode ); }
			void write_block( uint8_t *pbuf, uint16_t address, uint16_t len, bool real_address_mode = false ) { lgt_eeprom_write_block( pbuf, address, len, real_address_mode ); }

			uint32_t read32( uint16_t address ) { return lgt_eeprom_read32( address ); }
			void    write32( uint16_t address, uint32_t data ) { lgt_eeprom_write32( address, data ); }

			void writeSWM( uint16_t address, uint32_t *pdata, uint16_t len) { lgt_eeprom_writeSWM( address, pdata, len ); }
			void  readSWM( uint16_t address, uint32_t *pdata, uint16_t len) { lgt_eeprom_readSWM( address, pdata, len ); }

			int size( bool theoretical = false ) { return lgt_eeprom_size( theoretical ); }
			
			uint16_t change_size( uint8_t number_of_1KB_pages ) 
			{ 
				lgt_eeprom_size( number_of_1KB_pages ); 
				return lgt_eeprom_size(false); 
			}
		};
	#else
		class EEPROMClass
		{
		  public:
			EEPROMClass() { lgt_eeprom_init(); };

			uint8_t read( uint16_t address ) { return lgt_eeprom_read_byte( address ); }
			void   write( uint16_t address, uint8_t value ) { lgt_eeprom_write_byte( address, value ); }

			void  read_block( uint8_t *pbuf, uint16_t address, uint16_t len ) { lgt_eeprom_read_block( pbuf, address, len ); }
			void write_block( uint8_t *pbuf, uint16_t address, uint16_t len ) { lgt_eeprom_write_block( pbuf, address, len ); }

			uint32_t read32( uint16_t address ) { return lgt_eeprom_read32( address ); }
			void    write32( uint16_t address, uint32_t data ) { lgt_eeprom_write32( address, data ); }

			void writeSWM( uint16_t address, uint32_t *pdata, uint16_t len) { lgt_eeprom_writeSWM( address, pdata, len ); }
			void  readSWM( uint16_t address, uint32_t *pdata, uint16_t len) { lgt_eeprom_readSWM( address, pdata, len ); }
			

			int size( bool theoretical = false ) { return lgt_eeprom_size( theoretical ); }
			
			uint16_t change_size( uint8_t number_of_1KB_pages ) 
			{ 
				lgt_eeprom_size( number_of_1KB_pages ); 
				return lgt_eeprom_size(false); 
			}
		};
	#endif

#else // of #ifdef USE_LGT_EEPROM_API

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

struct EERef
{

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
	uint8_t operator++ (int)
	{ 
		uint8_t ret = **this;
		return ++(*this), ret;
	}
	
	uint8_t operator-- (int)
	{ 
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

struct EEPtr
{
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

struct EEPROMClass
{
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
	template< typename T > T &get( int idx, T &t )
	{
		EEPtr e = idx;
		uint8_t *ptr = (uint8_t*) &t;
		for( int count = sizeof(T) ; count ; --count, ++e )  *ptr++ = *e;
		return t;
	}
	
	template< typename T > const T &put( int idx, const T &t )
	{
		EEPtr e = idx;
		const uint8_t *ptr = (const uint8_t*) &t;
		for( int count = sizeof(T) ; count ; --count, ++e )  (*e).update( *ptr++ );
		return t;
	}

	uint16_t change_size( uint8_t number_of_1KB_pages ) 
	{ 
		lgt_eeprom_size( number_of_1KB_pages ); 
		return lgt_eeprom_size(false); 
	}

};


// -----------------------------------------------------------------

#endif // of #ifdef USE_LGT_EEPROM_API

static EEPROMClass EEPROM;

#endif

