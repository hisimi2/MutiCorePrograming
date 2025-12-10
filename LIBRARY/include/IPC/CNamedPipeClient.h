#pragma once
#include "CNamedPipe.h"

template <typename T>
class CNamedPipeClient : public CNamedPipe<T>
{
public:
	CNamedPipeClient(CString Name):CNamedPipe(Name)
	{
		create();
	}

	int request(T& tData)
	{
		send(tData);
		return receive(tData);
	}

protected:
	int create()
	{
		CString strPipeName;
		strPipeName.Format(L"\\\\.\\pipe\\%s", m_strName);

		// 연결요청
		m_hPipe = ::CreateFile(	strPipeName,
								GENERIC_READ | GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								0,
								NULL);

		if (INVALID_HANDLE_VALUE != m_hPipe)
		{
			return eSUCESS;
		}

		DWORD dwErrCode = ::GetLastError();
		if (dwErrCode != ERROR_PIPE_BUSY)
		{
			// 접속을 수용할 파이프의 인스턴스가 없음
			return eCREATE_ERROR;
		}

		return eSUCESS;
	}
};


