#pragma once


#include "CMailSlot.h"
template <typename T>
class CMailSlotReceiver:public CMailSlot<T>
{
public:
	CMailSlotReceiver(CString Name):CMailSlot(Name)
	{ 
		create(); 
	}
	
	int receive(T& tData)
	{
		DWORD numBytesRead = 0;
		BOOL bResult = ::ReadFile(m_hSlot, &tData, sizeof(tData),	&numBytesRead, NULL);	

		if (!bResult)
		{
			DWORD dwError = GetLastError();
			TRACE(L"%d \n", dwError);

			if (ERROR_INVALID_HANDLE == dwError)
			{
				m_hSlot = INVALID_HANDLE_VALUE;
			}
			return 0;
		}
		return numBytesRead;
	}

protected:
	int create()
	{
		if (INVALID_HANDLE_VALUE == m_hSlot)
		{
			CString strSlotName;
			strSlotName.Format(L"\\\\.\\mailslot\\%s", m_strName);

			m_hSlot = ::CreateMailslot(	strSlotName,	// 메일슬롯 주소
												0,					// 메일슬롯 버퍼크기 424 byte
												0,		    		// 최대 블록킹 시간(밀리세컨드 : ms) 
				(LPSECURITY_ATTRIBUTES)NULL);		    // 핸들 상속 여부
		}

		return (INVALID_HANDLE_VALUE == m_hSlot) ? eCREATE_ERROR : eSUCESS;
	}
};
