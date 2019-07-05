#include "GCOSNETWORK.h"

#include <stm32f4xx.h>
#include <stm32f4xx.h>
#include "stm32f4xx_it.h"
//static px_bool GCOS_NETWORK_SENDING=PX_FALSE;

static PX_DEVICE_W5500 GCOS_W5500;
static PX_Linker GCOS_W5500_Linker;
//////////////////////////////////////////////////////////////////////////
//Extern hard-device communications

px_bool PX_PTOTOCAL_W5500_SPI_Init(px_void *param)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	SPI_I2S_DeInit(SPI1);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(SPI1, &SPI_InitStructure);
  	SPI_Cmd(SPI1, ENABLE);

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;	 
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;	 
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    		
		
		return PX_TRUE;
}

px_int PX_PROTOCAL_W5500_IOCTL(px_int ioctl,px_int io,px_void *param)
{
	/*
	PX_DEVICE_VS1003B_IOCTL_SLEEPMS,
	PX_DEVICE_VS1003B_IOCTL_RST,
	PX_DEVICE_VS1003B_IOCTL_DREQ,
	PX_DEVICE_VS1003B_IOCTL_TXDCS,
	PX_DEVICE_VS1003B_IOCTL_TCS,
	*/
	px_int v=io;
	px_u16 PIN;
	if(ioctl==PX_DEVICE_W5500_IOCTL_RST) PIN=GPIO_Pin_1;
	if(ioctl==PX_DEVICE_W5500_IOCTL_INT) PIN=GPIO_Pin_2;
	if(ioctl==PX_DEVICE_W5500_IOCTL_NC) PIN=GPIO_Pin_3;
	if(ioctl==PX_DEVICE_W5500_IOCTL_SCS) PIN=GPIO_Pin_4;
	
	switch(ioctl)
	{
		case PX_DEVICE_W5500_IOCTL_SLEEPMS:
			sleepms(v);
			break;
		case PX_DEVICE_W5500_IOCTL_RST:
		case PX_DEVICE_W5500_IOCTL_NC:
		case PX_DEVICE_W5500_IOCTL_SCS:
		{
			if(v) 
				GPIO_SetBits(GPIOA, PIN);
			else
				GPIO_ResetBits(GPIOA, PIN);
		}
		break;
	}
	return 0;
}

px_int PX_PROTOCAL_W5500_Write(px_void *buffer,px_int size)
{
	px_char *p=(px_char *)buffer;
	while(size--) 
	{
		while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET);
		SPI1->DR = *p;
		while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET);
		SPI1->DR;
		p++;
	}
	return 1;
}

px_int PX_PROTOCAL_W5500_Read(px_void *buffer,px_int size)
{
	px_char *p=(px_char *)buffer;
	while(size--) 
	{
		while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET);
		SPI1->DR = 0x00;
		while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET);
		*p=SPI1->DR ;
		p++;
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////



static px_int GCOS_NETWORK_atoi(px_char s[])
{
	px_int i=0,n=0,sign=1;
	for(i=0;s[i]==' ';i++)
		sign=(s[i]=='-')?-1:1;
	if(s[i]=='+'||s[i]=='-')
		i++;
	for(n=0;s[i]>='0'&&s[i]<='9';i++)
		n=10*n+(s[i]-'0');
	return sign *n;
}


static px_bool GCOS_NETWORK_atoip(px_char *s,px_uchar ipaddr[4])
{
	px_char *p=s;
	px_char nn[4]={0};
	px_int oft,i,val;
	px_int dotcount=0;
	while (*p)
	{
		if (*p<'0'||*p>'9')
		{
			if (*p!='.'||dotcount>=3)
			{
				return PX_FALSE;
			}
			else
			{
				dotcount++;
			}
		}
		p++;
	}
	if (dotcount!=3)
	{
		return PX_FALSE;
	}
	p=s;
	for (i=0;i<4;i++)
	{
		oft=0;
		while (*p!='.'&&*p!=0)
		{
			nn[oft++]=*p;
			p++;
			if (oft>=4)
			{
				return PX_FALSE;
			}
		}
		if (oft==0)
		{
			return PX_FALSE;
		}
		nn[oft]=0;
		p++;
		if ((val=GCOS_NETWORK_atoi(nn))>255)
		{
			return PX_FALSE;
		}
		ipaddr[i]=(px_uchar)val;
	}
	return PX_TRUE;
}

