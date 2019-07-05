#include "network/inc/Cube_SocketUDP.h"

#define PX_NETPLAYER_BUFFER_SIZE 1024
#define PX_NETPLAYER_SERVER_PORT 12345

#define PX_NETPLAYER_CACHE_TOKEN_SIZE   1024

typedef enum
{
	PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_MUSIC,
	PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_PACKET,
	PX_NETPLAYER_PACKET_TYPE_INFO_AUDIO_PACKETINFO,
	PX_NETPLAYER_PACKET_TYPE_INFO_DATA,
}PX_NETPLAYER_PACKET_TYPE;

typedef struct  
{
	pt_int	 type;
	pt_int	 param[3];
	pt_byte  data[PX_NETPLAYER_CACHE_TOKEN_SIZE];
}PX_NETPLAYER_PACKET;

int main()
{
	Cube_SocketUDP_IO	_io;
	Cube_SocketUDP_O	_o;
	Cube_SocketUDP		Net;
	SOCKADDR_IN			_inaddr;
	pt_byte				data[PX_NETPLAYER_BUFFER_SIZE];
	pt_byte				rdata[PX_NETPLAYER_CACHE_TOKEN_SIZE];
	pt_string           filePath;
	PX_NETPLAYER_PACKET *pPacket=(PX_NETPLAYER_PACKET *)data;
	PX_NETPLAYER_PACKET Reply;
	pt_int				fileSize,SumToken;
	FILE				*pf=PT_NULL;
	_o.Buffer=&Reply;
	_o.Size=sizeof(Reply);
	_io.Port=PX_NETPLAYER_SERVER_PORT;
	if (!Net.Initialize(_io))
	{
		return 0;
	}

	printf("NetPlayer Server running....\n");
	while (Net.ReceiveData(data,sizeof(data),_inaddr))
	{
		_o.to=_inaddr;
		switch (pPacket->type)
		{
		case PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_MUSIC:
			if (pf!=PT_NULL)
			{
				fclose(pf);
			}

			filePath=pt_string("./")+pt_string().Number(pPacket->param[0])+".data";
			
			if ((pf=fopen(filePath.buffer,"rb"))!=PT_NULL)
			{
				printf("Music Play %s\n",filePath.buffer);
				fseek(pf,0,SEEK_END);
				fileSize=ftell(pf);
				fseek(pf,0,SEEK_SET);

				Reply.type=PX_NETPLAYER_PACKET_TYPE_INFO_AUDIO_PACKETINFO;
				Reply.param[0]=(fileSize)/PX_NETPLAYER_CACHE_TOKEN_SIZE;
				if (fileSize%PX_NETPLAYER_CACHE_TOKEN_SIZE)
				{
					Reply.param[0]++;
				}
				SumToken=Reply.param[0];
				//read first token
				memset(rdata,0,sizeof(rdata));
				fread(rdata,1,sizeof(rdata),pf);
				Net.Send(_o);
			}
			else
			{
				printf("Î´ÕÒµ½ÎÄ¼þ %s\n",filePath.buffer);
				Reply.type=PX_NETPLAYER_PACKET_TYPE_INFO_AUDIO_PACKETINFO;
				Reply.param[0]=0;//No packets
				Net.Send(_o);
			}
			break;
		case PX_NETPLAYER_PACKET_TYPE_COMMAND_QUERY_PACKET:
			{
				if (pf==PT_NULL)
				{
					break;
				}
	
				if(pPacket->param[0]<SumToken)
				{
					fseek(pf,pPacket->param[0]*PX_NETPLAYER_CACHE_TOKEN_SIZE,SEEK_SET);
					memset(rdata,0,sizeof(rdata));
					fread(rdata,1,sizeof(rdata),pf);
					Reply.type=PX_NETPLAYER_PACKET_TYPE_INFO_DATA;
					Reply.param[0]=pPacket->param[0];
					memcpy(Reply.data,rdata,sizeof(rdata));
					Net.Send(_o);
					Sleep(2);
				}
			}
			break;
		}
	}
}