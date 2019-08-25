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

/* Name: vmouse.c
 * 
 * Project: hid-mouse, a very simple HID example
 * Author: LogicGreen Technologies
 * Create Date: 2017/5/18
 *  - based on HID-mouse from VUSB library, port to arduino platform
 *  - VUSB library working with LGT8F328P series
 */

/*
This example should run on most AVRs with only little changes. No special
hardware resources except INT0 are used. You may have to change usbconfig.h for
different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
at least be connected to INT0 as well.

We use VID/PID 0x046D/0xC00E which is taken from a Logitech mouse. Don't
publish any hardware using these IDs! This is for demonstration only!
*/

#include <WDT.h>
#include <VUsbMouse.h>

// to generate coordinate for circle-rounded mouse
int sinus = 7 << 6;
int cosinus = 0;
void advanceCircleByFixedAngle();

// CRICIAL NOTE:
// Timer0 OVF interrupt is diabled after VmouseDevice.begin
// so we can not use standard delay() for its based on Timer0 OVF interrupt.
// but we can use delayMicroseconds() which is implemented by instruction cycle.
// We can also implemente our own delayms based on delayMicroseconds()
void delayms(uint8_t ms)
{
    uint8_t i;

    for(i = 0; i < ms; i++)
      delayMicroseconds(1000);
}

void setup()
{
    noInterrupts();
	  sysClock(EXT_OSC);

    wdt_reset();
    wdt_enable(WDTO_2S);
  
    /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
     */
    /* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */
    VUsbMouse.begin();
    interrupts();
}
    
void loop()
{
    uint8_t i; 

    wdt_reset();

    advanceCircleByFixedAngle();
    VUsbMouse.loop();
    
    delayms(10);                 
}

/* The following function advances sin/cos by a fixed angle
 * and stores the difference to the previous coordinates in the report
 * descriptor.
 * The algorithm is the simulation of a second order differential equation.
 */
#define DIVIDE_BY_64(val)  (val + (val > 0 ? 32 : -32)) >> 6    /* rounding divide */
 
void advanceCircleByFixedAngle()
{
    char    d;
    
    VUsbMouse.reportBuffer.dx = d = DIVIDE_BY_64(cosinus);
    sinus += d;
    VUsbMouse.reportBuffer.dy = d = DIVIDE_BY_64(sinus);
    cosinus -= d;
}
