#include <EEPROM.h>

#define TEST_ARDUINO_EEPROM_API

void test_writing( unsigned int cols ); // from EEPROM_test.cpp to test multiple inclusion


#define COLS 64

void print_hex( int d );
uint32_t rng();
void rngSeed(uint32_t seed);
int err;
byte *buff;

void setup()
{
	Serial.begin(9600);

	while(!Serial);
	
	// -----------------------------------------------------------------
	
	#if defined( __LGT8FX8E__ ) && defined( __LGT8F_SSOP20__ ) || defined( ARDUINO_AVR_LARDU_88DS20 )
		Serial.println(F("LGT8F88D-SSOP20 detected"));
		#define BOARD_88D
	#endif

	#if defined( __LGT8FX8E__ ) && ! defined( __LGT8F_SSOP20__ ) && ! defined( ARDUINO_AVR_LARDU_88DS20 )
		Serial.println(F("328D (rare) detected"));
		#define BOARD_328D
	#endif

	#if defined( __LGT8FX8P__ ) && ( LED_BUILTIN != 12 ) && ! defined( __LGT8F_SSOP20__ ) && ! defined( __LGT8FX8P48__ )
		Serial.println(F("328P-LQFP32 (MiniEVB) detected"));
		#define BOARD_328P
	#endif

	#if defined( __LGT8FX8P__ ) && ( LED_BUILTIN == 12 ) && ! defined( __LGT8F_SSOP20__ )
		Serial.println(F("328P-LQFP32 wemos-TTGO-XI detected"));
		#define BOARD_328P
	#endif

	#if defined( __LGT8FX8P48__ )
		Serial.println(F("328P-LQFP48 MiniEVB detected"));
		#define BOARD_328P
	#endif

	#if defined( __LGT8FX8P__ ) && defined( __LGT8F_SSOP20__ )
		Serial.println(F("328P-SSOP20 (e.g. green pseudo pro mini) detected"));
		#define BOARD_328P
	#endif
	
	// -----------------------------------------------------------------

	Serial.print(F("lgt_eeprom_size( true  ) = ")); Serial.println( lgt_eeprom_size(true) );
	Serial.print(F("lgt_eeprom_size( false ) = ")); Serial.println( lgt_eeprom_size(false) );
	Serial.print(F("EEPROM.length()          = ")); Serial.println( EEPROM.length() );
	Serial.print(F("EEPROM E2END             = ")); Serial.println( E2END );


#ifdef TEST_ARDUINO_EEPROM_API
	// -----------------------------------------------------------------
 
	Serial.println(F("--------"));
 
	test_writing( COLS );

	Serial.println("done");
	
	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println(F("Testing EEPROM[]++ : "));

	for( uint16_t a=0; a< EEPROM.length(); a++ )
	{
		EEPROM[a]++;

		print_hex( EEPROM[a] );
		Serial.print(F("."));

		if ( ( a % COLS ) == ( COLS-1 ) ) Serial.println();
	}

	Serial.println("done");

	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println(F("Testing --EEPROM[] : "));
	err = 0;
	
	rngSeed(0);

	for( uint16_t a=0; a< EEPROM.length(); a++ )
	{
		byte d = --EEPROM[a];
		byte r = rng() & 0xFF;
		
		//~ print_hex( abs(d-r) );
		print_hex( d );
		
		if ( d == r ) //((a>>4)&0xFF) )
		{
			Serial.print(F("."));
		}
		else
		{
			Serial.print(F("!"));
			err++;
		}

		if ( ( a % COLS ) == ( COLS-1 ) ) Serial.println();
	}

	if ( err )
	{
		Serial.print("Errors : ");
		Serial.println( err );
	}
	else
	{
		Serial.println("OK");
	}

#endif // TEST_ARDUINO_EEPROM_API
	
	
	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println();
	Serial.print(F("Testing lgt_eeprom_write_block() :\nWriting 512 bytes, please wait ..."));
	
	
	buff = (byte*)malloc( 512 );
	for( uint16_t a=0; a<512; a++)
	{
		buff[a] = a & 0xFF;
	}
	
	lgt_eeprom_write_block( buff, 123, 512 );

	free(buff);
	
	Serial.println("done.");
	
	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println();
	Serial.println(F("Testing lgt_eeprom_read_block() :\nReading and verifying 512 bytes ..."));
	
	buff = (byte*)calloc( 512, 1 );
	
	
	lgt_eeprom_read_block( buff, 123, 512 );
	err=0;
	
	for( uint16_t a=0; a<512; a++)
	{
		if ( buff[a] != (a&0xFF) )
		{
			err++;
		}
	}
	
	if ( err != 0 )
	{
		Serial.print(F("Err : "));
		Serial.println(err);
	}
	else
	{
		Serial.println("OK");
	}
	
	free(buff);
	
	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println();
	Serial.print(F("Testing lgt_eeprom_write32() :\nWriting ..."));
	
	lgt_eeprom_write32( 0, 0xDEADBEAF );
	lgt_eeprom_write32( 456, 0xDEADBEAF );
	
	Serial.println("done.");
	
	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println();
	Serial.println(F("Testing lgt_eeprom_read32() :\nReading ..."));
	
	if ( lgt_eeprom_read32( 0 ) != 0xDEADBEAF || lgt_eeprom_read32( 456 ) != 0xDEADBEAF )
	{
		Serial.print(F("Error !"));
		Serial.println( lgt_eeprom_read32( 456 ), HEX );
	}
	else
	{
		Serial.println("OK");
	}
	

	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println();
	Serial.println(F("Testing lgt_eeprom_writeSWM() :\nGenerating 512 bytes buffer ..."));
	
	rngSeed(0);
	
	buff = (byte*)malloc( 512 );
	for( uint16_t a=0; a<512; a++)
	{
		buff[a] = ( rng() & 0xff );
		
		print_hex( buff[a] );
		Serial.print(F("."));
		
		if ( ( a % COLS ) == ( COLS - 1 ) )
		{
			Serial.println();
		}
	}
	
	Serial.print(F("Writing, please wait ..."));
	
	lgt_eeprom_writeSWM( 123, (uint32_t*)buff, 512/sizeof(uint32_t) );

	free(buff);
	
	Serial.println("done.");
	
	// -----------------------------------------------------------------
	Serial.println(F("--------"));
	
	Serial.println();
	Serial.println(F("Testing lgt_eeprom_readSWM() :\nReading 512 bytes ..."));
	
	buff = (byte*)calloc( 512, 1 );
	
	rngSeed(0);
	
	lgt_eeprom_readSWM( 123, (uint32_t*)buff, 512/sizeof(uint32_t) );
	
	Serial.println(F("Verifying ..."));
	
	err=0;
	
	for( uint16_t a=0; a<512; a++)
	{
		print_hex( buff[a] );
		Serial.print(F("."));
		
		if ( ( a % COLS ) == ( COLS - 1 ) )
		{
			Serial.println();
		}
		
		if ( buff[a] != ( rng() & 0xFF ))
		{
			err++;
		}
	}
	
	if ( err != 0 )
	{
		
		
		#if defined( BOARD_328P ) // should be 3 errors
		if ( err == 3 )
		{
			Serial.println("OK ( should be 3 errors )");
		}
		else
		#elif defined( BOARD_328D ) // should be 2 errors
		if ( err == 2 )
		{
			Serial.println("OK ( should be 2 errors )");
		}
		else
		#endif
		{
			Serial.print(F("Err : "));
			Serial.println(err);
		}
	}
	else
	{
		Serial.println("OK");
	}
	
	free(buff);
	
	// -----------------------------------------------------------------
	
	Serial.println(F("----------------"));
	Serial.print(F("Final test : "));
	
	
	if ( lgt_eeprom_read32( 0 ) != 0xDEADBEAF )
	{
		Serial.println("ERROR : there should be a 0xDEADBEEF at 0");
	}
	else
	{
		Serial.println("OK : there is still a 0xDEADBEEF at 0");
	}
}

void loop()
{
}
