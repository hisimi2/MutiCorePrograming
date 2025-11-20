#include "stdafx.h"
#include "CTimer.h"
#include <time.h>

CTimer::CTimer(long long checkTimeMs) : m_checkTime(checkTimeMs)
{
	startTimer();
}

bool CTimer::isOver()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start);
	return elapsed >= m_checkTime;
}

void CTimer::startTimer()
{
	m_start = std::chrono::steady_clock::now();
}