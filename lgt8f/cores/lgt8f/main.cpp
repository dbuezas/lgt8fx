#include <stdint.h>
#include <avr/wdt.h>
#include <Arduino.h>

#if defined(__LGT8FX8E__) || defined(__LGT8FX8P__)
void __patch_wdt(void) \
	     __attribute__((naked)) \
	     __attribute__((section(".init3")));
void __patch_wdt(void)
{
	MCUSR = 0;
	wdt_disable();
}
#endif

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

void sysClock(uint8_t mode)
{
	if(mode == EXT_OSC) {
		// set to right prescale
		CLKPR = 0x80;
		CLKPR = 0x01;	

		asm volatile ("nop");
		asm volatile ("nop");

		// enable external crystal
		PMCR = 0x80;
		PMCR = 0x97;
		
		// waiting for crystal stable
		for(GPIOR0 = 0xff; GPIOR0 > 0; --GPIOR0);
		for(GPIOR0 = 0xff; GPIOR0 > 0; --GPIOR0);

		// switch to external crystal
		PMCR = 0x80;
		PMCR = 0xb7;

		// waiting for crystal stable
		for(GPIOR0 = 0xff; GPIOR0 > 0; --GPIOR0);
		for(GPIOR0 = 0xff; GPIOR0 > 0; --GPIOR0);
		// set to right prescale
		CLKPR = 0x80;
		CLKPR = 0x00;	
	} else if(mode == INT_OSC) {
		// prescaler settings
		CLKPR = 0x80;
		CLKPR = 0x01;	

		// switch to internal crystal
		GPIOR0 = PMCR & 0x9f;
		PMCR = 0x80;
		PMCR = GPIOR0;

		// disable external crystal
		GPIOR0 = PMCR & 0xfb;
		PMCR = 0x80;
		PMCR = GPIOR0;
	}
}	

void lgt8fx8x_init()
{
#if defined(__LGT8F_SSOP20__)
	GPIOR0 = PMXCR | 0x07;
	PMXCR = 0x80;
	PMXCR = GPIOR0;
#endif

#if defined(__LGT8FX8E__)
// store ivref calibration 
	GPIOR1 = VCAL1;
	GPIOR2 = VCAL2;

// enable 1KB E2PROM 
	ECCR = 0x80;
	ECCR = 0x40;

// clock source settings
	if((VDTCR & 0x0C) == 0x0C) {
		// switch to external crystal
		sysClock(EXT_OSC);
	} else {
		CLKPR = 0x80;
		CLKPR = 0x01;
	}
#else
	// enable 32KRC for WDT
	GPIOR0 = PMCR | 0x10;
	PMCR = 0x80;
	PMCR = GPIOR0;

	// clock scalar to 16MHz
	CLKPR = 0x80;
	CLKPR = 0x01;
#endif
}

int main(void)
{

#if defined(__LGT8F__)
	lgt8fx8x_init();
#endif	

	init();

	initVariant();

#if defined(USBCON)
	USBDevice.attach();
#endif
/* START CHANGE BY DBUEZAS */
  // set clock prescaler
	CLKPR = 0x80;
#if F_CPU == 32000000L
  CLKPR = 0x00;
#elif F_CPU == 16000000L
  CLKPR = 0x01;
#elif F_CPU == 8000000L
  CLKPR = 0x02;
#elif F_CPU == 4000000L
  CLKPR = 0x03;
#elif F_CPU == 2000000L
  CLKPR = 0x04;
#elif F_CPU == 1000000L
  CLKPR = 0x05;
#endif
/* END CHANGE BY DBUEZAS */
	setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
        
	return 0;
}
