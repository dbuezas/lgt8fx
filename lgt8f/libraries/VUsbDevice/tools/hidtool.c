/* Name: hidtool.c
 * Project: hid-data example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-11
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hiddata.h"
#include "../usbconfig.h"  /* for device VID, PID, vendor name and product name */

/* ------------------------------------------------------------------------- */
static char *usbErrorMessage(int errCode)
{
	static char buffer[80];

    switch(errCode){
        case USBOPEN_ERR_ACCESS:      return "Access to device denied";
        case USBOPEN_ERR_NOTFOUND:    return "The specified device was not found";
        case USBOPEN_ERR_IO:          return "Communication error with device";
        default:
            sprintf(buffer, "Unknown USB error %d", errCode);
            return buffer;
    }
	
    return NULL;    /* not reached */
}

static usbDevice_t  *openDevice(void)
{
	usbDevice_t     *dev = NULL;
	unsigned char   rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
	char            vendorName[] = {USB_CFG_VENDOR_NAME, 0}, productName[] = {USB_CFG_DEVICE_NAME, 0};
	int             vid = rawVid[0] + 256 * rawVid[1];
	int             pid = rawPid[0] + 256 * rawPid[1];
	int             err;

    if((err = usbhidOpenDevice(&dev, vid, vendorName, pid, productName, 1)) != 0){
        fprintf(stderr, "error finding %s: %s\n", productName, usbErrorMessage(err));
        return NULL;
    }
    return dev;
}

/* ------------------------------------------------------------------------- */

static void hexdump(char *buffer, int len)
{
	int     i;
	FILE    *fp = stdout;

    for(i = 0; i < len; i++){
        if(i != 0){
            if(i % 16 == 0){
                fprintf(fp, "\n");
            }else{
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "0x%02x", buffer[i] & 0xff);
    }
    if(i != 0)
        fprintf(fp, "\n");
}

static int  hexread(char *buffer, char *string, int buflen)
{
	char    *s;
	int     pos = 0;

	char 	*p = string;
	
	s = strtok(p, ", ");

    while(s != NULL) {
        buffer[pos++] = (char)strtoul(s, NULL, 0);
        if(pos >= buflen)
        	break;
        	
        s = strtok(NULL, ", ");
    }
    
    return pos;
}

/* ------------------------------------------------------------------------- */

static void usage()
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, ">>read - read back pin status of current selected io \n");
    fprintf(stderr, ">>write port value\n");
    fprintf(stderr, ">>   - port : which port to write (0 ~ n) \n");
    fprintf(stderr, ">>            port name is in accord with Arduino UNO style.  \n");
    fprintf(stderr, ">>   - value : 0, 1 or 2, write 2 to set port to input mode \n");
    fprintf(stderr, ">>example: write 0 1\n");
}

 // report counts + one byte (report ID)
#define	HID_REPORT_SZ	(2+1)

int main(int argc, char **argv)
{
	usbDevice_t *dev;
	char        cmd[BUFSIZ]; 
	char		buffer[HID_REPORT_SZ];
	int         err, len;

    if((dev = openDevice()) == NULL) {
    		printf(">> Can not find our HID device!");
        exit(1);
    }

    do {
    	printf("\r\n>> Waiting command: ");
    	
    	gets(cmd);
    	
    	memset(buffer, 0, sizeof(buffer));    	
	    
	if(strncmp(cmd, "read", 4) == 0) {			
		len = sizeof(buffer); 
		if((err = usbhidGetReport(dev, 0, buffer, &len)) != 0){
			printf("\r\n>> error reading data: %s\n", usbErrorMessage(err));
		} else { 
			printf("\r\n>> port(%d) = %1x", buffer[1], buffer[2]);
		}
	} else if(strncmp(cmd, "write", 5) == 0) {
		memset(buffer, 0, sizeof(buffer));
		buffer[0] = 0;	// report ID
			
		// skip report ID
		hexread(&buffer[1], &cmd[5], sizeof(buffer) - 1);
		
		if((err = usbhidSetReport(dev, buffer, sizeof(buffer))) != 0)
			printf("\r\n>> error writing data: %s\n", usbErrorMessage(err));
	} else if(strcasecmp(cmd, "quit") == 0) {
	   	exit(1);	    
	} else {
	        usage();
	}
    } while(1);

    usbhidCloseDevice(dev);
    return 0;
}

/* ------------------------------------------------------------------------- */
