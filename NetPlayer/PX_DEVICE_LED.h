#ifndef __PX_DEVICE_LED_H
#define __PX_DEVICE_LED_H
#include "PX_Linker.h"
typedef struct
{
	PX_Linker *linker;
}PX_DEVICE_LED;

typedef enum
{
	PX_DEVICE_LED_IOCTL_RED,
	PX_DEVICE_LED_IOCTL_GREEN,
	PX_DEVICE_LED_IOCTL_YELLOW
}PX_DEVICE_LED_IOCTL;

px_void PX_DEVICE_LEDInit(PX_DEVICE_LED *device,PX_Linker *linker);
px_void PX_DEVICE_LED_RED(PX_DEVICE_LED *device,px_bool blight);
px_void PX_DEVICE_LED_GREEN(PX_DEVICE_LED *device,px_bool blight);
px_void PX_DEVICE_LED_YELLOW(PX_DEVICE_LED *device,px_bool blight);
#endif
