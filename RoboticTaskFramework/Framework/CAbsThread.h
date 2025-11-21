#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class CAbsThread
{
public:
	enum class EState
	{
		NotExist = 0,
		Paused,
		Running,
	};

	CAbsThread();
	virtual ~CAbsThread();

	CAbsThread(const CAbsThread&) = delete;
	CAbsThread& operator=(const CAbsThread&) = delete;

	void pause();
	void resume();
	void setEnd();
	bool isEnd();

	EState getThreadState();

protected:
	/// <summary>
	/// sequence()를 구현할 때는 내부에 긴 블로킹(blocking) 호출이나 무한 루프를 만들지 않도록 주의해야 합니다. 
	/// 각 sequence() 호출은 짧은 시간 안에 완료되는 작은 작업 단위로 설계하는 것이 좋습니다. 
	/// 이렇게 하면 setEnd() 호출 시 스레드가 신속하게 반응하여 종료될 수 있습니다.
	/// </summary>
	/// <returns></returns>
	virtual bool sequence() { return false; };

	// 자식 클래스가 스레드 종료 여부를 안전하게 확인할 수 있도록 getter 제공
	bool isExitRequested() const { return m_bExit.load(std::memory_order_relaxed); }

private:
	void create();
	void threadProc();

	std::thread m_thread;
	std::atomic<bool> m_bExit{false};
	std::atomic<bool> m_bPaused{true};

	std::mutex m_mtx;
	std::condition_variable m_cv;
	std::once_flag m_onceFlag;

	// --- 소멸자 타임아웃을 위한 멤버 변수 추가 ---
	std::mutex m_join_mtx;
	std::condition_variable m_join_cv;
	bool m_bThreadFinished = false;
	// -----------------------------------------
};