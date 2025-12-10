#pragma once
#include "CSocket.h"

template <typename T>
class CSocketClient:public CSocket<T>
{
public:
	CSocketClient(CString strIP, int nPort, u_long dwMode = eNON_BLOCKING) 
		: CSocket(nPort, dwMode)
		, m_strIP(strIP)
	{
		connect();
	}

protected:
	int connect()
	{
		WSADATA wsdata;
		int nRes = ::WSAStartup(MAKEWORD(0x02, 0x02), &wsdata);

		if (ERROR_SUCCESS != nRes)
		{
			return eCREATE_ERROR;
		}

		// 소켓 만들기 
		m_uSocket = ::socket(PF_INET, SOCK_STREAM, 0);

		if (INVALID_SOCKET == m_uSocket)
		{
			return eCREATE_ERROR;
		}

		// connect
		SOCKADDR_IN serverAddr; // IPv4
		serverAddr.sin_family = AF_INET;
		//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
		::inet_pton(AF_INET, (CStringA)m_strIP, &serverAddr.sin_addr);
		serverAddr.sin_port = ::htons(m_wPort);

		do 
		{
			int nRes = ::connect(m_uSocket, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));

			if(nRes==0)
			{
				break;
			}
		} while (1);

		//-------------------------
		// dwMode = 0 blocking, dwMode != 0 non-blocking 
		int nResult = ::ioctlsocket(m_uSocket, FIONBIO, &m_dwMode);

		if (nResult == SOCKET_ERROR)
		{
			::closesocket(m_uSocket);
			m_uSocket = INVALID_SOCKET;
			::WSACleanup();

			TRACE(L"ioctlsocket failed with error: %ld \n", nResult);
			return eCREATE_ERROR;
		}

		return eSUCESS;
	}

	CString m_strIP;
};

