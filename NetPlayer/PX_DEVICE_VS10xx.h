#ifndef PX_DEVICE_VS10xx_H
#define PX_DEVICE_VS10xx_H
#include "PX_Linker.h"

#define PX_DEVICE_VS10xx_VS_WRITE_COMMAND 	0x02
#define PX_DEVICE_VS10xx_VS_READ_COMMAND 	0x03

#define PX_DEVICE_VS10xx_REG_MODE        	0x00   
#define PX_DEVICE_VS10xx_REG_STATUS      	0x01   
#define PX_DEVICE_VS10xx_REG_BASS        	0x02   
#define PX_DEVICE_VS10xx_REG_CLOCKF      	0x03   
#define PX_DEVICE_VS10xx_REG_DECODE_TIME 	0x04   
#define PX_DEVICE_VS10xx_REG_AUDATA      	0x05   
#define PX_DEVICE_VS10xx_REG_WRAM        	0x06   
#define PX_DEVICE_VS10xx_REG_WRAMADDR    	0x07   
#define PX_DEVICE_VS10xx_REG_HDAT0       	0x08   
#define PX_DEVICE_VS10xx_REG_HDAT1       	0x09 

#define PX_DEVICE_VS10xx_REG_AIADDR      	0x0a   
#define PX_DEVICE_VS10xx_REG_VOL         	0x0b   
#define PX_DEVICE_VS10xx_REG_AICTRL0     	0x0c   
#define PX_DEVICE_VS10xx_REG_AICTRL1     	0x0d   
#define PX_DEVICE_VS10xx_REG_AICTRL2     	0x0e   
#define PX_DEVICE_VS10xx_REG_AICTRL3     	0x0f   
#define PX_DEVICE_VS10xx_SM_DIFF         	0x01   
#define PX_DEVICE_VS10xx_SM_JUMP         	0x02   
#define PX_DEVICE_VS10xx_SM_RESET        	0x04   
#define PX_DEVICE_VS10xx_SM_OUTOFWAV     	0x08   
#define PX_DEVICE_VS10xx_SM_PDOWN        	0x10   
#define PX_DEVICE_VS10xx_SM_TESTS        	0x20   
#define PX_DEVICE_VS10xx_SM_STREAM       	0x40   
#define PX_DEVICE_VS10xx_SM_PLUSV        	0x80   
#define PX_DEVICE_VS10xx_SM_DACT         	0x100   
#define PX_DEVICE_VS10xx_SM_SDIORD       	0x200   
#define PX_DEVICE_VS10xx_SM_SDISHARE     	0x400   
#define PX_DEVICE_VS10xx_SM_SDINEW       	0x800   
#define PX_DEVICE_VS10xx_SM_ADPCM        	0x1000   
#define PX_DEVICE_VS10xx_SM_ADPCM_HP     	0x2000 		

#define PX_DEVICE_VS10xx_DEFAULT_VOLUME 168
typedef struct 
{
	PX_Linker *linker;
}PX_DEVICE_VS10xx;


typedef enum
{
	PX_DEVICE_VS10xx_IOCTL_SLEEPMS,
	PX_DEVICE_VS10xx_IOCTL_XDCS,
	PX_DEVICE_VS10xx_IOCTL_XCS,
	PX_DEVICE_VS10xx_IOCTL_DREQ,
	PX_DEVICE_VS10xx_IOCTL_RST,
	PX_DEVICE_VS10xx_IOCTL_SPI_HIGHSPEED,
	PX_DEVICE_VS10xx_IOCTL_SPI_LOWSPEED,
}PX_DEVICE_VS10xx_IOCTL;

px_bool PX_DEVICE_VS10xxInit(PX_DEVICE_VS10xx *device,PX_Linker *linker);
px_bool PX_DEVICE_VS10xxReset(PX_DEVICE_VS10xx *device);
px_bool PX_DEVICE_VS10xxSoftReset(PX_DEVICE_VS10xx *device);
px_bool PX_DEVICE_VS10xxWrite(PX_DEVICE_VS10xx *device,px_byte data[32]);
px_bool PX_DEVICE_VS10xxPatch(PX_DEVICE_VS10xx *device,px_byte *bin,px_int binsize);
px_void PX_DEVICE_VS10xxSinTest(PX_DEVICE_VS10xx *device);
#endif
