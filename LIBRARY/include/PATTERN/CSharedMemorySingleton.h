#pragma once
#include "CSharedMemoryServer.h"
#include "CSharedMemoryClient.h"

template <class T1, typename T2>
class CSharedMemorySingleton
{
public:
	static CSharedMemorySingleton& getInstance(CString strName)
	{
		static CSharedMemorySingleton instance(strName);
		return instance;
	}

	T2* getSharedMemory()
	{
		return m_MMF.get();
	}

private:
	CSharedMemorySingleton();
	CSharedMemorySingleton(const CSharedMemorySingleton& other);
	CSharedMemorySingleton& operator=(const CSharedMemorySingleton& other);
	CSharedMemorySingleton(CString strName) :m_MMF(strName) {}
	T1 m_MMF;
};

