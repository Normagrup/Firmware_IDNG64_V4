// Debuging OPTIONS

	//#define ETH_IGNORE_CRC
	//#define NO_BOOTLOADER
	//#define NO_WDT
	//#define DESACTIVAR_IED2
	//#define DEBUG
	//#define IGNORE_SUBNET_FILTER
	

#ifdef	DEBUG
	#include "debug.h"
#endif
	
	
	

#define FW_VERSION_MAJOR	(4)
#define FW_VERSION_MINOR	(8)
#define WEBSERVER_TEXT		"[Fw: v4.9 build 1 17/07/2024].\r\n"

#ifdef	DEBUG
	#define WEBSERVER_TEXT		"[Fw: v4.7].\r\n"	
#endif