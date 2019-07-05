#include "PX_DEVICE_VS10xx.h"

px_void PX_DEVICE_VS10xx_WriteByte(PX_DEVICE_VS10xx *device,px_u8 data)
{
	PX_LinkerWrite(device->linker,&data,1);
}

px_byte PX_DEVICE_VS10xx_ReadByte(PX_DEVICE_VS10xx *device)
{
	px_u8 data;
	PX_LinkerRead(device->linker,&data,1);
	return data;
}

px_void PX_DEVICE_VS10xx_Write_Register(PX_DEVICE_VS10xx *device,px_u8 addr,px_u16 data)
{
	if(PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL)==0)
		return;
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SPI_LOWSPEED,0,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XDCS,1,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XCS,0,PX_NULL);
	PX_DEVICE_VS10xx_WriteByte(device,PX_DEVICE_VS10xx_VS_WRITE_COMMAND);
	PX_DEVICE_VS10xx_WriteByte(device,addr);
	PX_DEVICE_VS10xx_WriteByte(device,data>>8);
	PX_DEVICE_VS10xx_WriteByte(device,data);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XCS,1,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SPI_HIGHSPEED,0,PX_NULL);
}

px_u16 PX_DEVICE_VS10xx_Read_Register(PX_DEVICE_VS10xx *device,px_u8 addr)
{
	px_u16 reg;
	if(PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL)==0)
		return 0;
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SPI_LOWSPEED,0,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XDCS,1,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XCS,0,PX_NULL);
	PX_DEVICE_VS10xx_WriteByte(device,PX_DEVICE_VS10xx_VS_READ_COMMAND);
	PX_DEVICE_VS10xx_WriteByte(device,addr);
	reg=(PX_DEVICE_VS10xx_ReadByte(device)<<8);
	reg|=PX_DEVICE_VS10xx_ReadByte(device);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XCS,1,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SPI_HIGHSPEED,0,PX_NULL);
  return reg;
}

px_u16 PX_DEVICE_VS10xx_Read_RAM(PX_DEVICE_VS10xx *device,px_u16 addr)
{
	PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_WRAMADDR,addr);
	return PX_DEVICE_VS10xx_Read_Register(device,PX_DEVICE_VS10xx_REG_WRAM);
}

px_void PX_DEVICE_VS10xx_Write_RAM(PX_DEVICE_VS10xx *device,px_u16 addr,px_u16 val)
{
	px_u16 retry=0xff;
	if(PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL)==0)
		return;
	PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_WRAMADDR,addr);
	while(PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL)==0)
		if (retry--==0)
		{
			return;
		}
		else
			PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,20,PX_NULL);

	PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_WRAM,val);
}

px_bool PX_DEVICE_VS10xxInit(PX_DEVICE_VS10xx *device,PX_Linker *linker)
{
	device->linker=linker;
	PX_DEVICE_VS10xxReset(device);
	return PX_TRUE;
}

px_bool PX_DEVICE_VS10xxReset(PX_DEVICE_VS10xx *device)
{
	px_u8 retry=0xff;
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_RST,0,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,20,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XDCS,0,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XCS,0,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_RST,1,PX_NULL);

	while (!PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL))
	{
		if (retry--==0)
		{
			return PX_FALSE;
		}
		PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,50,PX_NULL);
	}

	PX_DEVICE_VS10xx_WriteByte(device,0xff);

	while(PX_DEVICE_VS10xx_Read_Register(device,PX_DEVICE_VS10xx_REG_MODE)!=0x0800)	
	{
		PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_MODE,0x0804);    
		PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,2,PX_NULL);
		if(retry--==0)return PX_FALSE; 	    
	}	 		 
	retry=0xff;

	while (!PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL))
	{
		if (retry--==0)
		{
			return PX_FALSE;
		}
		PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,50,PX_NULL);
	}

	while(PX_DEVICE_VS10xx_Read_Register(device,PX_DEVICE_VS10xx_REG_CLOCKF)!=0X9800)
	{
		PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_CLOCKF,0X9800);	
		if(retry--==0)return PX_FALSE; 	    
	}	 
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,20,PX_NULL);
	
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SPI_HIGHSPEED,0,PX_NULL);
	return PX_TRUE;
}

px_bool PX_DEVICE_VS10xxWrite(PX_DEVICE_VS10xx *device,px_byte data[32])
{
	px_u8 i;
	if (PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL))
	{
		PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XDCS,0,PX_NULL);
		for(i=0;i<32;i++)
			PX_LinkerWrite(device->linker,&data[i],1);
		PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XDCS,1,PX_NULL);
		return PX_TRUE;
	}
	return PX_FALSE;
}

