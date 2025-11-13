#include "stdafx.h"
#include "CTimer.h"
#include <time.h>

CTimer::CTimer(double dCheckTime)
{
	this->m_dCheckTime = dCheckTime;
	this->startTimer();
}

bool CTimer::isOver()
{
	long lNow = 0;

	lNow = clock();
	double dElapsedTime = static_cast<double>(lNow - this->m_lStart) / static_cast<double>(CLOCKS_PER_SEC);

	if (dElapsedTime >= this->m_dCheckTime)
	{
		return true;
	} // end if( dElapsedTime >= this->m_dCheckTime )
	else
	{
		return false;
	} // end else
}

/******************************************************************************
  AUTHOR	: 장원희
  NAME		: startTimer
  COMMENTS	: 현재 시각을 멤버변수 m_lStart에 저장
******************************************************************************/

void CTimer::startTimer()
{
	this->m_lStart = clock();
}