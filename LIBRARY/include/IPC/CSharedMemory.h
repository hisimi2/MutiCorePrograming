#pragma once

template <typename T>
class CSharedMemory
{
public:
	CSharedMemory(CString Name) 	: m_hMap(NULL), m_lpData(NULL), m_strName(Name)	
	{
	}

	virtual ~CSharedMemory() 
	{
		if (m_lpData)		
		{ 
			::UnmapViewOfFile(m_lpData);	
		}

		if (m_hMap)		
		{ 
			::CloseHandle(m_hMap);		
		}
	}

	CString getName() 
	{ 
		return m_strName; 
	}

	T& getMemory() 
	{ 
		return *((T*)m_lpData);
	}

protected:
	void MapView()
	{
		m_lpData = ::MapViewOfFile(m_hMap,   // handle to map object
			FILE_MAP_ALL_ACCESS,			// read/write permission
			0,
			0,
			sizeof(T));
	}
	
	HANDLE		m_hMap;
	LPVOID		m_lpData;
	CString			m_strName;
};

