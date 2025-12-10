#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment( lib, "ws2_32.lib")

#include "IPC_CONSTANT.h"

template <typename T>
class CSocket
{
public:
	enum _eSOCKET{eBLOCKING = 0, eNON_BLOCKING};

	CSocket(u_short wPort,u_long dwMode ) :m_wPort(wPort), m_dwMode(dwMode)
	{
	}

	~CSocket()
	{
		if (m_uSocket)
		{
			::closesocket(m_uSocket);
			::WSACleanup();
		}
	}
	
	int send(const T& tData) 
	{
		return ::send(m_uSocket, (char*)&tData, sizeof(tData), 0);
	}

	int receive(T& tData) 
	{
		int nResult = ::recv(m_uSocket, (char*)&tData, sizeof(tData), 0);
		return nResult;
	}

protected:
	SOCKET	m_uSocket;
	u_short		m_wPort;
	u_long		m_dwMode;
};

