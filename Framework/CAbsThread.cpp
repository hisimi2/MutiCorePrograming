#include "stdafx.h"
#include "CAbsThread.h"


#include <cmath>

CAbsThread::CAbsThread(DWORD dwTimeOut)
{
	m_dwTimeOut = dwTimeOut;
	m_nStatus = eNOT_EXIST;
	m_bExit = false;
	m_dwTimeOut = eTIMEOUT;
}

CAbsThread::~CAbsThread()
{
	if (eNOT_EXIST == m_nStatus)
	{
		return;
	}

	DWORD dwExit = 0;
	::GetExitCodeThread(m_hThread, &dwExit); // dwExit Value = FuncProc's Return Value;
	if (STILL_ACTIVE == dwExit)
	{
		DWORD dwCheck = 0;
		dwCheck = ::WaitForSingleObject(m_hThread, m_dwTimeOut);
		if (WAIT_TIMEOUT == dwCheck)
		{
			::TerminateThread(m_hThread, 0);
		}
	}
}

int CAbsThread::create()
{
	if (eNOT_EXIST == m_nStatus)
	{
		m_pThread = ::AfxBeginThread(ThreadProc, this, THREAD_PRIORITY_HIGHEST, 0, CREATE_SUSPENDED, NULL);
		m_hThread = m_pThread->m_hThread;
		m_nStatus = eSUSPEND;

		SYSTEM_INFO siSysInfo;
		GetSystemInfo(&siSysInfo);

		int nMaxCore = (int)siSysInfo.dwNumberOfProcessors;
		int nSelectCore = nMaxCore - 3;
		if (0 > nSelectCore)
		{
			nSelectCore = 0;
		}
		int nBit = static_cast<int>(pow(2.0, nSelectCore) + 0.5);

		// 스레드의 Core 5를 지정한다. 
		SetThreadAffinityMask(m_pThread->m_hThread, nBit);
	}

	return m_nStatus;
}

int CAbsThread::suspend()
{
	create();

	if (eRUN == m_nStatus)
	{
		m_nStatus = eSUSPEND;
		m_pThread->SuspendThread();
	}

	return m_nStatus;
}

int CAbsThread::resume()
{
	create();

	if (eSUSPEND == m_nStatus)
	{
		m_nStatus = eRUN;
		m_pThread->ResumeThread();
	}

	return m_nStatus;
}

void CAbsThread::setEnd()
{
	m_bExit = true;
}

int CAbsThread::isThreadStatus()
{
	return m_nStatus;
}

UINT CAbsThread::ThreadProc(LPVOID lpParam)
{
	CAbsThread* pObject = (CAbsThread*)lpParam;
	while (!pObject->m_bExit)
	{
		pObject->sequence();
		Sleep(1);
	}
	return 0;
}