px_bool GCOS_UDPInit(px_char *localip,px_char *masker,px_char *gateway,px_u16 listenPort)
{
	px_uchar addr[4];
	volatile px_uchar state;

	PX_LinkerInit(&GCOS_W5500_Linker,PX_PTOTOCAL_W5500_SPI_Init,PX_PROTOCAL_W5500_Write,PX_PROTOCAL_W5500_Read,PX_PROTOCAL_W5500_IOCTL,PX_NULL);
	if(!PX_DEVICE_W5500Init(&GCOS_W5500,&GCOS_W5500_Linker))
		return PX_FALSE;

	PX_DEVICE_W5500Reset(&GCOS_W5500);
	if(!GCOS_NETWORK_atoip(gateway,addr))
	{
		return PX_FALSE;
	}
	
	if(!PX_DEVICE_W5500SetGateWay(&GCOS_W5500,addr))
	{
		return PX_FALSE;
	}
	
	if(!GCOS_NETWORK_atoip(masker,addr))
	{
		return PX_FALSE;
	}
	
	if(!PX_DEVICE_W5500SetSubMask(&GCOS_W5500,addr))
	{
		return PX_FALSE;
	}

	if(!PX_DEVICE_W5500SetPhyAddr(&GCOS_W5500,(px_u8 *)("\x0c\x29\xab\x7c\x00\x02")))
	{
		return PX_FALSE;
	}

	if(!GCOS_NETWORK_atoip(localip,addr))
	{
		return PX_FALSE;
	}

	if(!PX_DEVICE_W5500SetLocalIP(&GCOS_W5500,addr))
	{
		return PX_FALSE;
	}


	if(!PX_DEVICE_W5500SetRxTxBuffer(&GCOS_W5500,0,PX_DEVICE_W5500_S_RX_SIZE/1024,PX_DEVICE_W5500_S_TX_SIZE/1024))
	{
		return PX_FALSE;
	}

	if(!PX_DEVICE_W5500SetTCPRetryTime(&GCOS_W5500,2000))
	{
		return PX_FALSE;
	}

	if(!PX_DEVICE_W5500SetTCPRetryTimes(&GCOS_W5500,0,8))
	{
		return PX_FALSE;
	}
 
	if(!PX_DEVICE_W5500SetSocketInterrupt(&GCOS_W5500,0, PX_DEVICE_W5500_IMR_SENDOK | PX_DEVICE_W5500_IMR_TIMEOUT | \
		PX_DEVICE_W5500_IMR_RECV | PX_DEVICE_W5500_IMR_DISCON | PX_DEVICE_W5500_IMR_CON))
	{
		return PX_FALSE;
	}


	if (!PX_DEVICE_W5500Startup(&GCOS_W5500,0,PX_DEVICE_W5500_PROTOCOL_UDP,GCOS_UDP_DEFAULE_PORT))
	{
		return PX_FALSE;
	}
	
	return PX_TRUE;
}

px_int GCOS_UDPWrite(GCOS_addr_in *target,px_byte *buffer,px_int bufferSize)
{
	static GCOS_addr_in __lastin;

	if (__lastin.S_un.target_ip_u32!=target->S_un.target_ip_u32||__lastin.target_port!=target->target_port)
	{
		PX_DEVICE_W5500SetSocketTarget(&GCOS_W5500,0,target->S_un.target_ip_byte,target->target_port);
		__lastin=*target;
	}

	PX_DEVICE_W5500WriteSocketDataBuffer(&GCOS_W5500,0,(px_uchar *)buffer,(px_u16)bufferSize);

	
	return bufferSize;
}

GCOS_addr_in GCOS_UDP_TARGET_ADDRIN(px_char *ipaddr,px_u16 port)
{
	GCOS_addr_in __addr_in;
	__addr_in.target_port=(port);
	GCOS_NETWORK_atoip(ipaddr,__addr_in.S_un.target_ip_byte);
	return __addr_in;
}

px_int GCOS_UDPRead(GCOS_addr_in *target,px_byte *buffer)
{
	px_u16 size;
	px_u8 header[8];
	if (!(PX_DEVICE_W5500GetSocketInterrupt(&GCOS_W5500,0)&PX_DEVICE_W5500_INTERRUPT_RECV_OK))
	{
		return PX_FALSE;
	}
	size=PX_DEVICE_W5500ReadSocketDataBuffer(&GCOS_W5500,0,header,(px_uchar *)buffer);
	
	target->S_un.target_ip_byte[0]=header[0];
	target->S_un.target_ip_byte[1]=header[1];
	target->S_un.target_ip_byte[2]=header[2];
	target->S_un.target_ip_byte[3]=header[3];
	
	target->target_port=(header[4]<<8)|header[5];
	return size-8;
}

px_void GCOS_UDPFree(){}
