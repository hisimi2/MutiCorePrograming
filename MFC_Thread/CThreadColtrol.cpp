#include "stdafx.h"
#include "CThreadColtrol.h"
CThreadColtrol::CThreadColtrol(COPSwitch& startSwitch)
	: m_startSwitch(startSwitch)
{
}
CThreadColtrol::~CThreadColtrol()
{
	this->setEnd();
	Sleep(100);
}

bool CThreadColtrol::sequence()
{
	if (m_startSwitch.getStatus())
	{
		m_robot.resume();
	}
	else
	{
		m_robot.suspend();
	}
	return 0;
}
