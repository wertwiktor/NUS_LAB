// WinCom.h: interface for the WinCom class.
//
//////////////////////////////////////////////////////////////////////
/*********************************************************************
 Obs³uga portu szeregowego pod Win 32 - nie wykorzystuje w¹tkow
 Autorzy: D. Pazderski, M. Kie³czewski, M. Kowalski
 Wersja: 23-09-2006
*********************************************************************/

#pragma once

#include "Windows.h"

#define BUF_IN_SIZE 1024

class WinCom  
{
private:
	int state;
	
	DWORD WinCom::Recv(unsigned char *ptr, size_t len)
	{
		DWORD result;
		ReadFile(hPort,ptr, len, &result, 0);
		return result;
	}

	HANDLE hPort;									
	int portNumber;					
	char portName[16];					

public:
    int error;
	bool receivedFrame;
	bool isClosed;
	unsigned char buffer_in[BUF_IN_SIZE];						// bufor wejœciowy

	WinCom() 
	{
		portNumber = -1;
		isClosed = true;
		error = 0;
		state = 1;
	}

	void Close()
	{
		if(!isClosed )
			CloseHandle(hPort);
		isClosed = true;
	}

	int Open(int port_number,int baudrate);

	bool Receive();				
	bool Send(const unsigned char *ptr, size_t len)
	{
		if(isClosed) 
		{
			Sleep(50);
			return true;		
		}

		if(state == 2) // exit
			return false;
		else
		{
			DWORD dummy;
			receivedFrame = false;
			FlushFileBuffers(hPort);	
			if(!WriteFile(hPort, ptr, len, &dummy, 0))
				return false;
		}
		return true;
	}

};

