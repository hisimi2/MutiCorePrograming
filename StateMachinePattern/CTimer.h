#pragma once

#include <chrono>

class CTimer
{
public:
	
	CTimer(long long checkTimeMs);
	bool isOver();
	void startTimer();

private:
	std::chrono::milliseconds m_checkTime;
	std::chrono::steady_clock::time_point m_start;
};

