#include "stdafx.h"
#include "CAbsThread.h"
#include <chrono>

CAbsThread::CAbsThread()
	: m_bExit(false), m_bPaused(false)
{
}

CAbsThread::~CAbsThread()
{
	setEnd();

	if (m_thread.joinable())
	{
		m_thread.join();
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
	m_bExit = true;
	m_cv.notify_one();
}

void CAbsThread::threadProc()
{
	while (!m_bExit)
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_cv.wait(lock, [this]() { return !m_bPaused || m_bExit; });

		if (m_bExit)
			break;

		// sequence()의 반환값이 true이면 루프를 탈출하여 스레드를 종료합니다.
		if (sequence())
			break;
	}
}