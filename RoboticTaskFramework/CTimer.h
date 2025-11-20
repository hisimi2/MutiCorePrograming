#pragma once

#include <chrono>

class CTimer
{
public:
	// 생성자에서 초(double) 대신 밀리초(long long)를 받도록 변경
	CTimer(long long checkTimeMs);
	bool isOver();
	void startTimer();

private:
	std::chrono::milliseconds m_checkTime;
	std::chrono::steady_clock::time_point m_start;
};

