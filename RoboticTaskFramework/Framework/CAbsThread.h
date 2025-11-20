#pragma once
#include <thread>   // std::thread 사용
#include <atomic>   // std::atomic 사용
#include <mutex>
#include <condition_variable>

class CAbsThread
{
protected:
	std::thread m_thread;         // CWinThread 대신 std::thread 사용
	std::atomic<bool> m_bExit;    // 스레드 종료 플래그
	std::atomic<bool> m_bPaused;  // 스레드 일시정지 플래그

	// 대기/재개를 위한 동기화 객체
	std::mutex m_mtx;
	std::condition_variable m_cv;

	void threadProc();            // 스레드 함수
	virtual int	sequence() { return 0; };

public:
	enum _eStatus
	{
		eNOT_EXIST = 0,
		eSUSPEND,
		eRUN,
	};

	CAbsThread();
	virtual ~CAbsThread();

	// 복사 금지 (std::thread 멤버를 안전하게 관리하기 위함)
	CAbsThread(const CAbsThread&) = delete;
	CAbsThread& operator=(const CAbsThread&) = delete;

	void create();    // 스레드 생성 및 시작
	void suspend();   // 협력적 일시정지
	void resume();    // 협력적 재개
	void setEnd();    // 스레드 종료 신호
	int isThreadStatus();
};

