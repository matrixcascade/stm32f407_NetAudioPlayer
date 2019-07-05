#include "GCOSNETWORK.h"
#include "PX_DEVICE_LED.h"
#include "stm32f4xx_it.h"
#include "PX_DEVICE_VS10xx.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////
//VS10xx Extern hard-device communications

px_bool PX_PTOTOCAL_VS10xx_SPI_Init(px_void *param)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);  

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;   
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   
	SPI_InitStructure.SPI_CPOL  = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;	 //NSS
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;	 //XDCS
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;	//RST
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;	//DREQ
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    		
		
	return PX_TRUE;
}

px_int PX_PROTOCAL_VS10xx_IOCTL(px_int ioctl,px_int io,px_void *param)
{
	/*
	PX_DEVICE_VS10xx_IOCTL_SLEEPMS,
	PX_DEVICE_VS10xx_IOCTL_XDCS,
	PX_DEVICE_VS10xx_IOCTL_XCS,
	PX_DEVICE_VS10xx_IOCTL_DREQ,
	PX_DEVICE_VS10xx_IOCTL_RST,
	PX_DEVICE_VS10xx_IOCTL_SPI_HIGHSPEED,
	PX_DEVICE_VS10xx_IOCTL_SPI_LOWSPEED,
	*/
	px_int v=io;
	px_u16 PIN;
	if(ioctl==PX_DEVICE_VS10xx_IOCTL_XDCS) PIN=GPIO_Pin_11;
	if(ioctl==PX_DEVICE_VS10xx_IOCTL_XCS) PIN=GPIO_Pin_12;
	if(ioctl==PX_DEVICE_VS10xx_IOCTL_DREQ) PIN=GPIO_Pin_10;
	if(ioctl==PX_DEVICE_VS10xx_IOCTL_RST) PIN=GPIO_Pin_9;
	
	switch(ioctl)
	{
		case PX_DEVICE_VS10xx_IOCTL_SLEEPMS:
			sleepms(v);
			break;
		case PX_DEVICE_VS10xx_IOCTL_XDCS:
		case PX_DEVICE_VS10xx_IOCTL_XCS:
		case PX_DEVICE_VS10xx_IOCTL_RST:
		{
			if(v) 
				GPIO_SetBits(GPIOB, PIN);
			else
				GPIO_ResetBits(GPIOB, PIN);
		}
		break;
		case PX_DEVICE_VS10xx_IOCTL_DREQ:
			return GPIOB->IDR & PIN;
		case PX_DEVICE_VS10xx_IOCTL_SPI_HIGHSPEED:
		//	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler_4));
			SPI2->CR1&=0XFFC7;
			SPI2->CR1|=SPI_BaudRatePrescaler_4;
			SPI_Cmd(SPI2,ENABLE);
			break;
		case PX_DEVICE_VS10xx_IOCTL_SPI_LOWSPEED:
		//	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler_256));
			SPI2->CR1&=0XFFC7;
			SPI2->CR1|=SPI_BaudRatePrescaler_256;
			SPI_Cmd(SPI2,ENABLE);
			break;
	}
	return 0;
}

px_int PX_PROTOCAL_VS10xx_Write(px_void *buffer,px_int size)
{
	px_char *p=(px_char *)buffer;
	while(size--) 
	{
		while((SPI2->SR&SPI_I2S_FLAG_TXE)==RESET);
		SPI2->DR = *p;
		while((SPI2->SR&SPI_I2S_FLAG_RXNE)==RESET);
		SPI2->DR;
		p++;
	}
	return 1;
}

px_int PX_PROTOCAL_VS10xx_Read(px_void *buffer,px_int size)
{
	px_char *p=(px_char *)buffer;
	while(size--) 
	{
		while((SPI2->SR&SPI_I2S_FLAG_TXE)==RESET);
		SPI2->DR = 0x00;
		while((SPI2->SR&SPI_I2S_FLAG_RXNE)==RESET);
		*p=SPI2->DR ;
		p++;
	}
	return 1;
}

px_bool PX_DEVICE_LED_Init(px_void *param)
{
	px_int i;
	GPIO_InitTypeDef GPIO_InitStructure;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;	 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;	 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	for(i=0;i<6;i++)
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
		sleepms(50);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
		sleepms(50);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
		sleepms(50);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
		sleepms(50);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		sleepms(50);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		sleepms(50);
	};
	
	return PX_TRUE;
}

px_int PX_DEVICE_LED_IO(px_int ioctl,px_int io,px_void *param)
{
	px_u16 PIN;
	if(ioctl==PX_DEVICE_LED_IOCTL_RED) PIN=GPIO_Pin_5;
	if(ioctl==PX_DEVICE_LED_IOCTL_GREEN) PIN=GPIO_Pin_4;
	if(ioctl==PX_DEVICE_LED_IOCTL_YELLOW) PIN=GPIO_Pin_6;
	
		if (io)
		{
			GPIO_SetBits(GPIOB,PIN);
		}
		else
		{
			GPIO_ResetBits(GPIOB,PIN);
		}
 return 0;
}

