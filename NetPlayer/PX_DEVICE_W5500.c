#include "PX_DEVICE_W5500.h"


px_void PX_DEVICE_W5500_Write_DWORD(PX_DEVICE_W5500 *device,px_u32 srt)
{
	px_u8 byte;
	byte=(srt>>24)&0xff;
	PX_LinkerWrite(device->linker,&byte,1);
	byte=(srt>>16)&0xff;
	PX_LinkerWrite(device->linker,&byte,1);
	byte=(srt>>8)&0xff;
	PX_LinkerWrite(device->linker,&byte,1);
	byte=srt&0xff;
	PX_LinkerWrite(device->linker,&byte,1);
}

px_void PX_DEVICE_W5500_Write_WORD(PX_DEVICE_W5500 *device,px_u16 srt)
{
	px_u8 byte;
	byte=srt>>8;
	PX_LinkerWrite(device->linker,&byte,1);
	byte=srt&0xff;
	PX_LinkerWrite(device->linker,&byte,1);
}

px_void PX_DEVICE_W5500_Write_Byte(PX_DEVICE_W5500 *device,px_u8 byte)
{
	PX_LinkerWrite(device->linker,&byte,1);
}

px_u8 PX_DEVICE_W5500_Read_Byte(PX_DEVICE_W5500 *device)
{
	px_u8 byte;
	PX_LinkerRead(device->linker,&byte,1);
	return byte;
}

px_u16 PX_DEVICE_W5500_Read_Word(PX_DEVICE_W5500 *device)
{
	px_u16 word;
	PX_LinkerRead(device->linker,&word,2);
	return (word>>8)|((word&0x00ff)<<8);
}

px_u8 PX_DEVICE_W5500_Read_RegisterByte(PX_DEVICE_W5500 *device,px_u8 reg)
{
	unsigned char i;
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM1|PX_DEVICE_W5500_RWB_READ|PX_DEVICE_W5500_COMMON_R);

	i=PX_DEVICE_W5500_Read_Byte(device);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);
	return i;
}

px_void PX_DEVICE_W5500_Write_RegisterByte(PX_DEVICE_W5500 *device,px_u16 reg, px_u8 dat)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM1|PX_DEVICE_W5500_RWB_WRITE|PX_DEVICE_W5500_COMMON_R);
	PX_DEVICE_W5500_Write_Byte(device,dat);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);
}



px_u16 PX_DEVICE_W5500_Read_RegisterWord(PX_DEVICE_W5500 *device,px_u16 reg)
{
	px_u16 i;
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM2|PX_DEVICE_W5500_RWB_READ|PX_DEVICE_W5500_COMMON_R);

	i=PX_DEVICE_W5500_Read_Word(device);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);
	return i;
}

px_void PX_DEVICE_W5500_Write_RegisterWord(PX_DEVICE_W5500 *device,px_u16 reg, px_u16 dat)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM2|PX_DEVICE_W5500_RWB_WRITE|PX_DEVICE_W5500_COMMON_R);
	PX_DEVICE_W5500_Write_WORD(device,dat);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);
}


px_void PX_DEVICE_W5500_Write_RegisterMultiByte(PX_DEVICE_W5500 *device,px_u16 reg,px_u8 *dat_ptr,px_int size)
{
	unsigned short i;

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_WRITE|PX_DEVICE_W5500_COMMON_R);

	for(i=0;i<size;i++)
	{
		PX_DEVICE_W5500_Write_Byte(device,*dat_ptr++);
	}

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);
}

px_void PX_DEVICE_W5500_Read_RegisterMultiByte(PX_DEVICE_W5500 *device,px_u16 reg,px_u8 *dat_ptr,px_int size)
{
	unsigned short i;

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_READ|PX_DEVICE_W5500_COMMON_R);

	for(i=0;i<size;i++)
	{
		*dat_ptr++=PX_DEVICE_W5500_Read_Byte(device);
	}

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);
}



px_u8 PX_DEVICE_W5500_Read_SocketRegisterByte(PX_DEVICE_W5500 *device,px_int s,px_u16 reg)
{
	px_u8 i;

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM1|PX_DEVICE_W5500_RWB_READ|(s*0x20+0x08));

	i=PX_DEVICE_W5500_Read_Byte(device);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);
	return i;
}

px_void PX_DEVICE_W5500_Write_SocketRegisterByte(PX_DEVICE_W5500 *device,px_int s,px_u16 reg,px_u8 byte)
{

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM1|PX_DEVICE_W5500_RWB_WRITE|(s*0x20+0x08));

	PX_DEVICE_W5500_Write_Byte(device,byte);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

}

