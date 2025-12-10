#pragma once
#include "CSharedMemory.h"

template <typename T>
class CSharedMemoryClient:public CSharedMemory<T>
{
public:
	CSharedMemoryClient(CString Name):CSharedMemory(Name)
	{
		openMapping();
	}

protected:
	
	void openMapping() 
	{
		while(NULL == m_hMap)
		{
			m_hMap = ::OpenFileMapping(
				FILE_MAP_ALL_ACCESS, 
				NULL, 
				m_strName);
		} 

		MapView();
	}
};


