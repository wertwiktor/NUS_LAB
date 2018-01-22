// WinCom.cpp: implementation of the WinCom class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>

#include <memory.h>
#include <process.h>
#include "WinCom.h"


using namespace std;

#define E_NO_FRAME      1
#define E_WRONG_CRC     2

#include "crc16.h"
#define FRAME_STARTER	0xAA
#define MAX_FRAME_SIZE	62
#define MIN_FRAME_SIZE	5
#define UNIT_ADDRESS	0


/************************************************************************************************
 Czytanie danych i weryfikacja odebranej ramki
 Wyjœcie: true - znaleziono poprawn¹ ramkê, false - b³¹d ramki (zmienna 'error' zawiera kod b³êdu)
*************************************************************************************************/
bool WinCom::Receive() 
{
	unsigned char static temp_buffer[2048],rx,frame_buffer[2048];
	unsigned int i,data_size=0,ilosc=0,rx_ind,timeout=0,state=0; int data_ind;
	unsigned short CRC16_calc=0,CRC16_read=0;
	bool good_frame;
	timeout=0;good_frame = false;rx_ind=0;
	ilosc=Recv(&temp_buffer[0],BUF_IN_SIZE);

	if (ilosc>0)
	{
		state=0; rx=temp_buffer[0];  
		for (i=1;i<=ilosc;i++)
		{
			switch(state)
			{
				case 0:
					if ((rx==FRAME_STARTER) && (rx_ind==0))  
					{ temp_buffer[rx_ind++] = rx; state=1; CRC16Init(); good_frame=0;}
					break;
				case 1: 
					if (rx_ind==1)
					{
						if (rx <= MAX_FRAME_SIZE) 
						{temp_buffer[rx_ind++] = rx; data_size = data_ind = rx; state=2; CRC16Calc(rx);}
						else {rx_ind = 0; state=0; }
					}
					break;
				case 2:
					temp_buffer[rx_ind++] = rx; state=3; data_ind--; CRC16Calc(rx);
					break;
				case 3:
					temp_buffer[rx_ind++] = rx; data_ind--; if (data_ind<0) state = 4; else CRC16Calc(rx);
					break;
				case 4:
					temp_buffer[rx_ind] = rx;
					CRC16_read = (temp_buffer[rx_ind-1]<<8) + temp_buffer[rx_ind];
					CRC16_calc = CRC16GetValue();
 					if (CRC16_read==CRC16_calc)
					{
						memcpy(buffer_in, temp_buffer, data_size+2); 
						good_frame = 1; state = 0; rx_ind = 0; 

					} else { state = 0; rx_ind = 0; good_frame = 0;} //blad CRC RS
					break;
				default:
					break;
			}
			rx=temp_buffer[i];  
		}
	}
	else return false;
	return true;
}

int WinCom::Open(int pn,int baudrate)
{
	COMMTIMEOUTS CommTimeOuts;
	DCB dcb;

	//	ustalenie nazwy zale¿nie od systemu
	if(portNumber==-1 )
	{
		#ifdef __OS_WIN95__
			wsprintf(portName, "COM%d", pn);
		#else
			wsprintf(portName, "\\.\\\\COM%d", pn);
		#endif
	}

	// wpisanie numeru portu
	portNumber = pn;

	// parametry transmmisji
	if ((hPort=CreateFile(portName, GENERIC_READ | GENERIC_WRITE,
                  0,                    // exclusive access
                  NULL,                 // no security attrs
                  OPEN_EXISTING,
                  FILE_FLAG_NO_BUFFERING,
                  NULL ))==(HANDLE)-1 )
	{
		return -1;
	}
	SetupComm(hPort, 100, 100);

	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;//MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;	
    CommTimeOuts.WriteTotalTimeoutMultiplier = 1 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
    SetCommTimeouts(hPort, &CommTimeOuts ) ;
	GetCommState(hPort, &dcb );

	dcb.BaudRate = baudrate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if(!SetCommState(hPort, &dcb))
		return -2;
	isClosed = false;
	return 0;
}
