#pragma once

#include <afxwin.h>

#define THREAD_END_WAITING (5000)

class CWorkThread
{
public:
	enum _eSTATUS { eINVALID, eWAIT, eNOTIFY };

	CWorkThread();
	virtual ~CWorkThread();
	int		wait();		// suspend
	int		notify();   // resume

protected:
	AFX_THREADPROC	m_fn;
	LPVOID			m_pParam;
	CWinThread*		m_pThread;

	int				create();
	int				destory();

	static UINT		threadProc(LPVOID pParam);
	virtual int		sequence() = 0;
	
private:
	HANDLE			m_hThread;
	int				m_nStatus;
	BOOL			m_bWorkingEnd;
	void			setEnd();	// 스레드 시퀀스 종료 셋팅
};

