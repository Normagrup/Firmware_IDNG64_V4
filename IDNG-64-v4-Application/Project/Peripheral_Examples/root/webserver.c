#include "stm32f0xx.h"
#include "w5100/socket.h"
#include "w5100/w5100.h"
#include "dali.h"
#include "wdt.h"
#include "ethTransceiver.h"
#include "config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "systick.h"
#include "mainCfg.h"

unsigned char webServerEstado=0x01;

extern unsigned char _broadcastAddress[4];

void botoneraTestOn (unsigned char period);



int strindex(char *s,char *t)
{
	uint16_t i,n;
	n=strlen(t);
	for(i=0;*(s+i); i++) {
		if (strncmp(s+i,t,n) == 0)
		return i;
	}
	return -1;
}









unsigned char _webserverUtilsStringToIp (char string[], unsigned char *ip)
{
	unsigned char _n;
	char _charString[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	unsigned char _dotCnt;
	unsigned char _strlenght;	
	unsigned char _digitCnt;
	unsigned char _dotLoc[3];
	_dotCnt=0;

	_strlenght=strlen((char *)(string));
	
	// Busqueda de puntos
	for (_n=0;_n<_strlenght;_n++)
	{
		if (string[_n]=='.')
		{
			_dotLoc[_dotCnt]=_n;
			if (_dotCnt<3)
			{
					_dotCnt++;				
			}							
			else
			{
				// Error Handler
			}
		}
	}	
	_digitCnt=0;
	for (_n=0;_n<_dotLoc[0];_n++)
	{
		_charString[0][_digitCnt]=string[_n];	
		_digitCnt++;		
	}	
	_digitCnt=0;
	for (_n=(_dotLoc[0]+1);_n<_dotLoc[1];_n++)
	{
		_charString[1][_digitCnt]=string[_n];		
		_digitCnt++;
	}	
	_digitCnt=0;
	for (_n=(_dotLoc[1]+1);_n<_dotLoc[2];_n++)
	{
		_charString[2][_digitCnt]=string[_n];		
		_digitCnt++;
	}	
	_digitCnt=0;
	for (_n=(_dotLoc[2]+1);_n<_strlenght;_n++)
	{
		_charString[3][_digitCnt]=string[_n];		
		_digitCnt++;
	}	
	ip[0]=atoi(_charString[0]);
	ip[1]=atoi(_charString[1]);
	ip[2]=atoi(_charString[2]);
	ip[3]=atoi(_charString[3]);
	return 0x01;	
}

















void _webserverPrintIp(unsigned char *ip, unsigned char *buffer)
{
	unsigned char _centenas[4];
	unsigned char _decenas[4];
	unsigned char _unidades[4];
	unsigned char _ascii[3];	
	_centenas[0]=ip[0]/100;
	_centenas[1]=ip[1]/100;
	_centenas[2]=ip[2]/100;
	_centenas[3]=ip[3]/100;
	_decenas[0]=(ip[0]/10)-(10*_centenas[0]);
	_decenas[1]=(ip[1]/10)-(10*_centenas[1]);
	_decenas[2]=(ip[2]/10)-(10*_centenas[2]);
	_decenas[3]=(ip[3]/10)-(10*_centenas[3]);
	_unidades[0]=ip[0]-(100*_centenas[0])-(10*_decenas[0]);
	_unidades[1]=ip[1]-(100*_centenas[1])-(10*_decenas[1]);
	_unidades[2]=ip[2]-(100*_centenas[2])-(10*_decenas[2]);						
	_unidades[3]=ip[3]-(100*_centenas[3])-(10*_decenas[3]);				
	if (_centenas[0]==0)
	{
		if (_decenas[0]==0)
		{
			_ascii[0]=48+_unidades[0];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[0];
			_ascii[1]=48+_unidades[0];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[0];	
		_ascii[1]=48+_decenas[0];
		_ascii[2]=48+_unidades[0];							
	}
	strcat((char*)(buffer),(char*)(_ascii));						
	strcat((char*)(buffer),".");							
	if (_centenas[1]==0)
	{
		if (_decenas[1]==0)
		{
			_ascii[0]=48+_unidades[1];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[1];
			_ascii[1]=48+_unidades[1];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[1];	
		_ascii[1]=48+_decenas[1];
		_ascii[2]=48+_unidades[1];							
	}
	strcat((char*)(buffer),(char*)(_ascii));						
	strcat((char*)(buffer),".");								
	if (_centenas[2]==0)
	{
		if (_decenas[2]==0)
		{
			_ascii[0]=48+_unidades[2];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[2];
			_ascii[1]=48+_unidades[2];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[2];	
		_ascii[1]=48+_decenas[2];
		_ascii[2]=48+_unidades[2];							
	}
	strcat((char*)(buffer),(char*)(_ascii));						
	strcat((char*)(buffer),".");							
	if (_centenas[3]==0)
	{
		if (_decenas[3]==0)
		{
			_ascii[0]=48+_unidades[3];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[3];
			_ascii[1]=48+_unidades[3];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[3];	
		_ascii[1]=48+_decenas[3];
		_ascii[2]=48+_unidades[3];							
	}							
	strcat((char*)(buffer),(char*)(_ascii));		
}











unsigned char _webserverUtilsStringFromBufferGet (unsigned char *buffr, unsigned char search, unsigned char *ipString)
{
	unsigned int _ipIndex=0;
	unsigned char _n;
	unsigned char _end=0x00;
	unsigned char _lenght;
	unsigned char _ip=0x00;
	
	for (_n=0;_n<32;_n++)
	{
			ipString[_n]=0;		
	}	
	
	if (search==0x01)
	{
		_lenght=8;
		_ip=0x01;
		_ipIndex=strindex((char *)buffr,"ipValue=")+_lenght;			
	}
	
	if (search==0x02)
	{
		_lenght=13;
		_ip=0x01;
		_ipIndex=strindex((char *)buffr,"submaskValue=")+_lenght;			
	}
	
	if (search==0x03)
	{
		_lenght=9;
		_ip=0x01;
		_ipIndex=strindex((char *)buffr,"gtwValue=")+_lenght;			
	}
	
	if (search==0x04)
	{
		_lenght=15;
		_ipIndex=strindex((char *)buffr,"edificioNombre=")+_lenght;			
	}
	
	if (search==0x05)
	{
		_lenght=12;
		_ipIndex=strindex((char *)buffr,"lineaNombre=")+_lenght;			
	}
	
	if (search==0x06)
	{
		_lenght=10;
		_ipIndex=strindex((char *)buffr,"adminPass=")+_lenght;			
	}
	
	if ((_ipIndex>0) && ((_ipIndex+_lenght+32)<1024))
	{
		_n=0;
		while ((_n<32) && (_end==0x00))
		{			
			if (_ip==0x01)
			{
				if (((buffr[_ipIndex+_n]>=48) && (buffr[_ipIndex+_n]<=57)) || (buffr[_ipIndex+_n]=='.')) 
				{		
					ipString[_n]=buffr[_ipIndex+_n];
				}
				else
				{
					if (buffr[_ipIndex+_n]=='&')
					{
						_end=0x01;
					}			
				}		
			}
			else
			{
				if ((buffr[_ipIndex+_n]!='&') && (_n<16))
				{		
					ipString[_n]=buffr[_ipIndex+_n];
				}
				else
				{
					_end=0x01;						
				}				
			}		
			_n++;
		}
		if (_end==0x01)
		{		
			return 0x01;
		}
		else
		{
			return 0x00;
		}
	}
	else
	{
		return 0x00;
	}
}

void webServerDoTask (void)
{
	const unsigned char _hea[]="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<link rel=\"shortcut icon\" href=\"data:image/x-icon;,\" type=\"image/x-icon\"><html><body>\r\n<p><form method=\"POST\">\r\n";
	unsigned char nombreEdificio[17];
	unsigned char nombreLinea[17];
	unsigned char passAdmin[17];
	unsigned char _ipString[32];
	unsigned char _submaskString[32];
	unsigned char _gtwString[32];
	unsigned char _adminPassString[32];	
	unsigned char _ipAdrress[4];
	unsigned char _subMask[4];
	unsigned char _gtwAddress[4];
	unsigned char _edificioString[32];
	unsigned char _lineaString[32];
	unsigned char _gwadminPassString[32];	
	unsigned char _socketEstado;
	int _postidx;	
	unsigned char _applyBtn=0x00;
	unsigned char _rebootBtn=0x00;
	unsigned char _maxBtn=0x00;
	unsigned char _offBtn=0x00;		
	
	unsigned char _bufferRx[768];
	
	
	unsigned char apply[8];
	unsigned int n;	
	
	#ifdef DEBUG_WEBSERVER_SEND
		unsigned char debugFrame[DEBUG_WEBSERVER_SEND_FRAME_LENGHT];
	#endif
	
	
	if (webServerEstado==0x01)
	{		
		_socketEstado=getSn_SR(1);		
		switch (_socketEstado)
		{
			case SOCK_CLOSED:
			{
				// Crea un socket TCP en puerto 80
				if (socket(1,Sn_MR_TCP,80,0) > 0)
				{
					// Escuchar en el socket 0
					if (listen(1) <= 0)
					{
					}
				}
				break;
			}			
			case SOCK_ESTABLISHED:
			{
				unsigned int _rsize;
				_rsize=getSn_RX_RSR(1);
				if (_rsize>0)
				{
					
					botoneraTestOn (10);
					
					ethTransceiverTiempoSinRecibirCntRst();
					send(1,_hea,strlen((char *)_hea));						
					
					for (n=0;n<768;n++)
					{
						_bufferRx[n]=0x00;
					}								
					
					configIpAddressGet(&_ipAdrress[0]);
					configSubmaskAddressGet(&_subMask[0]);
					configGatewayAddressGet(&_gtwAddress[0]);	
					configNombreEdificioGet(&_edificioString[0]);
					configNombreLineaGet(&_lineaString[0]);
					configAdminPassGet(&_gwadminPassString[0]);
					nombreEdificio[16]=0;
					nombreLinea[16]=0;
					passAdmin[16]=0;	

					if (_rsize<768)
					{
						recv(1,_bufferRx,_rsize);
					}
					
					_postidx=strindex((char *)_bufferRx,"POST /");					
					if (_postidx >= 0) 
					{
						if (strindex((char *)_bufferRx,"ApplyBtn=Apply") > 0)
						{
							_applyBtn=0x01;
						}				

						if (strindex((char *)_bufferRx,"RebootBtn=Reboot") > 0)
						{
							_rebootBtn=0x01;
						}					
						
						if (strindex((char *)_bufferRx,"MaxBtn=Max") > 0)
						{
							_maxBtn=0x01;								
						}

						if (strindex((char *)_bufferRx,"OffBtn=Off") > 0)
						{
							_offBtn=0x01;
						}										

						if (_applyBtn==0x01)
						{						
							_webserverUtilsStringFromBufferGet(&_bufferRx[0],1,&_ipString[0]);
							_webserverUtilsStringFromBufferGet(&_bufferRx[0],2,&_submaskString[0]);				
							_webserverUtilsStringFromBufferGet(&_bufferRx[0],3,&_gtwString[0]);
							_webserverUtilsStringFromBufferGet(&_bufferRx[0],4,&_edificioString[0]);					
							_webserverUtilsStringFromBufferGet(&_bufferRx[0],5,&_lineaString[0]);
							_webserverUtilsStringFromBufferGet(&_bufferRx[0],6,&_adminPassString[0]);							
							_webserverUtilsStringToIp ((char*)(_ipString), &_ipAdrress[0]);
							_webserverUtilsStringToIp ((char*)(_submaskString), &_subMask[0]);
							_webserverUtilsStringToIp ((char*)(_gtwString), &_gtwAddress[0]);				
						}	
						
						if ((_rebootBtn==0x01) || (_maxBtn==0x01) || (_offBtn==0x01))
						{
							_webserverUtilsStringFromBufferGet(&_bufferRx[0],6,&_adminPassString[0]);	
						}
					}									
					// ip
					strcpy((char*)(_bufferRx),"IP Address:<input type=\"text\" name=\"ipValue\" value=\"");
					_webserverPrintIp(&_ipAdrress[0], &_bufferRx[0]);
					strcat((char*)(_bufferRx),"\"><br>\r\n");
					// ip
					// submask
					strcat((char*)(_bufferRx),"Subnet Mask:<input type=\"text\" name=\"submaskValue\" value=\"");
					_webserverPrintIp(&_subMask[0],&_bufferRx[0]);
					strcat((char*)(_bufferRx),"\"><br>\r\n");
					// submask
					// gateway
					strcat((char*)(_bufferRx),"Gateway:<input type=\"text\" name=\"gtwValue\" value=\"");
					_webserverPrintIp(&_gtwAddress[0],&_bufferRx[0]);
					strcat((char*)(_bufferRx),"\"><br>\r\n");
					// gateway					
					// nombre edificio
					strcat((char*)(_bufferRx),"Building:<input type=\"text\" name=\"edificioNombre\" value=\"");
					strcat((char*)(_bufferRx),(char*)(_edificioString));
					strcat((char*)(_bufferRx),"\"><br>\r\n");
					// nombre edificio							
					// nombre linea
					strcat((char*)(_bufferRx),"Line:<input type=\"text\" name=\"lineaNombre\" value=\"");
					strcat((char*)(_bufferRx),(char*)(_lineaString));	
					strcat((char*)(_bufferRx),"\"><br>\r\n");
					// nombre linea							
					// nombre linea
					strcat((char*)(_bufferRx),"Pass Admin:<input type=\"password\" name=\"adminPass\" value=\"");
					//strcat((char*)(_buffer),(char*)(_adminPassString));	
					strcat((char*)(_bufferRx),"\"><br>\r\n");							
					// nombre linea						
					
					#ifdef WEBSERVER_SEARCH
						strcat((char*)(_buffer),"<input type=\"submit\" name=\"SearchBtn\" value=\"Search\">");	
					#endif
					
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"ApplyBtn\" value=\"Apply\">");	
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"RebootBtn\" value=\"Reboot\">");	
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"MaxBtn\" value=\"Max\">");	
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"OffBtn\" value=\"Off\">");	
					if (_postidx >= 0) 
					{
						if (_applyBtn==0x01)
						{
							if (strcmp((char*)(_adminPassString),(char*)(_gwadminPassString))==0x00)
							{
							strcat((char*)(_bufferRx),"<br>\r\n");
							strcat((char*)(_bufferRx),"You must reboot this gateway to apply these changes.\r\n");
							}
							else
							{
							strcat((char*)(_bufferRx),"<br>\r\n");
							strcat((char*)(_bufferRx),"Wrong password.\r\n");
							_applyBtn=0x00;
							}
						}						
						if (_rebootBtn==0x01)
						{
							if (strcmp((char*)(_adminPassString),(char*)(_gwadminPassString))==0x00)
							{
							strcat((char*)(_bufferRx),"<br>\r\n");
							strcat((char*)(_bufferRx),"Reboot request received.\r\n");
							}
							else
							{
							strcat((char*)(_bufferRx),"<br>\r\n");
							strcat((char*)(_bufferRx),"Wrong password.\r\n");
							_rebootBtn=0x00;
							}
						}					
						if (_maxBtn==0x01) 
						{
							strcat((char*)(_bufferRx),"<br>\r\n");
							strcat((char*)(_bufferRx),"Done!\r\n");					
						}						
						if (_offBtn==0x01)
						{
							strcat((char*)(_bufferRx),"<br>\r\n");
							strcat((char*)(_bufferRx),"Done!\r\n");
						}					
					}				
					strcat((char*)(_bufferRx),"<br>\r\n");
					strcat((char*)(_bufferRx),WEBSERVER_TEXT);					
					send(1,_bufferRx,strlen((char *)_bufferRx));		
					sysTickDelay(100);
					disconnect(1);									
					if (_applyBtn==0x01)
					{					
						configIpAddressSet(&_ipAdrress[0], 0x00);
						configSubmaskAddressSet(&_subMask[0],0x00);
						configGatewayAddressSet(&_gtwAddress[0],0x00);
						configNombreEdificioSet(&_edificioString[0],0x00);
						configNombreLineaSet(&_lineaString[0], 0x00);
						configStoreInFlash();
					}									
					if (_rebootBtn==0x01)
					{
						wdtReboot();
					}					
					if (_maxBtn==0x01) 
					{
							#ifdef DEBUG_WEBSERVER_SEND
								debugFrame[0]=DEBUG_FRAME_TYPE;
								debugFrame[1]=0xFF;	
								debugFrame[2]=0xFF;						
								debugFrame[3]=DEBUG_WEBSERVER_SEND_OP;
								debugFrame[4]=DEBUG_WEBSERVER_SEND_SUB_OP_MAX;
								ethSendFrame(0,&debugFrame[0],DEBUG_WEBSERVER_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_WEBSERVER_SEND_PORT);	
							#endif
							daliStop();	
							daliSend(DALI_WEBSERVER, 0xFFFF, 0x01, 0xFF, &apply[0], 0x2B, 0x05);
						
					}									
					if (_offBtn==0x01)
					{						
							#ifdef DEBUG_WEBSERVER_SEND
								debugFrame[0]=DEBUG_FRAME_TYPE;
								debugFrame[1]=0xFF;	
								debugFrame[2]=0xFF;						
								debugFrame[3]=DEBUG_WEBSERVER_SEND_OP;
								debugFrame[4]=DEBUG_WEBSERVER_SEND_SUB_OP_OFF;
								ethSendFrame(0,&debugFrame[0],DEBUG_WEBSERVER_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_WEBSERVER_SEND_PORT);	
							#endif
							daliStop();
							daliSend(DALI_WEBSERVER, 0xFFFF, 0x01, 0xFF, &apply[0], 0x2B, 0x00);							
					}					
				}					
				else
				{
					sysTickDelay(1);
				}			
				break;
			}					
			case SOCK_FIN_WAIT:
      case SOCK_CLOSING:
      case SOCK_TIME_WAIT:
      case SOCK_CLOSE_WAIT:
      case SOCK_LAST_ACK:
			{
				close(1);
				break;
			}			
		}	
	}	
}