#define PX_NETPLAYER_SERVER_PORT 12345
#define PX_NETPLAYER_CACHE_TOKEN_SIZE 1024
#define PX_NETPLAYER_CACHE_TOKEN	100
#define PX_NETPLAYER_QUEUE_LEN	 8
#define PX_NETPLAYER_QUEUE_RETRY_TIME	 300
#define PX_NETPLAYER_QUEUE_SEND_INVERVAL	 10
typedef enum
{
	PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_MUSIC,
	PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_PACKET,
	PX_NETPLAYER_PACKET_TYPE_INFO_AUDIO_PACKETINFO,
	PX_NETPLAYER_PACKET_TYPE_INFO_DATA,
}PX_NETPLAYER_PACKET_TYPE;

typedef enum
{
	PX_NETPLAYER_STATUS_STANDBY,
	PX_NETPLAYER_STATUS_PLAYING,
}PX_NETPLAYER_STATUS;

typedef struct  
{
	px_u32 type;
	px_u32 param[3];
	px_u8  data[PX_NETPLAYER_CACHE_TOKEN_SIZE];
}PX_NETPLAYER_PACKET;

typedef struct 
{
	px_int	p_index;
	px_uint	timestamp;
}s_queue;

px_char Audio_Buffer[PX_NETPLAYER_CACHE_TOKEN_SIZE*PX_NETPLAYER_CACHE_TOKEN];//100kb for buffer 100 tokens
s_queue queue[PX_NETPLAYER_QUEUE_LEN]; //queue

