#include "stdafx.h"
#include "CAbsThread.h"
#include <chrono>

CAbsThread::CAbsThread()
	: m_bExit(false), m_bPaused(true)
{
}

CAbsThread::~CAbsThread()
{
	// 안전하게 종료 요청
	setEnd();

	// notify in case thread is waiting
	{
		std::lock_guard<std::mutex> lk(m_mtx);
	}
	m_cv.notify_all();

	if (m_thread.joinable())
	{
		try
		{
			m_thread.join();
		}
		catch (...)
		{
			// join 중 예외가 올라오면 무시하여 소멸자에서 예외 발생 방지
		}
	}
}

void CAbsThread::create()
{
	// 이미 실행중이면 아무 것도 하지 않음
	if (m_thread.joinable()) return;

	// reset flags
	m_bExit.store(false);
	// 스레드 생성 (예외 안전하게)
	try
	{
		m_thread = std::thread(&CAbsThread::threadProc, this);
	}
	catch (...)
	{
		// 생성 실패 시 상태를 복구
		m_bExit.store(true);
		throw;
	}
}

void CAbsThread::suspend()
{
	create(); // 스레드가 없으면 생성
	m_bPaused.store(true);
	// wake up to re-evaluate immediately
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
				// 예외는 로그를 남기거나 흡수. 스레드가 멈추지 않도록 함.
			}

			// 짧은 대기: 즉시 종료 또는 일시정지 반영을 위해 condition_variable::wait_for 사용
			{
				std::unique_lock<std::mutex> lk(m_mtx);
				m_cv.wait_for(lk, milliseconds(1), [this](){ return m_bExit.load() || m_bPaused.load(); });
			}
		}
	}
	catch (...)
	{
		// threadProc에서 어떤 예외가 발생하더라도 프로그램을 종료시키지 않도록 안전하게 처리
	}
}