/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * TODO: Make a proper file header. :-)
 */
#ifndef __UsbDevice_h__
#define __UsbDevice_h__

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "usbdrv.h"

#ifdef __cplusplus
}
#endif

#include <util/delay.h>     /* for _delay_ms() */

class VUsbGenericDevice {
public:
  void begin() {
    uchar   i;

    // disable timer 0 overflow interrupt (used for millis)
    TIMSK0 = 0;

    usbInit();
      
    usbDeviceDisconnect();
    
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }

    usbDeviceConnect();
  }
    
  // TODO: Deprecate update
  void loop() {
    usbPoll();
  }
};

extern VUsbGenericDevice VUsbDevice;

#endif // __UsbDevice_h__
