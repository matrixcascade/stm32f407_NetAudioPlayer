#include "PX_DEVICE_LED.h"
px_void PX_DEVICE_LED_RED(PX_DEVICE_LED *device,px_bool blight)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_LED_IOCTL_RED,blight,PX_NULL);
}

px_void PX_DEVICE_LED_GREEN(PX_DEVICE_LED *device,px_bool blight)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_LED_IOCTL_GREEN,blight,PX_NULL);
}

px_void PX_DEVICE_LED_YELLOW(PX_DEVICE_LED *device,px_bool blight)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_LED_IOCTL_YELLOW,blight,PX_NULL);
}

px_void PX_DEVICE_LEDInit(PX_DEVICE_LED *device,PX_Linker *linker)
{
	device->linker=linker;
	device->linker->_PX_LinkerInit(PX_NULL);
}

