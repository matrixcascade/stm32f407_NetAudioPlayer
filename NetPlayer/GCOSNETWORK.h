#ifndef __GCOS_NETWORK_H
#define __GCOS_NETWORK_H

#include "PX_DEVICE_W5500.h"

#define GCOS_UDP_DEFAULE_PORT 16666
typedef struct
{
	union 
	{
		px_u32 target_ip_u32;
		px_uchar target_ip_byte[4];
	}S_un;
	px_u16 target_port;
}GCOS_addr_in;

px_bool GCOS_UDPInit(px_char *localip,px_char *masker,px_char *gateway,px_u16 listenPort);
px_int  GCOS_UDPWrite(GCOS_addr_in *target,px_byte *buffer,px_int bufferSize);
px_int  GCOS_UDPRead(GCOS_addr_in *target,px_byte *buffer);
//px_void GCOS_UDPFree();

GCOS_addr_in GCOS_UDP_TARGET_ADDRIN(px_char *ipaddr,px_u16 port);

#endif
