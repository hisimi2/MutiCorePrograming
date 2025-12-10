#pragma once
#include "CNamedPipe.h"

template <typename T>
class CNamedPipeServer : public CNamedPipe<T>
{
	BOOL m_bRequest;
public:
	CNamedPipeServer(CString Name) :CNamedPipe(Name), m_bRequest(FALSE)
	{
		create();
	}

	~CNamedPipeServer()
	{
		if (INVALID_HANDLE_VALUE != m_hPipe)
		{
			::DisconnectNamedPipe(m_hPipe);
		}
	}

	BOOL isRequest() { return m_bRequest; }

	int receive(T& tData)
	{
		int nLen = CNamedPipe::receive(tData);
		if (0 < nLen)
		{
			m_bRequest = TRUE;
		}
		return nLen;
	}

	int send(T& tData)
	{
		if (isRequest())
		{
			m_bRequest = FALSE;
			return CNamedPipe::send(tData);
		}
		return 0;
	}

protected:

	int create()
	{
		CString strPipeName;
		strPipeName.Format(L"\\\\.\\pipe\\%s", m_strName);

		m_hPipe = ::CreateNamedPipe(strPipeName,	// name of the pipe
			PIPE_ACCESS_DUPLEX,					// 오픈모드
			PIPE_TYPE_BYTE ,					// 파이프 모드
			PIPE_UNLIMITED_INSTANCES,			// 파이프 최대 인스턴스 갯수
			sizeof(T),							// 출력 버퍼
			sizeof(T),							// 입력 버퍼
			20000,								// 타임 아웃
			NULL);								// use default security attributes

		BOOL bResult = ::ConnectNamedPipe(m_hPipe, NULL);

		DWORD dwMode = PIPE_TYPE_BYTE | PIPE_NOWAIT;
		::SetNamedPipeHandleState(m_hPipe,&dwMode,NULL,NULL);
	
		return eCREATE_ERROR;
	}
};

