/* Name: vmouse.h
 */

#ifndef __vmouse_h_included__
#define __vmouse_h_included__

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "usbdrv.h"

#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM const char usbHidReportDescriptor[52] = { /* USB report descriptor, size must match usbconfig.h */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM
    0x29, 0x03,                    //     USAGE_MAXIMUM
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Const,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //   END_COLLECTION
    0xC0,                          // END COLLECTION
};

/* This is the same report descriptor as seen in a Logitech mouse. The data
 * described by this descriptor consists of 4 bytes:
 *      .  .  .  .  . B2 B1 B0 .... one byte with mouse button states
 *     X7 X6 X5 X4 X3 X2 X1 X0 .... 8 bit signed relative coordinate x
 *     Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0 .... 8 bit signed relative coordinate y
 *     W7 W6 W5 W4 W3 W2 W1 W0 .... 8 bit signed relative coordinate wheel
 */
typedef struct {
	uchar	buttonMask;
	char	dx;
	char	dy;
	char 	dWheel;
} reportBuffer_t;


class VUsbMouseClass {
public:
	void begin() {
    		uchar   i;

		TIMSK0 = 0;

		memset(&reportBuffer, 0, sizeof(reportBuffer));
    
    		usbInit();
    		usbDeviceDisconnect();
    
		// fake USB disconnect for > 250ms
    		i = 0;
    		while(--i) {  
			nop(); nop();
        		delayMicroseconds(1000);
    		}
    
    		usbDeviceConnect();
	}

	void loop() {
      		usbPoll();
      		if(usbInterruptIsReady()){
          		/* called after every poll of the interrupt endpoint */
          		usbSetInterrupt((uchar *)&reportBuffer, sizeof(reportBuffer));
		}
	}

public:
	uchar idleRate;
	reportBuffer_t reportBuffer;
};

VUsbMouseClass  VUsbMouse;
/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
  usbRequest_t    *rq = (usbRequest_t *)data;

    /* The following requests are never used. But since they are required by
     * the specification, we implement them in this example.
     */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
            usbMsgPtr = (uchar *)&VUsbMouse.reportBuffer;
            return sizeof(reportBuffer_t);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = (uchar *)&VUsbMouse.idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            VUsbMouse.idleRate = rq->wValue.bytes[1];
        }
    } else {
        /* no vendor specific requests implemented */
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

#ifdef __cplusplus
}
#endif

#endif /* __vmouse_h_included__ */