px_void PX_DEVICE_W5500_Write_SocketRegisterWord(PX_DEVICE_W5500 *device,px_int s,px_u16 reg,px_u16 word)
{

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM2|PX_DEVICE_W5500_RWB_WRITE|(s*0x20+0x08));

	PX_DEVICE_W5500_Write_WORD(device,word);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

}

px_u16 PX_DEVICE_W5500_Read_SocketRegisterWord(PX_DEVICE_W5500 *device,px_int s,px_u16 reg)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM2|PX_DEVICE_W5500_RWB_READ|(s*0x20+0x08));

	reg=PX_DEVICE_W5500_Read_Byte(device)<<8;
	reg|=PX_DEVICE_W5500_Read_Byte(device);


	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);
	return reg;
}



px_void PX_DEVICE_W5500_Write_SocketRegisterDword(PX_DEVICE_W5500 *device,px_int s,px_u16 reg,px_u32 dword)
{

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM4|PX_DEVICE_W5500_RWB_WRITE|(s*0x20+0x08));

	PX_DEVICE_W5500_Write_DWORD(device,dword);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

}

px_void PX_DEVICE_W5500_Write_SocketRegisterDWORD(PX_DEVICE_W5500 *device,px_int s,px_u16 reg,px_u8 *data)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_FDM4|PX_DEVICE_W5500_RWB_WRITE|(s*0x20+0x08));

	
	PX_DEVICE_W5500_Write_Byte(device,*data++);
	PX_DEVICE_W5500_Write_Byte(device,*data++);
	PX_DEVICE_W5500_Write_Byte(device,*data++);
	PX_DEVICE_W5500_Write_Byte(device,*data++);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);
}


px_void PX_DEVICE_W5500_Write_SocketRegisterMultiByte(PX_DEVICE_W5500 *device,px_int s,px_u16 reg,px_u8 *data,px_int size)
{
	if(size==0) return;
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_WRITE|(s*0x20+0x08));

	while(size--)
	PX_DEVICE_W5500_Write_Byte(device,*data++);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

}

px_void PX_DEVICE_W5500_Read_SocketRegisterMultiByte(PX_DEVICE_W5500 *device,px_int s,px_u16 reg,px_u8 *data,px_int size)
{
	if(size==0) return;
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,reg);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_READ|(s*0x20+0x08));

	while(size--)
		*data++=PX_DEVICE_W5500_Read_Byte(device);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

}

px_void PX_DEVICE_W5500WriteSocketDataBuffer(PX_DEVICE_W5500 *device,px_int s, px_u8 *dat_ptr, px_u16 size)
{
	px_u16 offset,offset1;
	px_u16 i;

	offset=PX_DEVICE_W5500_Read_SocketRegisterWord(device,s,PX_DEVICE_W5500_Sn_TX_WR);
	offset1=offset;
	offset&=(PX_DEVICE_W5500_S_TX_SIZE-1);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,offset);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_WRITE|(s*0x20+0x10));


	if((offset+size)<PX_DEVICE_W5500_S_TX_SIZE)
	{
		for(i=0;i<size;i++)
		{
			PX_DEVICE_W5500_Write_Byte(device,*dat_ptr++);
		}
	}
	else
	{
		offset=PX_DEVICE_W5500_S_TX_SIZE-offset;
		for(i=0;i<offset;i++)
		{
			PX_DEVICE_W5500_Write_Byte(device,*dat_ptr++);
		}
		PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);

		PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

		PX_DEVICE_W5500_Write_WORD(device,0x00);
		PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_WRITE|(s*0x20+0x10));

		for(;i<size;i++)
		{
			PX_DEVICE_W5500_Write_Byte(device,*dat_ptr++);
		}
	}
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);

	offset1+=size;
	PX_DEVICE_W5500_Write_SocketRegisterWord(device,s, PX_DEVICE_W5500_Sn_TX_WR, offset1);
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,s, PX_DEVICE_W5500_Sn_CR, PX_DEVICE_W5500_SEND);			
}


