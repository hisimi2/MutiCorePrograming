#pragma once

class CAbsThread
{
protected:
	CWinThread* m_pThread;
	HANDLE			m_hThread;

	int				m_nStatus;
	bool			m_bExit;
	DWORD			m_dwTimeOut;

	static UINT ThreadProc(LPVOID lpParam);
	int create();
	virtual int	sequence() { return 0; };
	virtual int	sequenceStatus() { return 0; };
public:
	enum _eStatus
	{
		eNOT_EXIST = 0,
		eSUSPEND,
		eRUN,
		eTIMEOUT = 1000
	};

	CAbsThread(DWORD dwTimeOut = eTIMEOUT);
	virtual		 ~CAbsThread();
	int			suspend();
	int			resume();
	void		setEnd();
	int			isThreadStatus();
};

