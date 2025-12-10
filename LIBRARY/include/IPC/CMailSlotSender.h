#pragma once
#include "IPC_CONSTANT.h"

#include "CMailSlot.h"
template <typename T>
class CMailSlotSender:public CMailSlot<T>
{
public:
	CMailSlotSender(CString Name):CMailSlot(Name) {}
	int send(const T& tData)
	{
		create();

		BOOL bResult = FALSE;
		DWORD numBytesWritten = 0;
		bResult = ::WriteFile(m_hSlot,	// 데이터를 읽어 들일 파일을 지정(Mail Slot)
			&tData,							// 전송할 데이터가 저장되어 있는 버퍼 지정
			sizeof(tData),					// 전송할 데이터 크기를 지정한다 
			&numBytesWritten,			// 전송된 실제 데이터의 크기
			NULL								// not using overlapped IO
		);

		if (!bResult)
		{
			DWORD dwError = GetLastError();
			TRACE(L"%d \n", dwError);

			if (ERROR_INVALID_HANDLE == dwError)
			{
				m_hSlot = INVALID_HANDLE_VALUE;
			}
			return eWRITE_ERROR;
		}
		return eSUCESS;
	}

protected:
	void create()
	{
		if (INVALID_HANDLE_VALUE != m_hSlot)
		{
			return ;
		}

		CString strSlotName;
		strSlotName.Format(L"\\\\.\\mailslot\\%s", m_strName);

		m_hSlot = ::CreateFile(
			strSlotName,					// 개방(open)할 파일 이름
			GENERIC_WRITE,				// 읽기/쓰기 모드를 지정한다.(or (l) 연산으로 결합)
			FILE_SHARE_READ,				// 파일 공유방식 
			NULL,								// 보안 속성
			OPEN_EXISTING,				// 파일의 생성되는 방법을 지정
												// CREATE_ALWAYS : 항상 새 파일을 생성
												// CREATE_NEW : 새 파일 생성, 같은 이름의 파일이 존재하면 생성 실패
												// OPEN_ALWAYS : 기존 파일 개방.없으면 생성
												// OPEN_EXISTING : 기존 파일 개방, 존재하지 않으면 함수 호출 실패
												// TRUNCATE_EXISTING : 기존 파일의 내용 지우고 개방.
			FILE_ATTRIBUTE_NORMAL,	// 파일의 특성 정보를 설정
			NULL								// 기존에 존재하는 파일과 동일한 특성을 가지는 새 파일을 만들때 사용
		);
	}
};