px_u16 PX_DEVICE_W5500ReadSocketDataBuffer(PX_DEVICE_W5500 *device,px_int s,px_u8 *header_ptr, px_u8 *dat_ptr)
{
	unsigned short rx_size;
	unsigned short offset, offset1;
	unsigned short i;
	unsigned char byte;

	rx_size=PX_DEVICE_W5500_Read_SocketRegisterWord(device,s,PX_DEVICE_W5500_Sn_RX_RSR);
	if(rx_size<=8) return 0;

	offset=PX_DEVICE_W5500_Read_SocketRegisterWord(device,s,PX_DEVICE_W5500_Sn_RX_RD);
	offset1=offset;
	offset&=(PX_DEVICE_W5500_S_RX_SIZE-1);

	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

	PX_DEVICE_W5500_Write_WORD(device,offset);
	PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_READ|(s*0x20+0x18));


	if((offset+rx_size)<PX_DEVICE_W5500_S_RX_SIZE)
	{
		for(i=0;i<rx_size;i++)
		{
			byte=PX_DEVICE_W5500_Read_Byte(device);
			if(i<8)
			*header_ptr++=byte;
			else
			*dat_ptr++=byte;
		}
	}
	else
	{
		offset=PX_DEVICE_W5500_S_RX_SIZE-offset;
		for(i=0;i<offset;i++)
		{
			byte=PX_DEVICE_W5500_Read_Byte(device);
			if(i<8)
			*header_ptr++=byte;
			else
			*dat_ptr++=byte;
		}
		PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);

		PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,0,PX_NULL);

		PX_DEVICE_W5500_Write_WORD(device,00);
		PX_DEVICE_W5500_Write_Byte(device,PX_DEVICE_W5500_VDM|PX_DEVICE_W5500_RWB_READ|(s*0x20+0x18));

		for(;i<rx_size;i++)
		{
			byte=PX_DEVICE_W5500_Read_Byte(device);
			if(i<8)
			*header_ptr++=byte;
			else
			*dat_ptr++=byte;
		}
	}
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SCS,1,PX_NULL);

	offset1+=rx_size;

	PX_DEVICE_W5500_Write_SocketRegisterWord(device,s, PX_DEVICE_W5500_Sn_RX_RD, offset1);
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,s, PX_DEVICE_W5500_Sn_CR, PX_DEVICE_W5500_RECV);	
	return rx_size;
}

px_void PX_DEVICE_W5500Reset(PX_DEVICE_W5500 *device)
{
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_RST,0,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SLEEPMS,50,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_RST,1,PX_NULL);
	PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SLEEPMS,200,PX_NULL);
	while((PX_DEVICE_W5500_Read_RegisterByte(device,PX_DEVICE_W5500_PHYCFGR)&PX_DEVICE_W5500_LINK)==0);

	PX_DEVICE_W5500_Write_RegisterByte(device,PX_DEVICE_W5500_MR,PX_DEVICE_W5500_RST);
	while((PX_DEVICE_W5500_Read_RegisterByte(device,PX_DEVICE_W5500_MR))&PX_DEVICE_W5500_RST);

	
}

