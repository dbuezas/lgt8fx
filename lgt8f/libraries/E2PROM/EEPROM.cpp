#include "Arduino.h"
#include "EEPROM.h"

void lgt_eeprom_init( uint8_t number_of_1KB_pages )
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

int lgt_eeprom_size( bool theoretical )
{
#ifdef __LGT8FX8P__ // TODO catch LGT8F32p only here

	if ( ECCR & 0x40 ) // EEPROM emulation enabled ?
	{
		return theoretical ?
			( 1024 << ( ECCR & 0x3 ) ) // thoerical size of the emulated EEPROM (see notes above)
			:
			( lgt_eeprom_free_space_per_1KB_page() << ( ECCR & 0x3 ) ) // actual number of bytes available to the user (see notes above)
			;
	}

	return 0;
	
#else
	// TODO : detect LGT8F8  and return 2048
	// TODO : detect LGT8F16 and return 4096
	// TODO : detect LGT8F32d and return ???
	// TODO : detect LGT8F32e and return ???

	return 1024;
#endif
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

uint8_t lgt_eeprom_read_byte( uint16_t address, bool real_address_mode )
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

void lgt_eeprom_write_byte( uint16_t address, uint8_t value, bool real_address_mode )
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

void lgt_eeprom_read_block( uint8_t *pbuf, uint16_t address, uint8_t len, bool real_address_mode )
{
	uint8_t i;

	uint8_t *p = pbuf;

	for(i = 0; i < len; i++) {
		*p++ = lgt_eeprom_read_byte(address+i, real_address_mode );
	}
}

void lgt_eeprom_write_block( uint8_t *pbuf, uint16_t address, uint8_t len, bool real_address_mode )
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



