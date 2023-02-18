#ifndef __MERCURY_USB_H__
#define __MERCURY_USB_H__
#include "MercuryDef.h"

/*
Note: in the MC_USB_MODE_ONLY_TWO_SERIALS mode, because only two port, if you still want to use console, you can 
1. change CONSOLE_VCOM_PORT to 0 
2. the port0 is usb as both fastboot and console, so you need call MercuryFastbootStateSet(FASTBOOT_STOP); to disable fastboot function.
*/

typedef enum{
	MC_USB_MODE_CHARGER_ONLY = 0,
	MC_USB_MODE_SERIALS,			//three serials for user. other 4 serials for debug function, such as cp trace, ap trace
	MC_USB_MODE_ONLY_USER_SERIALS, 	//only three serials
	MC_USB_MODE_RNDIS_AND_SERIALS,	//rndis and 7 serial(3 for user, other 4 for debug function)
	MC_USB_MODE_ECM_AND_SERIALS,	//ecm and 7 serial(3 for user, other 4 for debug function)
	MC_USB_MODE_RNDIS_AND_ONLY_USER_SERIALS,	//rndis and 3 serial for user
	MC_USB_MODE_ECM_AND_ONLY_USER_SERIALS,		//ecm and 3 serial for user	
	MC_USB_MODE_ONLY_TWO_SERIALS,		/*only TWO serials for user. */										
	MC_USB_MODE_MAX
}MC_USB_MODE_E;

extern int MercuryUsbSetMode(MC_USB_MODE_E mode, BOOLEAN is_store);
extern MC_USB_MODE_E MercuryUsbGetMode(void);

#endif //__MERCURY_USB_H__