int main()
{
   PX_Linker VS10xxLinker,LEDLinker;
   PX_DEVICE_VS10xx VS10xx;
   PX_DEVICE_LED  LED;
   GCOS_addr_in gcos_addr_in,gcos_addr_out;
   px_byte zeroByte32[32]={0};
   px_int TokenCurrentPlayIndex=0;
   px_int TokenCurrentPlayOffset=0;
   px_int TokenLastRequestIndex=0;
   px_int TokenLastWrite=0;
   px_int TokenCount=0;
   px_int TokenFreeCount=PX_NETPLAYER_CACHE_TOKEN;
   px_int calcTi,i,lastQueueStamp=0;
   px_int CurrentMusicIndex=0;
   px_int outline=0;
   px_uint lastsendtime;
   px_bool LED_Green=PX_FALSE,LED_Yellow=PX_FALSE;
   PX_NETPLAYER_STATUS status=PX_NETPLAYER_STATUS_STANDBY;
   PX_NETPLAYER_PACKET Packet;

   gcos_addr_out=GCOS_UDP_TARGET_ADDRIN("47.104.129.1",PX_NETPLAYER_SERVER_PORT);
   
   TimerInit(); 
	 if (!PX_LinkerInit(&LEDLinker,PX_DEVICE_LED_Init,PX_NULL,PX_NULL,PX_DEVICE_LED_IO,PX_NULL))
		while(1);
	 PX_DEVICE_LEDInit(&LED,&LEDLinker);
	 
	 PX_DEVICE_LED_RED(&LED,PX_TRUE);
	 
   if(!PX_LinkerInit(&VS10xxLinker,PX_PTOTOCAL_VS10xx_SPI_Init,PX_PROTOCAL_VS10xx_Write,PX_PROTOCAL_VS10xx_Read,PX_PROTOCAL_VS10xx_IOCTL,PX_NULL))
	   while(1);

   if(!PX_DEVICE_VS10xxInit(&VS10xx,&VS10xxLinker))
	   while(1);

   if(!GCOS_UDPInit("192.168.1.108","255.255.255.0","192.168.1.1",54321))
	   while(1);

RESET:

   for (i=0;i<PX_NETPLAYER_QUEUE_LEN;i++)
   {
	   queue[i].p_index=-1;
	   queue[i].timestamp=0;
   }
   TokenLastWrite=0;
   TokenCurrentPlayIndex=0;
   TokenCurrentPlayOffset=0;
   TokenLastRequestIndex=0;
   TokenCount=0;
   TokenFreeCount=PX_NETPLAYER_CACHE_TOKEN;
   outline=0;
   lastsendtime=0;
   lastQueueStamp=0;
   status=PX_NETPLAYER_STATUS_STANDBY;

   PX_DEVICE_VS10xxSoftReset(&VS10xx);
   while(1)
   {
	        if (status==PX_NETPLAYER_STATUS_STANDBY)
	        {
				PX_DEVICE_LED_RED(&LED,PX_TRUE);
				Packet.type=PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_MUSIC;
				Packet.param[0]=CurrentMusicIndex;
				GCOS_UDPWrite(&gcos_addr_out,(px_char *)(&Packet),8);
				PX_DEVICE_LED_YELLOW(&LED,LED_Yellow=!LED_Yellow);
				sleepms(1000);
	        }

			if(status==PX_NETPLAYER_STATUS_PLAYING&&TokenLastWrite<TokenCount&&TimeGetTime()-lastsendtime>PX_NETPLAYER_QUEUE_SEND_INVERVAL)
			{
				Packet.type=PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_PACKET;

				for (i=0;i<PX_NETPLAYER_QUEUE_LEN;i++)
				{
					if (lastQueueStamp>=PX_NETPLAYER_QUEUE_LEN)
					{
						lastQueueStamp=0;
					}

					if(queue[lastQueueStamp].p_index!=-1)
					{
						if (TimeGetTime()-queue[lastQueueStamp].timestamp>PX_NETPLAYER_QUEUE_RETRY_TIME)
						{
							Packet.param[0]=queue[lastQueueStamp].p_index;
							queue[lastQueueStamp].timestamp=TimeGetTime();
							lastsendtime=TimeGetTime();
							GCOS_UDPWrite(&gcos_addr_out,(px_char *)(&Packet),8);
							PX_DEVICE_LED_YELLOW(&LED,LED_Yellow=!LED_Yellow);
							if (outline++>128)
							{
							PX_DEVICE_LED_RED(&LED,PX_TRUE);
							goto RESET;
							}
							lastQueueStamp++;
							break;
						}
					}
					else
					{
						if (TokenFreeCount&&TokenLastRequestIndex<TokenCount)
						{
							Packet.param[0]=TokenLastRequestIndex;
							queue[lastQueueStamp].p_index=TokenLastRequestIndex;
							queue[lastQueueStamp].timestamp=TimeGetTime();
							lastsendtime=TimeGetTime();
							GCOS_UDPWrite(&gcos_addr_out,(px_char *)(&Packet),8);
							PX_DEVICE_LED_YELLOW(&LED,LED_Yellow=!LED_Yellow);
							TokenLastRequestIndex++;
							TokenFreeCount--;
							lastQueueStamp++;
							break;
						}
					}
					lastQueueStamp++;
				}

			}
			
			if(status==PX_NETPLAYER_STATUS_PLAYING)
			{
				while (PX_TRUE)
				{
					if(TokenLastWrite-TokenCurrentPlayIndex<=0)
					{
						PX_DEVICE_LED_RED(&LED,PX_TRUE);
						break;
					}
					PX_DEVICE_LED_RED(&LED,PX_FALSE);
					calcTi=TokenCurrentPlayIndex%PX_NETPLAYER_CACHE_TOKEN;
					if (PX_DEVICE_VS10xxWrite(&VS10xx,(px_byte *)(Audio_Buffer+calcTi*PX_NETPLAYER_CACHE_TOKEN_SIZE+TokenCurrentPlayOffset))==0)
					{
						break;
					}
					TokenCurrentPlayOffset+=32;
					if (TokenCurrentPlayOffset>=PX_NETPLAYER_CACHE_TOKEN_SIZE)
					{
						TokenCurrentPlayIndex++;
						TokenCurrentPlayOffset=0;
						TokenFreeCount++;
					}
				}
				if (TokenCurrentPlayIndex>=TokenCount)
				{
					CurrentMusicIndex++;
					for(calcTi=0;calcTi<64;calcTi++)
					{
						while(!PX_DEVICE_VS10xxWrite(&VS10xx,(px_byte *)(zeroByte32)))
							sleepms(5);
					}
					PX_DEVICE_LED_RED(&LED,PX_TRUE);
					goto RESET;
				}
			}

				if (GCOS_UDPRead(&gcos_addr_in,(px_char *)(&Packet))!=0)
				{
					PX_DEVICE_LED_GREEN(&LED,LED_Green=!LED_Green);
					if (status==PX_NETPLAYER_STATUS_STANDBY)
					{
						if (Packet.type==PX_NETPLAYER_PACKET_TYPE_INFO_AUDIO_PACKETINFO)
						{
							if (Packet.param[0]==0)
							{
								CurrentMusicIndex=0;
								goto RESET;
							}
							TokenCurrentPlayIndex=0;
							TokenCurrentPlayOffset=0;
							TokenLastRequestIndex=0;
							TokenLastWrite=0;
							TokenCount=Packet.param[0];
							lastQueueStamp=0;
							TokenFreeCount=PX_NETPLAYER_CACHE_TOKEN;
							status=PX_NETPLAYER_STATUS_PLAYING;
						}
					}
					else if(status==PX_NETPLAYER_STATUS_PLAYING)
					{
						if (Packet.type==PX_NETPLAYER_PACKET_TYPE_INFO_DATA)
						{
							for (i=0;i<PX_NETPLAYER_QUEUE_LEN;i++)
							{
								if (Packet.param[0]==queue[i].p_index)
								{

									calcTi=queue[i].p_index%PX_NETPLAYER_CACHE_TOKEN;
									queue[i].p_index=-1;
									//memory copy
									memcpy(Audio_Buffer+calcTi*PX_NETPLAYER_CACHE_TOKEN_SIZE,Packet.data,sizeof(Packet.data));
									outline=0;
									break;
								}
							}

							TokenLastWrite=TokenLastRequestIndex;
							for (i=0;i<PX_NETPLAYER_QUEUE_LEN;i++)
							{
								if (queue[i].p_index!=-1&&queue[i].p_index<TokenLastWrite)
								{
									TokenLastWrite=queue[i].p_index;
								}
							}
						}
					}
				}
   }
  //while(1);
}
