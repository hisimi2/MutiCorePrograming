#pragma once
#include "CSocket.h"

template <typename T>
class CSocketServer:public CSocket<T>
{
public:
	CSocketServer(int nPort, u_long dwMode = eNON_BLOCKING)
		: CSocket(nPort, dwMode)
	{
		accept();
	}

protected:

	int accept()
	{
		WSADATA wsdata;
		int nRes = ::WSAStartup(MAKEWORD(0x02, 0x02), &wsdata);

		if (ERROR_SUCCESS != nRes)
		{
			return eCREATE_ERROR;
		}

		// 소켓 만들기 
		m_uServer = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (INVALID_SOCKET == m_uSocket)
		{
			return eCREATE_ERROR;
		}

		// IP와 포트를 생성한 소켓에 결합 
		SOCKADDR_IN serverAddr; // IPv4
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
		serverAddr.sin_port = ::htons(m_wPort);

		nRes = ::bind(m_uServer, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));

		if (ERROR_SUCCESS != nRes)
		{
			return eCREATE_ERROR;
		}

		// Listen 
		nRes = ::listen(m_uServer, SOMAXCONN);

		if (ERROR_SUCCESS != nRes)
		{
			return eCREATE_ERROR;
		}

		sockaddr accept_addr;
		int nLen = sizeof(accept_addr);
		m_uSocket = ::accept(m_uServer, &accept_addr, &nLen);

		if (INVALID_SOCKET == m_uSocket)
		{
			::closesocket(m_uServer);
			return 0;
		}

		//-------------------------
		// If iMode = 0, blocking is enabled; 
		// If iMode != 0, non-blocking mode is enabled.
		int nResult = ::ioctlsocket(m_uSocket, FIONBIO, &m_dwMode);
		if (nResult != NO_ERROR)
		{
			TRACE(L"ioctlsocket failed with error: %ld\n", nResult);
			return eCREATE_ERROR;
		}
		return eSUCESS;
	}

	SOCKET m_uServer;
};

