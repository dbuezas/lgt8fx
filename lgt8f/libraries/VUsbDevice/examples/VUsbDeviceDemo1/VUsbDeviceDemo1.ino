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

#include <VUsbDevice.h>

void setup() {
	noInterrupts();
	sysClock(EXT_OSC);

	VUsbDevice.begin();
	interrupts();
}

void loop() {

	VUsbDevice.loop();
}