px_bool PX_DEVICE_W5500Init(PX_DEVICE_W5500 *device,PX_Linker *linker)
{
	device->linker=linker;
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetGateWay(PX_DEVICE_W5500 *device,px_u8 gateway[4])
{
	px_u8 verify[4],i;
	PX_DEVICE_W5500_Write_RegisterMultiByte(device,PX_DEVICE_W5500_GAR,gateway,4);
	PX_DEVICE_W5500_Read_RegisterMultiByte(device,PX_DEVICE_W5500_GAR,verify,4);
	for (i=0;i<4;i++)
	{
		if (verify[i]!=gateway[i])
		{
			return PX_FALSE;
		}
	}
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetSubMask(PX_DEVICE_W5500 *device,px_u8 submask[4])
{
	px_u8 verify[4],i;
	PX_DEVICE_W5500_Write_RegisterMultiByte(device,PX_DEVICE_W5500_SUBR,submask,4);
	PX_DEVICE_W5500_Read_RegisterMultiByte(device,PX_DEVICE_W5500_SUBR,verify,4);
	for (i=0;i<4;i++)
	{
		if (verify[i]!=submask[i])
		{
			return PX_FALSE;
		}
	}
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetPhyAddr(PX_DEVICE_W5500 *device,px_u8 phy[6])
{
	px_u8 verify[6],i;
	PX_DEVICE_W5500_Write_RegisterMultiByte(device,PX_DEVICE_W5500_SHAR,phy,6);
	PX_DEVICE_W5500_Read_RegisterMultiByte(device,PX_DEVICE_W5500_SHAR,verify,6);
	for (i=0;i<6;i++)
	{
		if (verify[i]!=phy[i])
		{
			return PX_FALSE;
		}
	}
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetLocalIP(PX_DEVICE_W5500 *device,px_u8 ip[4])
{
	px_u8 verify[4],i;
	PX_DEVICE_W5500_Write_RegisterMultiByte(device,PX_DEVICE_W5500_SIPR,ip,4);
	PX_DEVICE_W5500_Read_RegisterMultiByte(device,PX_DEVICE_W5500_SIPR,verify,4);
	for (i=0;i<4;i++)
	{
		if (verify[i]!=ip[i])
		{
			return PX_FALSE;
		}
	}
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetRxTxBuffer(PX_DEVICE_W5500 *device,px_int s,px_u8 recvkb,px_u8 sendkb)
{
	px_u8 verify;
	if (recvkb+sendkb>16)
	{
		return PX_FALSE;
	}
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_RXBUF_SIZE,recvkb);
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_TXBUF_SIZE,sendkb);

	verify=PX_DEVICE_W5500_Read_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_RXBUF_SIZE);
	if (verify!=recvkb)
	{
		return PX_FALSE;
	}
	verify=PX_DEVICE_W5500_Read_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_TXBUF_SIZE);
	if (verify!=sendkb)
	{
		return PX_FALSE;
	}
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetTCPRetryTime(PX_DEVICE_W5500 *device,px_u16 n100us)
{
	px_u16 verify;
	PX_DEVICE_W5500_Write_RegisterWord(device,PX_DEVICE_W5500_RTR,n100us);
	verify=PX_DEVICE_W5500_Read_RegisterWord(device,PX_DEVICE_W5500_RTR);
	if (verify!=n100us)
	{
		return PX_FALSE;
	}
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetTCPRetryTimes(PX_DEVICE_W5500 *device,px_int s,px_u16 times)
{
	px_u16 verify;
	PX_DEVICE_W5500_Write_RegisterByte(device,PX_DEVICE_W5500_RCR,times);
	verify=PX_DEVICE_W5500_Read_RegisterByte(device,PX_DEVICE_W5500_RCR);
	if (verify!=times)
	{
		return PX_FALSE;
	}
	return PX_TRUE;
}


px_bool PX_DEVICE_W5500DetectGateWay(PX_DEVICE_W5500 *device)
{
	px_u8 ip_adde[4],reg;px_u16 retryTime=0xffff;
	PX_DEVICE_W5500_Read_RegisterMultiByte(device,PX_DEVICE_W5500_SIPR,ip_adde,4);
	ip_adde[0]++;
	ip_adde[1]++;
	ip_adde[2]++;
	ip_adde[3]++;

	PX_DEVICE_W5500SetSocketTarget(device,0,ip_adde,8080);
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_MR,PX_DEVICE_W5500_MR_TCP);
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_OPEN);
	
  
	while((reg=PX_DEVICE_W5500_Read_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_SR))!=PX_DEVICE_W5500_SOCK_INIT)
	{
		if(retryTime--==0)
			return PX_FALSE;
	}

	if( reg!= PX_DEVICE_W5500_SOCK_INIT)
	{
		PX_DEVICE_W5500_Write_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_CLOSE);
		return PX_FALSE;
	}
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_CONNECT);				

	do
	{
		reg=PX_DEVICE_W5500_Read_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_IR);
		if(reg!=0)
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_IR,reg);
		PX_LinkerIOCTL(device->linker,PX_DEVICE_W5500_IOCTL_SLEEPMS,5,PX_NULL);
		if((reg&PX_DEVICE_W5500_IR_TIMEOUT) == PX_DEVICE_W5500_IR_TIMEOUT)
		{
			return PX_FALSE;	
		}
		else if(PX_DEVICE_W5500_Read_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_DHAR) != 0xff)
		{
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,0,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_CLOSE);
			return PX_TRUE;							
		}
	}while(1);
}





px_bool PX_DEVICE_W5500Startup(PX_DEVICE_W5500 *device,px_int s,PX_DEVICE_W5500_PROTOCOL protocol,px_u16 listenPort)
{
	px_u16 retry=0xffff;
	if (!PX_DEVICE_W5500DetectGateWay(device))
	{
		return PX_FALSE;
	}
	PX_DEVICE_W5500_Write_SocketRegisterWord(device,s, PX_DEVICE_W5500_Sn_MSSR, 1460);
	PX_DEVICE_W5500_Write_SocketRegisterWord(device,s, PX_DEVICE_W5500_Sn_PORT, listenPort);
	switch(protocol)
	{
	case PX_DEVICE_W5500_PROTOCOL_UDP:
		{
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_CLOSE);
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_MR,PX_DEVICE_W5500_MR_UDP);
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_OPEN);
			while(PX_DEVICE_W5500_Read_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_SR)!=PX_DEVICE_W5500_SOCK_UDP)
			{
				if(retry--==0)
				{
				PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_CLOSE);
					return PX_FALSE;
				}
			}
			
			return PX_TRUE;
		}
