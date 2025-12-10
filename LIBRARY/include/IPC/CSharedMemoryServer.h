#pragma once
#include "CSharedMemory.h"

template <typename T>
class CSharedMemoryServer:public CSharedMemory<T>
{
public:
	CSharedMemoryServer(CString Name):CSharedMemory(Name)
	{
		createMapping();
	}

protected:
	void createMapping() 
	{
		m_hMap = ::CreateFileMapping( INVALID_HANDLE_VALUE,    
									NULL,											
									PAGE_READWRITE,									
									0,												
									sizeof(T),
									m_strName);			
		MapView();
	}
};

