#ifndef __PX_LINKER_H
#define __PX_LINKER_H
#include "PX_typedef.h"

typedef struct __PX_LINKER
{
	px_void *data;
	px_bool (* _PX_LinkerInit)(px_void *Info);
	px_int  (* _PX_LinkerWrite)(_IN px_void *buffer,px_int size);
	px_int  (* _PX_LinkerRead) (_OUT px_void *buffer,px_int size);
	px_int (* _PX_LinkerIOCTL)(_IN px_int ioctl,_IN px_int io,_IN px_void *param);
}PX_Linker;
px_bool  PX_LinkerInit(PX_Linker *linker, px_void *Init,px_void *Write,px_void *Read,px_void *ioctl,px_void *param);
px_int   PX_LinkerIOCTL(PX_Linker *linker,px_int IOCTL,px_void *param);
#define  PX_LinkerWrite(lnk,buffer,size) ((lnk)->_PX_LinkerWrite(buffer,size))
#define  PX_LinkerRead(lnk,buffer,size) ((lnk)->_PX_LinkerRead(buffer,size))
#define  PX_LinkerIOCTL(lnk,ioctl,io,param) ((lnk)->_PX_LinkerIOCTL(ioctl,io,param))
#endif