px_void PX_DEVICE_VS10xxSetVolume(PX_DEVICE_VS10xx *device,px_u8 vol)
{
	px_u16 volt=0; 		
	volt=254-vol;			
	volt<<=8;
	volt|=(254-vol);			
	PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_VOL,volt);
}

px_void PX_DEVICE_VS10xxSetBass(PX_DEVICE_VS10xx *device,px_u8 bfreq,px_u8 bass,px_u8 tfreq,px_u8 treble)
{
	px_u16 bass_set=0; 
	px_char temp=0;   	 
	if(treble==0)temp=0;	   	
	else if(treble>8)temp=treble-8;
	else temp=treble-9;  
	bass_set=temp&0X0F;			
	bass_set<<=4;
	bass_set+=tfreq&0xf;		
	bass_set<<=4;
	bass_set+=bass&0xf;			
	bass_set<<=4;
	bass_set+=bfreq&0xf;		 
	PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_BASS,bass_set);
}

px_void PX_DEVICE_VS10xxSetEffect(PX_DEVICE_VS10xx *device,px_u8 eft)
{
	px_u16 temp;	 
	temp=PX_DEVICE_VS10xx_Read_Register(device,PX_DEVICE_VS10xx_REG_MODE);
	if(eft&0X01)temp|=1<<4;		
	else temp&=~(1<<5);			
	if(eft&0X02)temp|=1<<7;		
	else temp&=~(1<<7);									   
	PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_MODE,temp);	  
}

px_void PX_DEVICE_VS10xxSetDefault(PX_DEVICE_VS10xx *device)
{
	PX_DEVICE_VS10xxSetVolume(device,PX_DEVICE_VS10xx_DEFAULT_VOLUME);
}

px_void PX_DEVICE_VS10xxSinTest(PX_DEVICE_VS10xx *device)
{
	PX_DEVICE_VS10xxReset(device);
	PX_DEVICE_VS10xx_Write_Register(device,0x0b,0X2020);
	PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_MODE,0x0820);
	while(PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL)==0);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SPI_LOWSPEED,0,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XDCS,0,PX_NULL);
	PX_DEVICE_VS10xx_WriteByte(device,0x53);
	PX_DEVICE_VS10xx_WriteByte(device,0xef);
	PX_DEVICE_VS10xx_WriteByte(device,0x6e);
	PX_DEVICE_VS10xx_WriteByte(device,0x24);
	PX_DEVICE_VS10xx_WriteByte(device,0x00);
	PX_DEVICE_VS10xx_WriteByte(device,0x00);
	PX_DEVICE_VS10xx_WriteByte(device,0x00);
	PX_DEVICE_VS10xx_WriteByte(device,0x00);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,100,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_XDCS,1,PX_NULL);
}

px_bool PX_DEVICE_VS10xxPatch(PX_DEVICE_VS10xx *device,px_byte *patch,px_int len)
{
		px_u16 i; 
		px_u16 addr, n, val; 	  			   
		for(i=0;i<len;) 
		{ 
			addr = patch[i++]; 
			n    = patch[i++]; 
			if(n & 0x8000U) //RLE run, replicate n samples 
			{ 
				n  &= 0x7FFF; 
				val = patch[i++]; 
				while(n--)PX_DEVICE_VS10xx_Write_Register(device,addr, val);  
			}else //copy run, copy n sample 
			{ 
				while(n--) 
				{ 
					val = patch[i++]; 
					PX_DEVICE_VS10xx_Write_Register(device,addr, val); 
				} 
			} 
		} 	 
		return PX_TRUE;
}

px_bool PX_DEVICE_VS10xxSoftReset(PX_DEVICE_VS10xx *device)
{
	px_u16 retry;
	PX_DEVICE_VS10xx_WriteByte(device,0xff);

	while(PX_DEVICE_VS10xx_Read_Register(device,PX_DEVICE_VS10xx_REG_MODE)!=0x0800)	
	{
		PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_MODE,0x0804);    
		PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,2,PX_NULL);
		if(retry--==0)return PX_FALSE; 	    
	}	 		 
	retry=0xff;

	while (!PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_DREQ,0,PX_NULL))
	{
		if (retry--==0)
		{
			return PX_FALSE;
		}
		PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,50,PX_NULL);
	}

	while(PX_DEVICE_VS10xx_Read_Register(device,PX_DEVICE_VS10xx_REG_CLOCKF)!=0X9800)
	{
		PX_DEVICE_VS10xx_Write_Register(device,PX_DEVICE_VS10xx_REG_CLOCKF,0X9800);	
		if(retry--==0)return PX_FALSE; 	    
	}	 
	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SLEEPMS,20,PX_NULL);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_VS10xx_IOCTL_SPI_HIGHSPEED,0,PX_NULL);

	PX_DEVICE_VS10xxSetVolume(device,PX_DEVICE_VS10xx_DEFAULT_VOLUME);
	return PX_TRUE;
}
