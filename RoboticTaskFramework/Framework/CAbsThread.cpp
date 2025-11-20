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
	if (m_thread.joinable())
	{
		m_thread.join(); // 스레드가 종료될 때까지 안전하게 대기
	}
}

void CAbsThread::create()
{
	if (!m_thread.joinable())
	{
		m_thread = std::thread(&CAbsThread::threadProc, this);
	}
}

void CAbsThread::suspend()
{
	create(); // 스레드가 없으면 생성
	m_bPaused = true;
}

void CAbsThread::resume()
{
	create(); // 스레드가 없으면 생성
	m_bPaused = false;
}

void CAbsThread::setEnd()
{
	m_bExit = true;
	m_bPaused = false; // 종료 시 대기 상태를 해제하여 루프가 빠져나올 수 있도록 함
}

int CAbsThread::isThreadStatus()
{
	if (!m_thread.joinable()) return eNOT_EXIST;
	if (m_bPaused) return eSUSPEND;
	return eRUN;
}

void CAbsThread::threadProc()
{
	while (!m_bExit)
	{
		if (m_bPaused)
		{
			// 일시정지 상태일 때 CPU 낭비 없이 대기
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		sequence(); // 실제 작업 수행
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}