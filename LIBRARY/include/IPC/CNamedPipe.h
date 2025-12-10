#pragma once

#include "IPC_CONSTANT.h"

template <typename T>
class CNamedPipe
{
public:
	CNamedPipe(CString Name) : m_strName(Name), m_hPipe(INVALID_HANDLE_VALUE) {}
	virtual ~CNamedPipe()
	{ 
		close();
	}

	CString getName() { return m_strName; }

protected:
	virtual int create() = 0;

	virtual int receive(T& tData)
	{
		DWORD dwBytesRead = 0;
		BOOL bIsOk = ::ReadFile(
			m_hPipe,
			&tData,					// the data from the pipe will be put here
			sizeof(tData),			// number of bytes allocated
			&dwBytesRead,			// this will store number of bytes actually read
			NULL);					// not using overlapped IO

		return dwBytesRead;
	}

	virtual int send(T& tData)
	{
		BOOL bIsOk = FALSE;
		DWORD dwBytesWritten = 0;

		bIsOk = ::WriteFile(m_hPipe,		// handle to our outbound pipe
			&tData,							// data to send
			sizeof(tData),					// length of data to send (bytes)
			&dwBytesWritten,				// will store actual amount of data sent
			NULL							// not using overlapped IO
		);

		if (!bIsOk)
		{
			close();
			create();
		}

		return dwBytesWritten;
	}

	void close()
	{
		if (INVALID_HANDLE_VALUE != m_hPipe)
		{
			::CloseHandle(m_hPipe);
			m_hPipe = INVALID_HANDLE_VALUE;
		}
	}
	CString m_strName;
	HANDLE	m_hPipe;
};

