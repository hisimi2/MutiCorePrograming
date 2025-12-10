#include "pch.h"
#include "CWorkThread.h"

CWorkThread::CWorkThread()
{
	m_fn			= threadProc;
	m_pParam		= this;
	m_nStatus		= eINVALID;
	m_bWorkingEnd	= FALSE;
	m_pThread		= nullptr;
	m_hThread		= nullptr;
}

CWorkThread::~CWorkThread()
{
	destory();
}

int CWorkThread::create()
{
	if (eINVALID != m_nStatus)
	{
		return m_nStatus;
	}

	m_pThread = ::AfxBeginThread(m_fn, m_pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_nStatus = eWAIT;
	m_hThread = m_pThread->m_hThread;

	return TRUE;
}

int CWorkThread::wait()
{
	if (eINVALID == m_nStatus)
	{
		create();
	}

	if (eNOTIFY == m_nStatus)
	{
		m_pThread->SuspendThread();
		m_nStatus = eWAIT;
	}

	return m_nStatus;
}
int CWorkThread::notify()
{
	if (eINVALID == m_nStatus)
	{
		create();
	}

	if (eWAIT == m_nStatus)
	{
		m_pThread->ResumeThread();
		m_nStatus = eNOTIFY;
	}

	return FALSE;
}

int CWorkThread::destory()
{
	if (nullptr == m_hThread)
	{
		return TRUE;
	}

	if (eWAIT == m_nStatus)
	{
		::TerminateThread(m_hThread, 0);
		return FALSE; // 강제 종료
	}

	setEnd();

	DWORD dwObject = ::WaitForSingleObject(m_hThread, THREAD_END_WAITING);

	if (WAIT_OBJECT_0 != dwObject)
	{
		TerminateThread(m_hThread, 0);
		return FALSE; // 강제 종료
	}

	return TRUE; // 정상 종료
}

void CWorkThread::setEnd()
{
	m_bWorkingEnd = TRUE;
}

UINT CWorkThread::threadProc(LPVOID pParam)
{
	CWorkThread* pObject = (CWorkThread*)pParam;

	while (FALSE == pObject->m_bWorkingEnd)
	{
		Sleep(1);

		pObject->sequence();
	}

	return 0;
}
