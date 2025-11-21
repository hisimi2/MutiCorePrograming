#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class CAbsThread
{
protected:
	std::thread m_thread;
	std::atomic<bool> m_bExit;
	std::atomic<bool> m_bPaused;

	std::mutex m_mtx;
	std::condition_variable m_cv;
	std::once_flag m_onceFlag; // 스레드를 한 번만 생성하기 위한 플래그

	// sequence()의 반환 타입을 bool로 변경합니다. true를 반환하면 스레드가 종료됩니다.
	virtual bool sequence() { return false; };

	void threadProc();

private:
	void create();

public:
	enum class EState // enum _eStatus -> enum class EState로 변경
	{
		NotExist = 0,
		Paused, // Suspend -> Paused
		Running, // Run -> Running
	};

	CAbsThread();
	virtual ~CAbsThread();

	CAbsThread(const CAbsThread&) = delete;
	CAbsThread& operator=(const CAbsThread&) = delete;

	void pause();   // suspend() -> pause()
	void resume();
	void setEnd();

	EState getThreadState(); // 새 메서드 선언
};