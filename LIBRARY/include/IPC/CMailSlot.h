#pragma once

#include "IPC_CONSTANT.h"

template <typename T>
class CMailSlot
{
public:
	CMailSlot::CMailSlot(CString Name)
		:m_strName(Name)
		, m_hSlot(INVALID_HANDLE_VALUE)	{}

	virtual ~CMailSlot()
	{
		if (INVALID_HANDLE_VALUE != m_hSlot)
		{
			::CloseHandle(m_hSlot);
		}
	}

	CString CMailSlot::getName()
	{
		return m_strName;
	}
protected:
	CString m_strName;
	HANDLE	m_hSlot;
};

