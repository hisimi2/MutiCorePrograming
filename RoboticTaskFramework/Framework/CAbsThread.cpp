#include "stdafx.h"
#include "CAbsThread.h"
#include <chrono>

CAbsThread::CAbsThread()
	: m_bExit(false), m_bPaused(false)
{
}

CAbsThread::~CAbsThread()
{
	setEnd(); // 1. 스레드에 종료를 요청합니다.

	if (m_thread.joinable())
	{
		std::unique_lock<std::mutex> lock(m_join_mtx);

		// 2. m_join_cv를 사용하여 최대 2초 동안 스레드 종료를 기다립니다.
		//    - 스레드가 정상 종료되면(m_bThreadFinished == true), wait_for는 즉시 true를 반환합니다.
		//    - 2초가 지나도 종료되지 않으면, wait_for는 false를 반환합니다.
		if (m_join_cv.wait_for(lock, std::chrono::seconds(2), [this] { return this->m_bThreadFinished; }))
		{
			// 타임아웃 내에 스레드가 정상 종료된 경우
			m_thread.join();
		}
		else
		{
			// 타임아웃 발생: sequence()가 무한 대기에 빠진 것으로 간주합니다.
			// 스레드를 강제로 분리하여 메인 스레드(UI)가 종료될 수 있도록 합니다.
			// 경고: 이 스레드는 좀비 상태로 남아 리소스를 계속 차지할 수 있습니다.
			m_thread.detach();
		}
	}
}

CAbsThread::EState CAbsThread::getThreadState()
{
	if (!m_thread.joinable())
		return EState::NotExist;
	if (m_bPaused)
		return EState::Paused;
	return EState::Running;
}

void CAbsThread::create()
{
	std::call_once(m_onceFlag, [this]() {
		m_thread = std::thread(&CAbsThread::threadProc, this);
		});
}

void CAbsThread::pause()
{
	create();
	std::unique_lock<std::mutex> lock(m_mtx);
	m_bPaused = true;
}

void CAbsThread::resume()
{
	create();
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_bPaused = false;
	}
	m_cv.notify_one();
}

void CAbsThread::setEnd()
{
	// m_bExit는 atomic이므로 뮤텍스 없이 안전하게 접근 가능
	if (!m_bExit)
	{
		m_bExit = true;
		// wait 상태의 스레드를 깨우기 위해 notify 호출
		m_cv.notify_one();
	}
}


void CAbsThread::threadProc()
{
	while (!m_bExit)
	{
		{
			std::unique_lock<std::mutex> lock(m_mtx);
			m_cv.wait(lock, [this]() { return !m_bPaused || m_bExit; });

			if (m_bExit)
				break;
		}

		if (sequence())
		{
			m_bExit = true;
		}
	}

	// --- 스레드 종료 직전, 소멸자에게 알리는 코드 추가 ---
	{
		std::lock_guard<std::mutex> lock(m_join_mtx);
		m_bThreadFinished = true; // 종료 상태 플래그를 true로 설정
	}
	m_join_cv.notify_one(); // 대기 중인 소멸자에게 스레드가 종료되었음을 알림
	// ----------------------------------------------------
}