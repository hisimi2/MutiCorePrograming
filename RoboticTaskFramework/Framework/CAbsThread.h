#pragma once
#include <thread>   // std::thread 사용
#include <atomic>   // std::atomic 사용
#include <mutex>
#include <condition_variable>
#include <chrono>

class CAbsThread
{
protected:
	std::thread m_thread;         // CWinThread 대신 std::thread 사용
	std::atomic<bool> m_bExit;    // 스레드 종료 플래그
	std::atomic<bool> m_bPaused;  // 스레드 일시정지 플래그

	// 대기/재개를 위한 동기화 객체
	std::mutex m_mtx;
	std::condition_variable m_cv;

	// 스레드 루프에서 호출되는 실제 작업 (파생 클래스에서 오버라이드)
	virtual int sequence() { return 0; };

	// 스레드 진입 함수
	void threadProc();

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

	// 스레드 제어
	void create();    // 스레드 생성 (내부에서 안전하게 1회만 생성)
	void suspend();   // 협력적 일시정지
	void resume();    // 협력적 재개 (create() 포함)
	void setEnd();    // 스레드 종료 신호

	int isThreadStatus();
};

