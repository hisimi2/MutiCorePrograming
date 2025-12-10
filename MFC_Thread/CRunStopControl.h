#pragma once
#include "COPSwitch.h"
#include "CRobot.h"

class CRunStopControl : public CAbsThread
{
	COPSwitch&	m_startSwitch;
	CRobot&		m_robot;

public:
	CRunStopControl(CRobot& robot, COPSwitch& startSwitch)
		: m_startSwitch(startSwitch), m_robot(robot)
	{
		resume();
	}

protected:
	bool sequence()
	{
		if (m_startSwitch.getStatus()){
			m_robot.resume();
		}
		else{
			m_robot.suspend();
		}
		return 0;
	}
};

