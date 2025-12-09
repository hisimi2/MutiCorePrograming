#pragma once

class CTimer
{
public:
	CTimer(double dCheckTime);
	bool isOver();
	void startTimer();
private:
	double	m_dCheckTime;
	long	m_lStart;
};

