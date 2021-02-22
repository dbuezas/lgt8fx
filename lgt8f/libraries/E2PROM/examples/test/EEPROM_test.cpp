#include <EEPROM.h>

void print_hex( int d )
{
	if ( d <= 0x0f ) Serial.print(0);
	Serial.print( d, HEX );
}

static uint32_t rng_state = 1;

void rngSeed( uint32_t seed )
{
	if ( seed == 0 ) seed = 123;
	rng_state = seed;
}

uint32_t rng()
{
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = rng_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return rng_state = x;
}


void test_writing( unsigned int COLS )
{
	Serial.println(F("Writing using EEPROM[] = xx : "));
	
	rngSeed(0);

	for( uint16_t a=0; a< EEPROM.length(); a++ )
	{
		EEPROM[a] = rng() & 0xff; 
		
		print_hex( EEPROM[a] );
		Serial.print(F("."));

		if ( ( a % COLS ) == ( COLS-1)  ) Serial.println();
	}

}
