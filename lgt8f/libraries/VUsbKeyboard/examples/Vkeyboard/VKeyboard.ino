// ==========================================================================
// LARDUINO_HSP VUSB (HID Device) Demostration
//
// ==========================================================================
//                                                           VCC
// Usage Note:                                               ___   
//      1. VUSB is type of low speed device                   | 
//      2. Larduino VUSB apply to LGT8FX8P device only        | 
//      3. VUSB IO connections:                              | |  1.5Kohm
//                                                            |
//              Larduino D4 <--------|| 50ohm ||--------------+---------> DP   
//              Larduino D2 <--------|| 50ohm ||------------------------> DM
//
//	4. A 50ohm resister should be serial connected to DP/DM and D4/2 for isolation.
//	5. A 1.5Kohm pullup on D4 should be used to apply USB low speed identification.
//	6. IO used for DP/DM can be modified inside ../usbconfig.h, But note interrupter 
//         on DP is needed to capture start of bus transaction.
//	7. VCC should be working at 3.3V (3.0V ~ 3.6V is accepted by USB spec), 
//	   or clamp diodbe should be used to limit voltage of DP/DM.
//	8. NOTE: TMR0_OVF inerrupter is disabled inside of VUSB intiliazaiton,
//	   so arduino's delay() which based on TMR0_OVF interupter can not be used anymore.
//	   use arduino's delayMicroseconds() which is instruction based delay.

#include <VUsbKeyboard.h>

#define BUTTON_PIN 12

// If the timer isr is corrected
// to not take so long change this to 0.
#define BYPASS_TIMER_ISR 1

void delay_ms(uint16_t ms) {
#if BYPASS_TIMER_ISR
	for (uint16_t i = 0; i < ms; i++) {
		delayMicroseconds(1000);
	}
#else
	delay(ms);
#endif
}

void setup() 
{
	noInterrupts();
	sysClock(EXT_OSC);

	pinMode(BUTTON_PIN, INPUT);
	digitalWrite(BUTTON_PIN, HIGH);
  
	VUsbKeyboard.begin();
	interrupts();
}


void loop() 
{
	// VUSB event loop
	VUsbKeyboard.loop();

	digitalWrite(13, !digitalRead(13));

	if (digitalRead(BUTTON_PIN) == 0) {
    
		// Send "HELLO WORLD" + Return
		// if D12 is driven to low level

    		//VUsbKeyboard.sendKeyStroke(KEY_B, MOD_GUI_LEFT);
    
		VUsbKeyboard.sendKeyStroke(KEY_H);
		VUsbKeyboard.sendKeyStroke(KEY_E);
		VUsbKeyboard.sendKeyStroke(KEY_L);
		VUsbKeyboard.sendKeyStroke(KEY_L);
		VUsbKeyboard.sendKeyStroke(KEY_O);

		VUsbKeyboard.sendKeyStroke(KEY_SPACE);

		VUsbKeyboard.sendKeyStroke(KEY_W);
		VUsbKeyboard.sendKeyStroke(KEY_O);
		VUsbKeyboard.sendKeyStroke(KEY_R);
		VUsbKeyboard.sendKeyStroke(KEY_L);
		VUsbKeyboard.sendKeyStroke(KEY_D);
		//VUsbKeyboard.sendKeyStroke(KEY_B, MOD_GUI_LEFT);

		VUsbKeyboard.sendKeyStroke(KEY_ENTER);

    		delay_ms(20);
   }
}