//		break;
	case PX_DEVICE_W5500_PROTOCOL_TCP:
		{
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_CLOSE);
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_MR,PX_DEVICE_W5500_MR_TCP);
			PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_OPEN);
			
			while(PX_DEVICE_W5500_Read_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_SR)!=PX_DEVICE_W5500_SOCK_INIT)
			{
				if(retry--==0)
				{
				PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_CR,PX_DEVICE_W5500_CLOSE);
					return PX_FALSE;
				}
			}
			return PX_TRUE;
		}
//		break;
	}
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500_SocketSetTarget(PX_DEVICE_W5500 *device,px_int s,px_u8 IPAddr[4],px_u16 port)
{
	PX_DEVICE_W5500_Write_SocketRegisterMultiByte(device,s, PX_DEVICE_W5500_Sn_DIPR, IPAddr,4);	
	PX_DEVICE_W5500_Write_SocketRegisterWord(device,s, PX_DEVICE_W5500_Sn_DIPR, port);	
	return PX_TRUE;
}

px_bool PX_DEVICE_W5500SetSocketInterrupt(PX_DEVICE_W5500 *device,px_int s,px_u8 interruptmask)
{
	PX_DEVICE_W5500_Write_RegisterByte(device,PX_DEVICE_W5500_IMR,PX_DEVICE_W5500_IM_IR7 | PX_DEVICE_W5500_IM_IR6);
	PX_DEVICE_W5500_Write_RegisterByte(device,PX_DEVICE_W5500_SIMR,1<<s);
	PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_IMR, interruptmask);

	if (PX_DEVICE_W5500_Read_RegisterByte(device,PX_DEVICE_W5500_IMR)!=(PX_DEVICE_W5500_IM_IR7 | PX_DEVICE_W5500_IM_IR6))
	{
		return PX_FALSE;
	}

	if (PX_DEVICE_W5500_Read_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_IMR)!=interruptmask)
	{
		return PX_FALSE;
	}

	return PX_TRUE;
}

px_u16 PX_DEVICE_W5500GetSocketInterrupt(PX_DEVICE_W5500 *device,px_int s)
{
	px_u16 w5500_interrupt=0;
	px_u8 byint;

	byint=PX_DEVICE_W5500_Read_RegisterByte(device,PX_DEVICE_W5500_IR);

	PX_DEVICE_W5500_Write_RegisterByte(device,PX_DEVICE_W5500_IR,byint&(0xf0));

	if (byint&PX_DEVICE_W5500_CONFLICT)
	{
		w5500_interrupt|=PX_DEVICE_W5500_INTERRUPT_IP_CONFLICT;
	}

	if (byint&PX_DEVICE_W5500_UNREACH)
	{
		w5500_interrupt|=PX_DEVICE_W5500_INTERRUPT_UDP_UNREACH;
	}

	byint=PX_DEVICE_W5500_Read_RegisterByte(device,PX_DEVICE_W5500_SIR);

	if (byint&(1<<s))
	{
		byint=PX_DEVICE_W5500_Read_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_IR);
		PX_DEVICE_W5500_Write_SocketRegisterByte(device,s,PX_DEVICE_W5500_Sn_IR,byint);
		
		if (byint&PX_DEVICE_W5500_IR_CON)
		{
			w5500_interrupt|=PX_DEVICE_W5500_INTERRUPT_TCP_CONNECT;
		}

		if (byint&PX_DEVICE_W5500_IR_DISCON)
		{
			w5500_interrupt|=PX_DEVICE_W5500_INTERRUPT_TCP_DISCONNECT;
		}

		if (byint&PX_DEVICE_W5500_IR_SEND_OK)
		{
			w5500_interrupt|=PX_DEVICE_W5500_INTERRUPT_SEND_OK;
		}

		if (byint&PX_DEVICE_W5500_IR_RECV)
		{
			w5500_interrupt|=PX_DEVICE_W5500_INTERRUPT_RECV_OK;
		}

		if (byint&PX_DEVICE_W5500_IR_TIMEOUT)
		{
			w5500_interrupt|=PX_DEVICE_W5500_INTERRUPT_TCP_TIMEOUT;
		}
	}
	return w5500_interrupt;
}

px_bool PX_DEVICE_W5500SetSocketTarget(PX_DEVICE_W5500 *device,px_int s,px_u8 ip[4],px_u16 port)
{
	PX_DEVICE_W5500_Write_SocketRegisterDWORD(device,s, PX_DEVICE_W5500_Sn_DIPR, ip);
	PX_DEVICE_W5500_Write_SocketRegisterWord(device,s, PX_DEVICE_W5500_Sn_DPORTR,port);

	return PX_TRUE;
}





