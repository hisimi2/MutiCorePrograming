#include "stdafx.h"
#include "CAbsThread.h"
#include <chrono>

CAbsThread::CAbsThread()
	: m_bExit(false), m_bPaused(true) // 생성 시 초기 상태는 '일시정지'
{
}

CAbsThread::~CAbsThread()
{
	setEnd();
	// notify in case thread is waiting
	{
		std::lock_guard<std::mutex> lk(m_mtx);
	}
	m_cv.notify_all();

	if (m_thread.joinable())
	{
		m_thread.join(); // 스레드가 종료될 때까지 안전하게 대기
	}
}

void CAbsThread::create()
{
	// 이미 실행중이면 아무 것도 하지 않음
	if (m_thread.joinable()) return;

	// reset flags
	m_bExit.store(false);
	// 스레드 생성
	m_thread = std::thread(&CAbsThread::threadProc, this);
}

void CAbsThread::suspend()
{
	create(); // 스레드가 없으면 생성
	m_bPaused.store(true);
	// notify to ensure thread re-evaluates condition promptly
	m_cv.notify_all();
}

void CAbsThread::resume()
{
	create(); // 스레드가 없으면 생성
	m_bPaused.store(false);
	// 조건변수로 대기중인 스레드 깨우기
	m_cv.notify_all();
}

void CAbsThread::setEnd()
{
	m_bExit.store(true);
	// 종료 시 대기 상태를 해제하여 루프가 빠져나올 수 있도록 함
	m_bPaused.store(false);
	// notify to wake thread from condition_variable waits
	m_cv.notify_all();
}

int CAbsThread::isThreadStatus()
{
	if (!m_thread.joinable()) return eNOT_EXIST;
	if (m_bPaused.load()) return eSUSPEND;
	return eRUN;
}

void CAbsThread::threadProc()
{
	using namespace std::chrono;
	try
	{
		// 루프: 종료 플래그가 설정될 때까지 실행
		while (!m_bExit.load())
		{
			// 일시정지 처리: pause 상태이면 condition_variable로 대기(효율적)
			{
				std::unique_lock<std::mutex> lk(m_mtx);
				m_cv.wait(lk, [this](){ return m_bExit.load() || !m_bPaused.load(); });
			}

			if (m_bExit.load()) break;

			// 실제 작업 수행은 락을 걸지 않은 상태에서 호출 (sequence() 구현에 따라 자체 동기화 필요)
			try
			{
				sequence();
			}
			catch (...)
			{
				// 예외는 로그를 남기거나 무시. 여기선 안전하게 흡수하여 스레드 종료를 방해하지 않음.
			}

			// 짧은 대기: condition_variable::wait_for 를 사용해서 setEnd() 호출 시 즉시 깨어나도록 함
			{
				std::unique_lock<std::mutex> lk(m_mtx);
				// 1ms 대기 동안 exit 또는 pause 신호가 오면 즉시 반응
				m_cv.wait_for(lk, milliseconds(1), [this](){ return m_bExit.load() || m_bPaused.load(); });
			}
		}
	}
	catch (...)
	{
		// threadProc에서 어떤 예외가 발생하더라도 프로그램을 종료시키지 않도록 안전하게 처리
	}
}